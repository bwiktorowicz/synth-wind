#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include "synth_wind.h"


double mgd_f(double **param, float v){

	int i, j, N;
	double res;

	res = 0;

		for(j=0; j<MGD_TERM; j++){
			res+= *(*(param+2)+j)*gauss_f(*(*(param+1)+j), *(*(param)+j), v);
		}


	
	return(res);

}
