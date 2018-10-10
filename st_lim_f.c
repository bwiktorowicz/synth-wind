
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include "synth_wind.h"

//Generate arrays holding upper and lower limit of each state

int **st_lim_f(int max_spd, int term, int flag){

	int i, j, state_sz;
	int **st_lim;

		st_lim = malloc(term*sizeof(st_lim));

		for(i=0; i<term; i++)
			*(st_lim+i) = malloc(2*sizeof(int));

		state_sz = ceil(max_spd/term);
	
		for(i=0; i<term-1; i++){
			**(st_lim+i) = i*state_sz;
			*(*(st_lim+i)+1) = state_sz+i*state_sz;
		}

		**(st_lim+i) = i*state_sz;
		*(*(st_lim+i)+1) = max_spd;


        if(PR_MGD && !flag){
		printf("\n --------------------------------\n");
		printf(" Markov Chain State Ranges\n");
		printf(" --------------------------------\n");
		printf(" \033[31;1mSTATE%9s\033[0m\n", "RANGE");
	}
	else if(PR_STATE && flag){
		printf("\n --------------------------------\n");
		printf(" Mixed Gaussian Term Ranges\n");
		printf(" --------------------------------\n");
		printf(" \033[31;1mTERM%9s\033[0m\n", "RANGE");
	}
	

	if(PR_MGD || PR_STATE){

		for(i=0; i<term; i++)
			printf(" %3d%7d%4d\n", i, *(*(st_lim+i)), *(*(st_lim+i)+1));

		printf(" --------------------------------\n");
	}

	return(st_lim);
}									
