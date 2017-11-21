#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <pthread.h>

#include "../headers/mode2.h"
#include "../headers/ec2.h"
#include "../headers/utility.h"

#define _DEBUG_ 1

struct context {
	int fd, nodes, num;
	char* str;
};

typedef struct context context;

int* colors;

pthread_mutex_t mcolor = PTHREAD_MUTEX_INITIALIZER;

void* on_thread_start(void* _ctx) {

	context* ctx = (context*)_ctx;
		
	char buf[BUFSIZ];
	sprintf(buf, "%d ", ctx->nodes);
	strcat(buf, ctx->str);

	int rt = write(ctx->fd, buf, strlen(buf) + 1);
	if(rt < 0)
		exit_on_error("write");

	#if _DEBUG_ > 0
		printf("Sent: %s\n", buf);
	#endif

	rt = read(ctx->fd, buf, BUFSIZ); 
	if(rt < 0)
		exit_on_error("read");

	#if _DEBUG_ > 0
		if(rt > 0)
			printf("Received: %s\n", buf);
	#endif

	int j;
	char* _buf = buf;
	for(j = 0; j < ctx->nodes; ++j) {
		int c = strtol(_buf, &_buf, 10);
		if (c == 1) {
			pthread_mutex_lock(&mcolor);
			colors[j] |= 1 << ctx->num;
			pthread_mutex_unlock(&mcolor);
		}
	}

	close(ctx->fd);
	free(_ctx);

	return NULL;
}

int* mode2(char** forestStrings, int nodes, int nforests) {

	colors = (int*)calloc(nodes, sizeof(int));
	
	myAccount* acc = account("AKxxxTDKO3xxxSYIxxUQ", "AqxxxxxZVDwbinAXxGXnh0xxxxxoL9gv9zxxxxuM", "ap-south-1");
	std::vector<Instance*> ins = getInstances(acc);

	#if _DEBUG_ > 0
		puts("");
		printf("#instances : %d\n", acc->total);
		printf("#running : %d\n", acc->total - acc->stopped - acc->others);
		for(Instance* i: ins)
			printf("state: %s id: %s ip: %s\n", i->state == 80 ? "stopped" : i->state == 16 ? "running" : "other", i->id, i->ip);
		puts("");
	#endif

	int running = acc->total - acc->stopped - acc->others;
	int diff = nforests - running;

	if(diff <= 0) {
		puts("Sufficient resources available.");
	} else {
		int diff2 = diff - acc->stopped;
		if(diff2 <= 0) {
			printf("Starting %d instance(s) to match resource requirement\n", diff);
			startInstances(acc, ins, diff);
		}
		else {
			printf("Starting %d instance(s) and creating %d instance(s) to match resource requirement\n", acc->stopped, diff2);
			startInstances(acc, ins, acc->stopped);
			createInstances(acc, ins, diff2);
		}
	}

	printf("\npress enter to continue ");
	getchar();
	getchar();

	int i, index = 0;
	pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t)*nforests);

	for(i = 0; i < nforests; ++i) {
		int fd = createSocket(1, AF_INET, 45000 + i, INADDR_ANY, 0);
		
		struct sockaddr_in worker;
		memset(&worker, 0, sizeof(struct sockaddr_in));
		worker.sin_family = AF_INET;
		worker.sin_port = htons(35000);
		while(ins[index]->state != 16)
			index += 1;
		inet_aton(ins[index]->ip, &worker.sin_addr);
		
		int rt = connect(fd, aCast(&worker), sizeof(struct sockaddr_in));
		if(rt < 0)
			exit_on_error("connect");

		context* x = (context*)malloc(sizeof(context));
		x->fd = fd;
		x->nodes = nodes;
		x->str = forestStrings[i];
		x->num = i;
		pthread_create(&threads[i], NULL, on_thread_start, x);
	}

	for(i = 0; i < nforests; ++i)
		pthread_join(threads[i], NULL);

	free(threads);

	for(i = 0; i< nforests; ++i)
		free(forestStrings[i]);


	for(Instance* i : ins)
		free(i);

	return colors;
}