#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include "synth_wind.h"

//filter and separate velocity data based on states

float **v_fltr(float *v, int N, int *nf, int **st_lim, int term){

	int i, j, *cnt;
	float **res;

	res = malloc(term*sizeof(res));
	cnt = malloc(term*sizeof(int));

	for(i=0; i<term; i++){
		*(nf+i) = 0;
		*(cnt+i) = 0;
	}

	for(i=0; i<N; i++)
		for(j=0; j<term; j++)
			if(*(v+i) >=*(*(st_lim+j)) && *(v+i) <=*(*(st_lim+j)+1))
				(*(nf+j))++;
//	for(i=0; i<term; i++)
		for(i=0; i<term; i++)
			*(res+i) = malloc(*(nf+i)*sizeof(float));

	for(i=0; i<N; i++){
		for(j=0; j<term; j++){
			if(*(v+i) >=*(*(st_lim+j)) && *(v+i) <=*(*(st_lim+j)+1)){
				*(*(res+j)+(*(cnt+j))) = *(v+i);
				(*(cnt+j))++;
			}
		}
	}

	free(cnt);

//	for(i=0; i<term; i++)
//		printf("%d %d\n", i, *(nn+i));

	return(res);
}
