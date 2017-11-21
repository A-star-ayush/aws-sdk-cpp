#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "../headers/mode1.h"
#include "../headers/utility.h"

#define _DEBUG_ 1

int* mode1(char** forestStrings, int nodes, int nforests) {

	int* colors = (int*)calloc(nodes, sizeof(int));
	
	int i;
	for(i = 0; i < nodes; ++i)
		colors[i] = 0;

	for(i = 0; i < nforests; ++i) {
		int fd = createSocket(1, AF_INET, 45000, INADDR_ANY, 0);
		
		struct sockaddr_in worker;
		memset(&worker, 0, sizeof(struct sockaddr_in));
		worker.sin_family = AF_INET;
		worker.sin_port = htons(35000);
		inet_aton("127.0.0.1", &worker.sin_addr);
		
		int rt = connect(fd, aCast(&worker), sizeof(struct sockaddr_in));
		if(rt < 0)
			exit_on_error("connect");

		char buf[BUFSIZ];
		sprintf(buf, "%d ", nodes);
		strcat(buf, forestStrings[i]);

		rt = write(fd, buf, strlen(buf) + 1);
		if(rt < 0)
			exit_on_error("write");

		#if _DEBUG_ > 0
			printf("Sent: %s\n", buf);
		#endif

		rt = read(fd, buf, BUFSIZ); 
		if(rt < 0)
			exit_on_error("read");

		#if _DEBUG_ > 0
			if(rt > 0)
				printf("Received: %s\n", buf);
		#endif

		int j;
		char* _buf = buf;
		for(j = 0; j < nodes; ++j) {
			int c = strtol(_buf, &_buf, 10);
			if (c == 1)
				colors[j] |= 1 << i;
		}

		close(fd);
	}

	for(i = 0; i< nforests; ++i)
		free(forestStrings[i]);

	return colors;
}