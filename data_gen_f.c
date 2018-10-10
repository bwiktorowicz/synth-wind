#include "synth_wind.h"

float *data_gen_f(int **st_lim, double ***param, double **trans, int *n_gen){

	int i, j, *state_sz, st0, st1, st_sz, n, flag, idx;
	long int t;
	float u, *v_gen, *mode, vn, **xy;
	double **st_mgd, *nrm, v, test, test0, **st_cdf, low, hgh;
	FILE *fp;

	//waste some memory
	state_sz = malloc(STATE*sizeof(int));
	nrm = malloc(STATE*sizeof(double));
	st_mgd = malloc(STATE*sizeof(st_mgd));
	mode = malloc(STATE*sizeof(float));
	st_cdf = malloc(STATE*sizeof(st_cdf));
//	xy = malloc(2*sizeof(xy));

	n = ceil(MAX_SPD/DV);

	for(i=0; i<STATE; i++){
		*(state_sz+i) = ceil(*(*(st_lim+i)+1)-*(*(st_lim+i)));
		*(st_mgd+i) = malloc(*(state_sz+i)*sizeof(double));
		*(nrm+i) = 0;
	}

	nrm = malloc(STATE*sizeof(double));

	fp = fopen("v_gen.dat", "w");

	t = SEC_YR*N_GEN/DT;

	v_gen = malloc(t*sizeof(float));

//	*xy = malloc(t*sizeof(float));
//	*(xy+1) = malloc(t*sizeof(float));

	if(v_gen==NULL){
		printf("Out of memory\n");
		exit(-1);
	}


	//Find mode for MGD in each state

	for(i=0; i<STATE; i++){

		*(mode+i)  = test = test0 = 0;

		st_sz = *(*(st_lim+i)+1)-*(*(st_lim+i));


		for(j=0; j<=n; j++){
			test = (float)mgd_f(*(param+i), j*DV);
			if(test >= test0){
				*(mode+i) = j*DV;
				test0 = test;
			}
		}
	}

	//Calculate Cumulative Density Function for each state

	for(i=0; i<STATE; i++){
		*(st_cdf+i) = cdf_v(*(param+i), MAX_SPD, i);

		//Normalize CDFs that do not converge to 1.0 to avoid erroneous speeds in lower states
		if(*(*(st_cdf+i)+(int)ceil(MAX_SPD/DV))<1.0)
			for(j=0; j<MAX_SPD/DV; j++)
				*(*(st_cdf+i)+j) /= *(*(st_cdf+i)+(int)ceil(MAX_SPD/DV));

	}

	//Set Initial State
	st0 = 0;

	//Generate Wind Speed Data

	srand(time(NULL));

	for(i=0; i<t; i++){

		//Select new state every STATE_RES time steps
		if(!(i% STATE_RES)){
			st1 = st_new(trans, st0);
//			printf("%d %d\n", i, st1);
			fflush(stdout);
		}

		st0 = st1;

		flag = 0;

//		*(*xy+i) = i;

		u = rand()/(float)RAND_MAX;

		*(v_gen+i) = *(mode+st1);
		idx = (int)*(v_gen+i)/DV;
		test = *(*(st_cdf+st1)+idx);
//		printf("%d %d %d %.3f %.3f \n", i, i%STATE_RES, st1, v_gen, test);


		//Binary Search Random Variate Generation
	
		low = 0.0;
		hgh = (double)MAX_SPD;

		while(fabs(low-hgh)>DV){

		//	printf("%d %f %f %f %f %f\n", st1, *(v_gen+i), low, hgh, test, u);
			if(test>=u){
				hgh = *(v_gen+i);
				*(v_gen+i) = (hgh+low)/2.0;
			}
			else if(test<u){
				low = *(v_gen+i);
				*(v_gen+i) = (hgh+low)/2.0;
			}

			idx = (int)(*(v_gen+i)/DV);
			test = *(*(st_cdf+st1)+idx);


		//	printf("%d %f %f %f %f %f\n", st1, *(v_gen+i), low, hgh, test, u);
		}

		if(*(v_gen+i)>MAX_SPD-5 && st1==0){
			i--;
			printf("%d %f %f %f %f %f\n", st1, *(v_gen+i), low, hgh, test, u);
			exit(0);
		}
		else
			fprintf(fp, "%d %lf\n", i, *(v_gen+i));

//		*(*(xy+1)+i) = *(v_gen+i);


	}

	*n_gen = t;


	printf("Plotting Synthesized Wind Data vs. Time\n\n");
//	plt_lin(xy, t, "#frTime (min)", "#frSpeed (m/s)", "#frSynthesized Speed vs Time (1 Month)");

	fclose(fp);
	for(i=0; i<STATE; i++){
		free(*(st_mgd+i));
		free(*(st_cdf+i));
	}

	free(state_sz);
	free(nrm);
	free(st_mgd);
	free(st_cdf);
	free(mode);

	return(v_gen);
}


//Determine new state based on STPM and current state

int st_new(double **trans,  int st0){

	int i, res;
	float u;
	double st_chk;

	st_chk = 0;
//	sleep(.5);
//	srand(time(NULL));
	u = rand()/(float)RAND_MAX;

//	printf("%f\n", u);

	for(i=0; i<STATE; i++){
		st_chk+=*(*(trans+st0)+i);

		if(!i){
			if(u <= st_chk){
				res = i;
	//			printf("\033[31;1m%d %d %f\033[0m\n", i, res, st_chk);
				}
		}
		else if(i){
			if(u <= st_chk && u > st_chk-*(*(trans+st0)+i)){
				res = i;
	//			printf("\033[31;1m%d %d %f\033[0m\n", i, res, st_chk);
				}
		}	
		
	}
//printf("%d\n", res);
	return(res);
}

