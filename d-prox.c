/*
	d-prox
	Copyright (C) 2016 jaiverma
	jai2.verma@outlook.com
	www.jaiverma.com

	This file is part of d-prox.

    d-prox is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    d-prox is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with d-prox.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "dns.h"
#include "d-prox.h"

#define MAX_DATA_SIZE 2048

void udp_interceptor();
void dns_resolution(void*, u_char*, char*);

void decode_dns(const u_char*); //this function is incomplete, it only decodes the header as of now

int main(void)
{
	fprintf(stdout, "Listening for DNS requests on %s:%s\n", DNS_SERVER, DNS_PORT);
	fprintf(stdout, "Using %s:%s as DNS Server\n", SOCKS_SERVER, SOCKS_PORT);

	udp_interceptor();

	return 0;
}

void udp_interceptor()
{
	int dns_sock, r, yes=1;
	char numbytes;
	u_char buffer[MAX_DATA_SIZE];
	struct addrinfo dns_listener, *servinfo;
	struct sockaddr_in dns_client;
	socklen_t addr_len;

	memset(&dns_listener, 0, sizeof dns_listener);
	dns_listener.ai_family = AF_INET;
	dns_listener.ai_socktype = SOCK_DGRAM;

	if ((r = getaddrinfo(DNS_SERVER, DNS_PORT, &dns_listener, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
		exit(1);
	}

	if ((dns_sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
	{
		perror("socket\n");
		exit(1);
	}

	if (setsockopt(dns_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt");
		exit(1);
	}

	if (bind(dns_sock, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
	{
		close(dns_sock);
		perror("bind\n");
		exit(1);
	}

	freeaddrinfo(servinfo);

	while(1)
	{
		memset(&buffer, 0, MAX_DATA_SIZE);

		addr_len = sizeof(dns_client);
		if((numbytes = recvfrom(dns_sock, buffer, MAX_DATA_SIZE-1, 0, (struct sockaddr*)&dns_client, &addr_len)) == -1)
		{
			continue;
		}

		fprintf(stdout, "Got a DNS packet!\n\n");
		decode_dns(buffer);

		//hexdump of packet
		int i = 0;
		fprintf(stdout, "Hexdump\n");
		for (; i < numbytes; i++)
			fprintf(stdout, "%02x ", buffer[i]);
		fprintf(stdout, "\n\n\n\n");

		u_char *dns_query = malloc(numbytes+3);
		dns_query[0] = 0;
		dns_query[1] = numbytes;	//a tcp dns query requires the length(2 bytes) of the dns query to precede the query, refer to the DNS RFC for more details!
		memcpy(dns_query+2, buffer, numbytes);

		numbytes += 2;
		dns_resolution(dns_query, buffer, &numbytes);

		//as the lenth is prepended to the response packet, we send the packet back from after the length i.e 2 bytes
		sendto(dns_sock, buffer+2, numbytes-2, 0, (struct sockaddr*)&dns_client, sizeof(dns_client));

		fprintf(stdout, "Response\n\n");
		decode_dns(buffer+2);

		//hexdump of response
		i = 2;
		fprintf(stdout, "Hexdump:\n");
		for (; i < numbytes; i++)
			fprintf(stdout, "%02x ", buffer[i]);
		fprintf(stdout, "\n\n\n\n");

		free(dns_query);
	}

	close(dns_sock);
}

void dns_resolution(void *dns_query, u_char *buffer, char *len)
{
	int socks_sock;
	struct addrinfo socks_server, *servinfo;
	int r;
	u_char data[MAX_DATA_SIZE];

	memset(&socks_server, 0, sizeof socks_server);
	socks_server.ai_family = AF_INET;
	socks_server.ai_socktype = SOCK_STREAM;

	if ((r = getaddrinfo(SOCKS_SERVER, SOCKS_PORT, &socks_server, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
		exit(1);
	}

	if ((socks_sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
	{
		perror("socket\n");
		exit(1);
	}

	if (connect(socks_sock, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
	{
		close(socks_sock);
		perror("connect\n");
		exit(1);
	}

	freeaddrinfo(servinfo);

	//initial greeting from client
	//socks version 0x05, number of auth. methods 0x01, auth. method 0x00(no auth.)
	//for more details check out the SOCKS5 RFC
	send(socks_sock, "\x05\x01\x00", 3, 0);
	recv(socks_sock, data, MAX_DATA_SIZE, 0);

	//using 8.8.8.8 as DNS Server
	memset(&data, 0, MAX_DATA_SIZE);
	in_addr_t dns = inet_addr(REMOTE_DNS_SERVER);

	//client connection request
	memcpy(data, "\x05\x01\x00\x01", 4);
	memcpy(data+4, &dns, 4);
	memcpy(data+8, "\x00\x35", 2);

	send(socks_sock, data, 10, 0);
	recv(socks_sock, data, MAX_DATA_SIZE, 0);

	//dns query
	send(socks_sock, dns_query, *len, 0);
	*len = recv(socks_sock, buffer, MAX_DATA_SIZE, 0);

	close(socks_sock);
}
