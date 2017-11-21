#ifndef _UTILITY_H
#define _UTILITY_H

#include <stdio.h>
#include <unistd.h>

#define exit_err(msg) \
	do { fprintf(stderr, "%s\n", msg); exit(EXIT_FAILURE); } while(0)

#define exit_on_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

#define aCast(x) (struct sockaddr*)x

int createSocket(int type, int addressFamily, unsigned short portNum, unsigned int bindingAddress, int QLen);

#endif