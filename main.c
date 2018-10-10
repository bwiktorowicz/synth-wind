/*
Wind Model
Bart Wiktorowicz
*/

#include "synth_wind.h"

//usage: main option

main(int argc, char **argv){

	int i, j, k, sz, N, yr, mn, dy, hr, min, size;
	float *v, azmt, elev, **pr_trn, dir2, spd2, dir5, spd5, *a, **xy;
	double **param, **trans;
	char **buf;
	FILE *fp;

	buf = malloc(argc*sizeof(buf));

	for(i=0; i<argc; i++){
		*(buf+i) = malloc(strlen(*(argv+i))*sizeof(char));

		memcpy(*(buf+i), *(argv+i), strlen(*(argv+i)));
	}

	fp = fopen(*(buf+1), "r");
	sz = N = 0;

	while(!fscanf(fp, "%*[^\n]%*c"))
		sz++;
	fseek(fp, 0L, SEEK_SET);

	v = malloc(sz*sizeof(float));
	a = malloc(sz*sizeof(float));
	xy = malloc(2*sizeof(xy));

	if(!v){
		perror("Allocation error\n");
		exit(1);
	}

	i = 0;

	while(fscanf(fp, "%d-%d-%d %d:%d:00,%f,%f,%f,%f,%f,%f", &yr, &mn, &dy, &hr, &min, &dir2, &spd2, &dir5, &spd5, &azmt, &elev) !=EOF){

		if(spd2<=MAX_SPD && spd2!=999.0 && dir2!=999.0){

			*(v+i) = spd2;
			*(a+i) = dir2;
			i++;
		}
	}

	fclose(fp);

	v = realloc(v, i*sizeof(int));

	*xy = malloc(i*sizeof(float));
	*(xy+1) = malloc(i*sizeof(float));

	k = i;
/*
	if(N_GEN <7)
		k = SEC_YR*N_GEN/DT;

	for(j=0; j<i; j++){
		*(*(xy)+j) = j;
		*(*(xy+1)+j) = *(v+j);;
	}
*/

	*xy = a;
	*(xy+1) = v;

	plt_sctr(xy, i, "#frDirection (Deg)", "#frSpeed (m/s)", "#frWind Speed Vs. Direction");
//	plt_hist(v, i, 0, MAX_SPD, MAX_SPD, "#frSpeed (m/s)", "#frQuantity", "#frWind Speed Distribution");

//	printf("Plotting Recorded Wind Data\n\n");
//	plt_lin(xy, k, "#frTime (min)", "#frSpeed (m/s)", "#frRecorded Speed vs. Time (1 Month)");


	synth_wind(v, a, i);
	
	for(i=0; i<argc; i++)
		free(*(buf+i));
	free(buf);
	free(v);
}
