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



// Variables for holding error return info from PLplot

static PLINT pl_errcode;
static char  errmsg[160];

//--------------------------------------------------------------------------
// main program
//--------------------------------------------------------------------------

int plt_sctr(float **x, int n, char *xlab, char *ylab, char *ttl){

	int i;
	static PLFLT *xd, *yd;
	PLINT nn;

	nn = (PLINT)n;

	xd = malloc(n*sizeof(PLFLT));
	yd = malloc(n*sizeof(PLFLT));

	for(i=0; i<n; i++){

		*(xd+i) = *(*x+i);
		*(yd+i) = *(*(x+1)+i);
	}

//    const char *legline[4];

// Axes options same as plbox.
// Only automatic tick generation and label placement allowed
// Eventually I'll make this fancier

/*
    colbox     = 1;
    collab     = 3;
    styline[0] = colline[0] = 2;        // pens color and line style
    styline[1] = colline[1] = 3;
    styline[2] = colline[2] = 4;
    styline[3] = colline[3] = 5;
*/

/*
    legline[0] = "sum";                         // pens legend
    legline[1] = "sin";
    legline[2] = "sin*noi";
    legline[3] = "sin+noi";

    xlab = 0.; ylab = 0.25;     // legend position
*/

// Initialize plplot
	plinit();
	plschr(0, .5);
	plenv(0.0, 365.0, 0.0, 60.0, 0, -2);
//	plvsta();
	plwind( 0.0, 365.0, 0.0, 60.0 );

	plbox("bcinst", 50, 5, "bcinst", 5, 5);
	plcol0(3);
	pllab (xlab, ylab, ttl);

	plcol0(2);
	plstring(n, xd, yd, ".");
//	plcol0(2);

	//put labels on axes
	
	//plcol0(2);
	//plmtex("b", 3.2, .5, .5, "Time (min)");
	//plmtex("l", 5, .5, .5, "Speed (m/s)");
	//plmtex("t", 2, .5, .5, "Wind Speed Over Time");


// Let plplot handle errors from here on

	plend();
}

