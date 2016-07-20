# d-prox

d-prox is a DNS proxy which tunnels DNS requests over a SOCKS proxy using TCP. This allows you to use a SOCKS proxy (e.g. Tor) for name resolution.

For more details and a tutorial, check out <http://www.jaiverma.com/blog/proxying-dns/>

## Version

0.1

## Configuration

Open up `d-prox.h` in a text editor and change the configuration to suit your requirement.

Examples:

1. For running d-prox with Tor Browser. Assuming tor is running on 127.0.0.1 using port 9050

````d-prox.h```
```C
#define DNS_PORT "53" //port to listen on
#define SOCKS_PORT "9050" //port to connect to (port of SOCKS proxy)(Tor)
#define DNS_SERVER "0.0.0.0" //address to listen on
#define SOCKS_SERVER "127.0.0.1" //address of SOCKS proxy(Tor)
#define REMOTE_DNS_SERVER "8.8.8.8" //dns server to be used by proxy for name resolution (Google DNS)
````

1. Using a remote machine as a SOCKS Server for DNS Resolution. This requires the remote machine to be accessible remotely via the host. If you are using a public IP, this can be done my using a service such as <http://dynu.com>. If the machine is behind a NAT, you will also have to set up port forwarding for this machine.

On the Ubuntu machine to be used as the SOCKS server, run the following command to set it up as a SOCKS proxy

```sh
ssh -N -D 0.0.0.0:8888 localhost
```

Then on the host machine

````d-prox.h```
```C
#define DNS_PORT "53" //port to listen on
#define SOCKS_PORT "8888" //port to connect to (port of SOCKS proxy)(port of remote machine)
#define DNS_SERVER "0.0.0.0" //address to listen on
#define SOCKS_SERVER "192.168.43.75" //address of SOCKS proxy(address of remote machine)
#define REMOTE_DNS_SERVER "8.8.8.8" //dns server to be used by proxy for name resolution (Google DNS)
````

## Installation

d-prox works on Linux and OS X. To install, download and unzip the repository.

```sh
$ cd d-prox
$ make
```

If the configuration `d-prox.h` is changed, run `make` again.

You need to run this program as root because it binds to port 53 of your local machine. To change this, edit `d-prox.h`.

```sh
$ [sudo] ./d-prox
```

To test if the program is working:

```sh
$ dig @127.0.0.1 google.com [-p <port> (if you have changed the default port)]
```

## License

d-prox Copyright (C) 2016 jaiverma

jai2.verma@outlook.com www.jaiverma.com

This file is part of d-prox.

d-prox is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

d-prox is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with d-prox. If not, see <http://www.gnu.org/licenses/>.
