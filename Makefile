CC=gcc

d-prox: d-prox.c dns.h dns.c d-prox.h
	$(CC) -o d-prox d-prox.c dns.c
