#include "synth_wind.h"

double **markov(int **st_lim, float *v, int N){

	double **trans;
	int i, j, st0, st1, *st_num, state_sz;

	st_num = malloc(STATE*sizeof(int));
	trans = malloc(STATE*sizeof(trans));
	for(i=0; i<STATE; i++)
		*(trans+i) = malloc(STATE*sizeof(double));

	//generage probability state transition matrix

	for(i=0; i<STATE; i++){
		for(j=0; j<STATE; j++)
			*(*(trans+i)+j) = 0;
		*(st_num+i) = 0;
		}

	for(i=0; i<N-1; i++){
		for(j=0; j<STATE; j++){
			if(*(v+i)>=*(*(st_lim+j)) && *(v+i)<=*(*(st_lim+j)+1))
				st0 = j;
			if(*(v+i+1)>=*(*(st_lim+j)) && *(v+i+1)<=*(*(st_lim+j)+1))
				st1 = j;
		}
		*(*(trans+st0)+st1)+=1;
		*(st_num+st1)+=1;
	}

	for(i=0; i<STATE; i++){
		for(j=0; j<STATE; j++)
			*(*(trans+i)+j)/=(double)*(st_num+i);
			}

	//cleanup

	free(st_num);

	//print matrix if required

	if(PR_TRN){

		printf("\n ");

		for(i=0; i<STATE; i++)
			printf("-------");

		printf("\n State Transition Probability Matrix\n ");

		for(i=0; i<STATE; i++)
			printf("-------");

		for(i=0; i<STATE; i++){
			printf("\n");
			for(j=0; j<STATE; j++){
				if(i==j)
					printf("\033[31;1m%7.3lf\033[0m", *(*(trans+i)+j));
				else
					printf("%7.3lf", *(*(trans+i)+j));
			}
			printf("\n ");
		}

		for(i=0; i<STATE; i++)
			printf("-------");

		printf("\n\n");
	}

	return(trans);
}
