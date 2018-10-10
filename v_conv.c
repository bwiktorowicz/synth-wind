#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include "synth_wind.h"

int v_conv(float v, int n, int fac){

	int i, *vc;

	vc =  malloc(n*sizeof(int));

	for(i=0; i<n; i++){

		tst = fac_tst = 1;

		while(tst){
			tst = ceil(*(v+i)*fac_tst - floor(*(v+i)*fac_tst));
			fac_tst*=10;
		}

	if(fac<f_tst && BIN_PREC >= 1/fac_tst)
		fac = f_tst
	else
		fac = BIN_PREC;
	}

	for(i=0; i<n; i++)
		*(vc+i) = ceil(fac**(v+i));

	return(vc);
}
