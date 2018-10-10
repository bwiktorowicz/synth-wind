#include "synth_wind.h"

double gauss_f(double sigma, double mu, float v){

	double res, std_dev;
	float vf;

	vf = v;

	std_dev = pow(sigma, .5);

	res = (1.0/(std_dev*pow(2*PI, .5)))*exp(-1*pow(vf-mu,2)/(2*pow(std_dev,2)));

	if(isnan(res)){
		printf("\n**** ERROR: Infinite result in gauss_f() ****\n");
		printf("\n**** sigma: %.3lf; mu: %.3lf; v: %.3f ****\n\n", sigma, mu, vf);
		exit(0);
	}

	return(res);
}
