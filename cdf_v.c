#include "synth_wind.h"

double *cdf_v(double **param, double v, int i){

	int j , n, prog;
	double vn, *res;

	n = ceil(v/DV);
	res = malloc(n*sizeof(double));

	vn = 0;
	*res = DV*mgd_f(param, vn);

//	printf(" Generating CDF for State \033[31;1m%d\033[0m\n", i);
//	fflush(stdout);

	for(j=1; j<=n; j++){

		//print progress
		prog = ceil(100*j/n);
//		printf("\r \033[31;1m%d\033[0m", prog);
//		fflush(stdout);

		vn+=DV;
		*(res+j) = *(res+j-1) +  DV*mgd_f(param, vn);
//		printf("%f %f\n", vn, res);
	}

//	printf("\n\n");

	return(res);

}
