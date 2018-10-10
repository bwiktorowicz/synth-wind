#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include "synth_wind.h"

#include "plplot/plplot.h"
#include "plplot/plstrm.h"

#ifdef PL_HAVE_UNISTD_H
#	include <unistd.h>
#else
#	ifdef PL_HAVE_POLL
#		include <poll.h>
#	endif
#endif


/*
 * Synthesized wind model
 * Bart Wiktorowicz
 */

void synth_wind(float *v, float *a, int N){

	int i, j, *state_sz, **st_lim, **mgd_lim, *nf, n_gen, **ang_lim;
	float  **vf, k, res, *v_gen, u;
	double **st_param, *data_gen, **trans, ***param, ***ang_param, **ang_cdf, *ang_gen;
	FILE *fp;
	static PLFLT *xd, *yd;

	state_sz = malloc(STATE*sizeof(state_sz));
	nf = malloc(STATE*sizeof(int));
	param = malloc(STATE*sizeof(param));
	ang_cdf = malloc(sizeof(ang_cdf));
	ang_param = malloc(sizeof(ang_param));
	xd = malloc((int)ceil(MAX_SPD/DV)*sizeof(PLFLT));
	yd = malloc((int)ceil(MAX_SPD/DV)*sizeof(PLFLT));

	//Generate state probability transition matrix

	st_lim = st_lim_f(MAX_SPD, STATE, 0);

	trans = markov(st_lim, v, N);

	
	//Represent dataset with MGD for debugging/testing
	//*****************************************//
	if(MGD_DB){
		for(i=0; i<STATE; i++)
			free(*(st_lim+i));
		free(st_lim);

		st_lim = st_lim_f(MAX_SPD, 1, 1);

		st_param = mgd_param(v, N, st_lim, 0);

		k = 0;
		i = 0;

		while(k<MAX_SPD){

			res = mgd_f(st_param, k);

			*(yd+i) = res;

			*(xd+i) = k;

		//	printf("%f %f\n", k, res);

			k+=DV;

			i++;

		}

		//Plot Results


		plinit();
		plschr(0, .5);
		plenv(0.0, MAX_SPD, 0.0, 0.2, 0, -2);
		plwind(0.0, MAX_SPD, 0.0, 0.2);

		plbox("bcinst", 0, 0, "bcinst", 0, 0);
		plcol0(3);
		pllab("#frSpeed (m/s)", "Probability", "Mixed Gaussian Distribution");

		plcol0(2);
		plline(i, xd, yd);

		plend();

		for(i=0; i<STATE; i++)
			free(*(st_param+i));

		free(st_param);
		free(*st_lim);
		free(st_lim);

		exit(0);
	}
	//****************************************//

	vf = v_fltr(v, N, nf, st_lim, STATE);

	//*(*(*(param+STATE)+VARIABLE)+MGD_TERM)
	//generate MGD parameter for each term in each state
	for(i=0; i<STATE; i++){

		if(PR_MGD){
			printf("\n --------------------------------\n");
			printf(" \033[31;1mSTATE:\033[0m %d\n", i);
			printf(" --------------------------------\n");
		}

		*(param+i) = mgd_param(*(vf+i), *(nf+i), st_lim, i);
	}

	//generate data using model parameters and transition matrix
	v_gen = data_gen_f(st_lim, param, trans, &n_gen);

	//Represent angle using MGD
	//****************************************//
/*	
	ang_lim = st_lim_f(360, 1, 1);

	*ang_param = mgd_param(a, N, ang_lim, 0);

	srand(time(NULL));

	*ang_cdf = cdf_v(*(ang_param), 360, 0);

	ang_gen = malloc(n_gen*sizeof(double));

	fp = fopen("ang_gen.dat", "w");	

	for(i-0; i<n_gen; i++){

		u = rand()/(float)RAND_MAX;

		*(ang_gen+i) = bin_srch(0, 360, u, *ang_cdf);

		fprintf(fp, "%d %f %f\n", i, *(ang_gen+i), *(a+i));

	}
*/
//Provide data analysis if required
//*******************************//


	if(DA)
		data_analyze(vf, nf, v_gen, n_gen, st_lim);


//*******************************//

	//cleanup

	for(i=0; i<STATE; i++){
		free(*(vf+i));
		free(*(trans+i));
		free(*(st_lim+i));

		for(j=0; j<STATE; j++)
			free(*(*(param+j)+i));

		free(*(param+i));
	}

	free(st_lim);
	free(v_gen);
	free(vf);
	free(trans);
	free(param);
}
