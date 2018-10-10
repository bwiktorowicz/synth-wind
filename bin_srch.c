#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include "synth_wind.h"

double bin_srch(float low, float hgh, float u, double *cdf){

	int idx;
	double v_gen, test;

	v_gen = (hgh-low)/2.0;



	while(fabs(low-hgh)>10*DV){

//		printf("%f %f %f %f %f\n", v_gen, low, hgh, test, u);
		if(test>=u){
			hgh = v_gen;
			v_gen = (hgh+low)/2.0;
		}
		else if(test<u){
			low = v_gen;
			v_gen = (hgh+low)/2.0;
		}

		idx = (int)(v_gen/DV);
		test = *(cdf+idx);

	}


	return(v_gen);
}
