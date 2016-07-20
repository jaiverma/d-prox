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
#include <sys/types.h>
#include "dns.h"

//decodes the dns header
void decode_dns(const u_char *query)
{
	const struct dns_hdr *d;
	d = (const struct dns_hdr*)query;

	fprintf(stdout, "\t:::::::::::::::: Decoding DNS packet ::::::::::::::::\n");

	fprintf(stdout, "\t[\tID = %hu\t]\n", ntohs(d->dns_id));

	int qr = ntohs(d->dns_qr_opcode_aa_tc_rd_ra_z_rcode)>>dns_qr & 1;
	int opcode = ntohs(d->dns_qr_opcode_aa_tc_rd_ra_z_rcode)>>dns_opcode & 7;
	int rcode = ntohs(d->dns_qr_opcode_aa_tc_rd_ra_z_rcode) & 7;

	fprintf(stdout, "\t[\tQR = %d", qr);
	if (qr == 0)
		fprintf(stdout, "(Query)\t]\n");
	else
		fprintf(stdout, "(Resp.)\t]\n");

	fprintf(stdout, "\t[\tOPCODE = %d", opcode);
	switch (opcode)
	{
		case 0:
			fprintf(stdout, "(Standard Query)\t]\n");
			break;
		case 1:
			fprintf(stdout, "(Inverse Query)\t]\n");
			break;
		case 3:
			fprintf(stdout, "(Server Status Query)\t]\n");
			break;
		default:
			fprintf(stdout, "(Reserved)\t]\n");
			break;
	}

	fprintf(stdout, "\t[\tRCODE = %d", rcode);
	switch (rcode)
	{
		case 0:
			fprintf(stdout, "(No Error)\t]\n");
			break;
		case 1:
			fprintf(stdout, "(Format Error)\t]\n");
			break;
		case 2:
			fprintf(stdout, "(Server Failure)\t]\n");
			break;
		case 3:
			fprintf(stdout, "(Name Error)\t]\n");
			break;
		case 4:
			fprintf(stdout, "(Not Implemented)\t]\n");
			break;
		case 5:
			fprintf(stdout, "(Refused)\t]\n");
			break;
		default:
			fprintf(stdout, "(Unknown Error)\t]\n");
			break;
	}
	fprintf(stdout, "\t-----------------------------------------------------\n\n");
}
