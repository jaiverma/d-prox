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


#ifndef DNS_H
#define DNS_H

struct dns_hdr {
    uint16_t dns_id;
    uint16_t dns_qr_opcode_aa_tc_rd_ra_z_rcode;
    uint16_t dns_qdcount;
    uint16_t dns_ancount;
    uint16_t dns_nscount;
    uint16_t dns_arcount;
    #define dns_qr 15
    #define dns_opcode 11
};

struct dns_question {
	uint16_t dns_qname;
	uint16_t dns_qtype;
	uint16_t dns_qclass;
};

struct dns_rr {
	uint16_t dns_name;
	uint16_t dns_type;
	uint16_t dns_class;
	uint16_t dns_ttl;
	uint16_t dns_rdlength;
	uint16_t dns_rdata;
};

void decode_dns(const u_char*);

#endif
