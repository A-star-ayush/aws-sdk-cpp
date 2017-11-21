#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "utility.h"

#define MAX_FOREST_STRING_SIZE 100
#define _DEBUG_ 1

void colorForest(int fd, char* buf) {
	char* buf_cpy = buf;  // buf will be moved around, so we store the original address to use for free
	int nodes = strtol(buf, &buf, 10);
	buf = buf + 1;

	int* colors = (int*)malloc(sizeof(int)*nodes);
	int i;
	for(i = 0; i < nodes; ++i)
		colors[i] = 2;

	while(1) {		
		
		if(*buf == '\0')
			break;

		int n1 = strtol(buf, &buf, 10);
		int n2 = strtol(buf, &buf, 10);

		if(n1 == 0 && n2 == 0)
			break;

		#if _DEBUG_ > 0
			printf("n1: %d n2: %d\n", n1, n2);
		#endif

		if(colors[n1] < 2) {
			if (colors[n1] == 0)
				colors[n2] = 1;
			else
				colors[n2] = 0;
		} else if(colors[n2] < 2) {
			if (colors[n2] == 0)
				colors[n1] = 1;
			else
				colors[n1] = 0;
		} else {
			colors[n1] = 0;
			colors[n2] = 1;
		}
	}

	int length = 0;

	char answer[BUFSIZ];
	for(i = 0; i < nodes; ++i) 
		length += sprintf(answer + length, "%d ", colors[i]);

	int rt = write(fd, answer, strlen(answer) + 1);
	if(rt < 0) 
		exit_on_error("write");
	close(fd);

	#if _DEBUG_ > 0
		printf("Output sent: %s\n", answer);
	#endif

	free(buf_cpy);
	free(colors);
}


int main(int argc, char const *argv[])
{
	int fd = createSocket(1, AF_INET, 35000, INADDR_ANY, 5);

	while(1) {
		int master = accept(fd, NULL, NULL);
		char* buf = (char*)malloc(sizeof(char)*MAX_FOREST_STRING_SIZE);
		int rt = read(master, buf, MAX_FOREST_STRING_SIZE);
		if(rt < 0) exit_on_error("read");
		else if(rt > 0) {
			#if _DEBUG_ > 0
				printf("Input received: %s\n", buf);
			#endif
			colorForest(master, buf);
		}
	}

	return 0;
}


