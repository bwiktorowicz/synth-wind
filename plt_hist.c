#include "plplot/plplot.h"
#include "plplot/plstrm.h"
#include <stdlib.h>
#ifdef PL_HAVE_UNISTD_H
# include <unistd.h>
#else
# ifdef PL_HAVE_POLL
#  include <poll.h>
# endif
#endif


int plt_hist(float *x, int n, float x_min, float x_max, int nbin, char *xlab, char *ylab, char *ttl){

	int i, pnbin;
	static PLFLT *xd, px_min, px_max;

	xd = malloc(n*sizeof(PLFLT));

	for(i=0; i<n; i++)
		*(xd+i) = *(x+i);
	
	pnbin = nbin;
	px_min = x_min;
	px_max = x_max;


	// Initialize plplot

	plinit();
	plschr(0, .5);
	plenv(0.0, px_max, 0.0, 10*n/(nbin), 0, -2);
	plwind( 0.0, px_max, 0.0, 10*n/nbin);

	plbox("bcinst", 5, 0, "bcinst", 0, 0);

	plcol0(2);
	plhist(n, xd, x_min, x_max, nbin, 3);

	//put labels on axes

	plcol0(3);
	pllab (xlab, ylab, ttl);

/*
	plcol0(3);
	plmtex("b", 3.2, .5, .5, xlab);
	plmtex("l", 5, .5, .5, ylab);
	plmtex("t", 2, .5, .5, ttl);
*/

	plend();
}

