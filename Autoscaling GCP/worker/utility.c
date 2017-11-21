#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "utility.h"

int createSocket(int type, int aFm, short pNum, int bAdd, int nListen){
	struct sockaddr_in lAdd;
	int fd, rt, sz = sizeof(struct sockaddr_in);
	if(type == 1)
		fd = socket(aFm, SOCK_STREAM, 0);
	else
		fd = socket(aFm, SOCK_DGRAM, 0);
	if(fd==-1) exit_on_error("createSocket:socket");
	bzero(&lAdd, sz);
	lAdd.sin_family = aFm;
	lAdd.sin_port = htons(pNum);
	lAdd.sin_addr.s_addr = bAdd;
	rt = bind(fd, aCast(&lAdd), sz);
	if(rt==-1) exit_on_error("createSocket:bind");
	if(type==1 && nListen > 0) { rt = listen(fd, nListen);  // listen operation not supported for UDP
				  if(rt==-1) exit_on_error("createSocket:listen"); }
	return fd;
}