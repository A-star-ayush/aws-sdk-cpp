#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../headers/mode1.h"
#include "../headers/mode2.h"

#define MAX_FOREST_STRING_SIZE 100
#define _DEBUG_ 1

char** getForests(int** mat, int nodes, int forests) {
	int i, j, k;
	char** f = (char**)malloc(sizeof(char*)*forests);
	for(i = 0; i < forests; ++i)
		f[i] = (char*)malloc(sizeof(char)*MAX_FOREST_STRING_SIZE);

	int* lengths = (int*)malloc(sizeof(int)*forests);
	for(i = 0; i < forests; ++i)
		lengths[i] = 0;

	for(j = 0; j < nodes; ++j) {
		int i = 0;
		for(k = 0; k < nodes; ++k) {
			if(mat[j][k] != 0) {
				lengths[i] += sprintf(f[i] + lengths[i], "%d %d ", j, k);
				++i;
			}
		}
	}
	
	#if _DEBUG_ > 0
		puts("\nThe division of forests is as follows: ");
		for(i = 0; i < forests; ++i)
			puts(f[i]);
	#endif

	for(i = 0; i < nodes; ++i)
		free(mat[i]);
	free(mat);

	free(lengths);

	return f;
}

int main()
{
	int nodes;
	scanf("%d", &nodes);

	int i, j;
	
	int** adjMat;
	adjMat = (int**)malloc(sizeof(int*)*nodes);
	for(i=0;i<nodes;++i)
		adjMat[i] = (int*)malloc(sizeof(int)*nodes);

	for(i = 0; i < nodes; ++i)
		for(j = 0; j < nodes; ++j)
			adjMat[i][j] = 0;

	for(i = 0; i < nodes; ++i){
		int sz;
		scanf("%d", &sz);

		for(j = 0; j < sz; ++j){
			int k;
			scanf("%d", &k);
			adjMat[k][i] = 1;
		}
	}

	#if _DEBUG_ > 0
		puts("\nThe adjacency matrix: ");
		for(i = 0; i < nodes; ++i){
			for(j = 0; j < nodes; ++j)
				printf("%d ", adjMat[i][j]);
			puts("");
		}
		puts("");
	#endif

	int max_degeree = 0;

	for(i=0;i<nodes;++i){
		int out_edges = 0;
		for(j=0;j<nodes;++j) {
			out_edges += adjMat[i][j];
		}
		if(out_edges > max_degeree)
			max_degeree = out_edges;
	}

	printf("No of forests : %d\n", max_degeree);
	printf("Enter mode: ");

	int mode;
	scanf("%d", &mode);
	
	int* colors = NULL;

	switch(mode) {
		case 1: colors = mode1(getForests(adjMat, nodes, max_degeree), nodes, max_degeree);
				break;
		case 2: colors = mode2(getForests(adjMat, nodes, max_degeree), nodes, max_degeree);
				break;
		default: puts("Invalid mode");
	}

	if(colors != NULL){
		for(i = 0; i < nodes; ++i)
			printf("%d ", colors[i]);
		puts("");
		free(colors);
	}

	return 0;
}
