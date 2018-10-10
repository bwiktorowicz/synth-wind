#include "synth_wind.h"

void data_analyze(float **vf_t, int *nf_t, float *v_gen, int n_gen, int **st_lim){

	//*(*(*(param+STATE)+VARIABLE)+MGD_TERM)

	int i, *nf_gen;
	float **vf_gen;
	double std_dev, mean, ***param_trn, ***param_gen;

	nf_gen = malloc(STATE*sizeof(int));
	param_trn = malloc(STATE*sizeof(param_trn));
	param_gen = malloc(STATE*sizeof(param_gen));

	vf_gen = v_fltr(v_gen, n_gen, nf_gen, st_lim, STATE);



	for(i=0; i<STATE; i++){

		if(PR_MGD)
			printf("\n Training Data Distribution - State: %d\n", i);

		*(param_trn+i) = stat_param_f(*(vf_t+i), st_lim, 1, *(nf_t+i), i);

		if(PR_MGD)
			printf("\n Generated Data Distribution - State: %d\n", i);

		*(param_gen+i) = stat_param_f(*(vf_gen+i), st_lim, 1, *(nf_gen+i), i);
	}

	printf("\n --------------------------------\n");
	printf(" Training Data Parameters\n");
	printf(" --------------------------------\n");
	printf("\033[31;1m%6s%12s%12s\033[0m\n", " State", " Std Dev", " Mean");

	for(i=0; i<STATE; i++){

		std_dev = pow(*(*(*(param_trn+i))+1), .5);
		mean = *(*(*(param_trn+i)));
	
		printf("%4d%14.4lf%14.4lf\n", i, std_dev, mean);
	}

	printf(" --------------------------------\n");

	printf("\n --------------------------------\n");
	printf(" Synthetic Data Parameters\n");
	printf(" --------------------------------\n");
	printf("\033[31;1m%6s%12s%12s\033[0m\n", " State", " Std Dev", " Mean");

	for(i=0; i<STATE; i++){

		std_dev = pow(*(*(*(param_gen+i))+1), .5);
		mean = *(*(*(param_gen+i)));

		printf("%4d%14.4lf%14.4lf\n", i, std_dev, mean);
	}

	printf(" --------------------------------\n");
}
