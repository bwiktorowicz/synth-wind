#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include "synth_wind.h"

//Calculate Mixed Gaussian Distribution Model Parameters
//using Expectation Maximization Algorithm
//
//st=0 for when defining a single state for entire dataset


double **mgd_param(float *v, int N, int **st_lim, int st){

	int i, j, state_sz;
	double **tao_t, *mu_t, *sigma_t, *mix_t, **res, mu_0, sigma_0, mix_0, tmp0, tmp1, **temp_param, conv;
	FILE *fp;

	conv = .01;
	state_sz = *(*(st_lim+st)+1)-*(*(st_lim+st))+1;

	tao_t = malloc(MGD_TERM*sizeof(tao_t));
	mu_t = malloc(MGD_TERM*sizeof(double));
	sigma_t = malloc(MGD_TERM*sizeof(double));
	res = malloc(3*sizeof(res));
	mix_t = malloc(MGD_TERM*sizeof(double));
	for(i=0; i<MGD_TERM; i++)
		*(tao_t+i) = malloc(N*sizeof(double));


	//initialize mu, sigma

	temp_param = stat_param_f(v, st_lim, MGD_TERM, N, st);

	for(i=0; i<MGD_TERM; i++){
		*(mu_t+i) = *(*(temp_param+i));
		*(sigma_t+i) = *(*(temp_param+i)+1);
	}

	//initialize mixing coefficients
	
	for(i=0; i<MGD_TERM; i++)
		*(mix_t+i) = 1.0/MGD_TERM;

	//clean up

	for(i=0; i<MGD_TERM; i++)
		free(*(temp_param+i));
	free(temp_param);


	//print extra information

	if(PR_MGD)
		p_mgd_param(mu_t, sigma_t, mix_t, 0);

	//EM algorithm to find mu, sigma, mixing constant

	while(conv>=THRESHOLD){

		//Expectation Step

		for(i=0; i<MGD_TERM; i++)
			for(j=0; j<N; j++)
				*(*(tao_t+i)+j) = tao_f(mix_t, mu_t, sigma_t, v, N, i, j);
		

		//Maximization Step

		for(i=0; i<MGD_TERM; i++){

			mu_0 = *(mu_t+i);
			sigma_0 = *(sigma_t+i);
			mix_0 = *(mix_t+i);

			*(mu_t+i) = mu_f(*(tao_t+i), v, N, i);

			*(sigma_t+i) = sigma_f(*(tao_t+i), *(mu_t+i), v, N, i);

			*(mix_t+i) = mix_f(*(tao_t+i), N, i);
			
			conv = abs(mu_0-*(mu_t+i));
			tmp0 = abs(sigma_0-*(sigma_t+i));
			tmp1 = abs(mix_0-*(mix_t+i));

			if(tmp0 > conv)
				conv = tmp0;
			else if(tmp1 > conv)
				conv = tmp1;
		}
	}


	*res = mu_t;
	*(res+1) = sigma_t;
	*(res+2) = mix_t;


	//printf extra information

	if(PR_MGD)
		p_mgd_param(mu_t, sigma_t, mix_t, 2);

	for(i=0; i<MGD_TERM; i++)
		free(*(tao_t+i));
	free(tao_t);

	return(res);
}

double tao_f(double *mix, double *mu, double *sigma, float *v, int N, int i, int j){

	int k;
	double num, den, res;

	den = 0;

	num = *(mix+i)*gauss_f(*(sigma+i), *(mu+i), (float)*(v+j));

	if(isnan(num)){
		printf("\n **** ERROR: Infinite result in tao_f() ****\n");
		printf("\n **** index: %d; sigma: %.3lf; mu: %.3lf; v: %.4f ****\n\n", i, *(sigma+i), *(mu+i), *(v+j));
		exit(0);
	}

	for(k=0; k<MGD_TERM; k++)
		den+= *(mix+k)*gauss_f(*(sigma+k), *(mu+k), (float)*(v+j));
	
	res = num/den;




	return(res);

}

double mu_f(double *tao, float *v, int N, int i){

	int j;
	double num, den, res;

	num = den = 0;

	for(j=0; j<N; j++){
		num+= *(tao+j)**(v+j);
		den+= *(tao+j);
	}

	res = num/den;

	return(res);

}

double sigma_f(double *tao, double mu_t, float *v, int N, int i){

	int j;
	double num, den, res;

	num = den = 0;

	for(j=0; j<N; j++){
		num+= *(tao+j)*pow(*(v+j), 2);
		den+= *(tao+j);
	}

	res = num/den - pow(mu_t, 2);

	return(res);

}

double mix_f(double *tao, int N, int i){

	int j;
	float n_fl;
	double num, res;

	num = 0;

	n_fl = N;

	for(j=0; j<N; j++)
		num+= *(tao+j);
	
	res = num/n_fl;

	if(isnan(res)){
		printf("\n **** ERROR: Infinite result in mix_f() ****\n");
		printf("\n **** index: %d; sum tao: %.3lf ****\n\n", i, num);
		exit(0);
	}

	return(res);

}

void p_mgd_param(double *mu, double *sigma, double *mix, int stage){

	int i;

	if(!stage){
	printf("\n --------------------------------\n");
	printf(" Initialized Values\n");
	}
	else if(stage==1){
	printf("\n --------------------------------\n");
	printf(" Intermediate Values\n");
	}
	else{
	printf("\n --------------------------------\n");
	printf(" Converged Values\n");
	}

	printf(" --------------------------------\n");
	printf(" \033[31;1mTERM%8s%10s%10s\033[0m\n", "mu", "sigma", "mix");
	for(i=0; i<MGD_TERM; i++){
		printf(" %2d%10.4lf%10.4lf%10.4lf\n", i, *(mu+i), pow(*(sigma+i),.5), *(mix+i));
	}
	printf(" --------------------------------\n");
}           
