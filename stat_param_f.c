#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include "synth_wind.h"

//Generate Histrogram and Probabilities
//from Training Data

double **stat_param_f(float *v, int **st_lim, int term, int N, int st){

	int i, j, state_sz, v_sum, low_spd, max_spd, len, fac, *v_c, flag;
	float *bin, *pr, *v_num, dev_sum;
	double **res;

/**************************************************/

	//check data to determine if input is integer or real
	
	i = flag = 0;

	while(!flag && i<N){
		if(floorf(*(v+i))!=*(v+i))
			flag = 1;
		i++;
	}

	if(flag){
		v_c = v_conv(v, N, &fac);
	}
	else{
		fac = 1;

		v_c = malloc(N*sizeof(int));

		for(i=0; i<N; i++)
			*(v_c+i) = (int)*(v+i);
	}

/**************************************************/

	//initialize variables/allocate memory

	max_spd = fac**(*(st_lim+st)+1);
	low_spd = fac**(*(st_lim+st));
	len = max_spd-low_spd+1;
	state_sz = ceil(len/term);

	bin = malloc(len*sizeof(float));
	pr = malloc(len*sizeof(float));
	res = malloc(term*sizeof(res));
	v_num = malloc(term*sizeof(int));
	for(i=0; i<term; i++)
		*(res+i) = malloc(2*sizeof(double));

	for(i=0; i<term; i++)
		*(v_num+i) = 0;

	dev_sum = v_sum = 0;

	j = len;
	
/**************************************************/

	//Generate Histogram and Probability Data

	while(j--)
		*(bin+j) = 0;

	for(i=0; i<N; i++)
		(*(bin+*(v_c+i)-low_spd))++;
				

	for(i=0; i<len; i++)
		*(pr+i) = (*(bin+i))/N;

	//printf extra information
	if(PR_MGD){
		printf("\n --------------------------------\n");
		printf(" Bin Quant. and Prob. - \033[31;1mSS\033[0m: (%d)\n", state_sz);
		printf(" --------------------------------\n");
		printf(" \033[31;1m%8s%9s%10s\033[0m\n", "Spd. (m/s)", "NO.", "Prob.");
		for(i=0; i<len; i++)
			printf(" %8.3f %10.0f %9.3f\n", (i+low_spd)/(float)fac, *(bin+i), *(pr+i));
		printf(" --------------------------------\n");
	}

/**************************************************/

	//calculate mean in each state

	for(i=0; i<term-1; i++){

		for(j=i*state_sz; j<(1+i)*state_sz; j++){

			v_sum+= *(bin+j)*(j+low_spd);
			*(v_num+i)+= *(bin+j);
		}

		if(*(v_num+i)==0 && i!=0)
			*(*(res+i)) = .00001;
		else if(*(v_num+i)==0 && i==0)
			*(*(res+i)) = .00001;
		else
		*(*(res+i)) = v_sum/(*(v_num+i));

		v_sum = 0;

	}

	for(j=i*state_sz; j<len; j++){

		v_sum+= *(bin+j)*(j+low_spd);
		*(v_num+term-1)+= *(bin+j);
	}


		if(*(v_num+i)==0 && i!=0)
			//*(*(res+term-1)) = *(*(res+i-1));
			*(*(res+term-1)) = .00001;
		else if(*(v_num+i)==0 && i==0)
			*(*(res+term-1)) = .00001;
		else
			*(*(res+term-1)) = v_sum/(*(v_num+term-1));

	//calculate variance in each state

	for(i=0; i<term-1; i++){


		for(j=i*state_sz; j<(1+i)*state_sz; j++)
			dev_sum+= *(bin+j)*pow((j+low_spd)-*(*(res+i)), 2);

		if(*(v_num+i)<=1)
			*(*(res+i)+1) = .0001;
		else
			*(*(res+i)+1) = pow(dev_sum/(*(v_num+i)-1), 1);

		dev_sum = 0;
	}
		
	for(j=i*state_sz; j<len; j++)
		dev_sum+= *(bin+j)*pow((j+low_spd)-*(*(res+term-1)), 2);

	if(*(v_num+term-1)<=1)
		*(*(res+term-1)+1) = .0001;
	else
		*(*(res+term-1)+1) = pow(dev_sum/(*(v_num+term-1)-1), 1);

//*(*(res+term-1)+1) = 3.14;
//printf("%f\n", *(*(res+term-1)+1));

	free(v_num);
	free(bin);
	free(pr);

	//convert mean and variance back if required
	
	if(fac!=1){
		for(i=0; i<term; i++){
			*(*(res+i))/= (float)fac;
			*(*(res+i)+1)/= (float)pow((float)fac,2);
		}

	}

	return(res);
}

//transform velocity data from real to integer bins for use in histograms

int *v_conv(float *v, int n, int *fac){

	int i, fac_tst, *vc;
	float tst;

	vc =  malloc(n*sizeof(int));

	for(i=0; i<n; i++){

		tst = fac_tst = 1;

		while(tst){
			fac_tst*=10;
			tst = ceilf(*(v+i)*fac_tst - floorf(*(v+i)*fac_tst));
		}

	if(*fac<fac_tst && BIN_PREC <= 1/fac_tst)
		*fac = fac_tst;
	else
		*fac = (int)(1/BIN_PREC);

	}

	for(i=0; i<n; i++)
		*(vc+i) = (int)ceilf(*fac**(v+i));

	return(vc);
}
