#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#define PI 3.141592653589793238462643383279502884197
#define MAX_SPD 45
#define MGD_TERM 3
#define STATE 8
#define STATE_RES 15 //time steps between transitions in Markov Chain
#define THRESHOLD .01
#define N_GEN 20 //.0027 //years of data to generate
#define SEC_YR 31536000 //seconds per year
#define DT 1 //time step in seconds
#define DV .00001 //velocity step
#define EPS .001 //convergence precision
#define BIN_PREC .1 //histrogram bin resolution

#define MGD_DB 0 //set to one to try out MGD on entire training data set
#define PR_MGD 1 //set to one to print mean, standard dev, mixing constant
#define PR_STATE 1 //set to one to print state or term ranges
#define PR_TRN 1 //set to one to print transition matrix
#define DA 1 //set to one in order to provide analysis of generated velocity

double **mgd_param(float *v, int N, int **st_lim, int st);
double mgd_f(double **param, float v);
double tao_f(double *mix, double *mu, double *sigma, float *v, int N, int i, int j);
double mu_f(double *tao, float *v, int N, int i);
double sigma_f(double *tao, double mu_t, float *v, int N, int i);
double mix_f(double *tao, int N, int i);
double gauss_f(double sigma, double mu, float v);
void p_mgd_param(double *mu, double *sigma, double *mix, int stage);
double **markov(int **st_lim, float *v, int N);
float *data_gen_f(int **st_lim, double ***param, double **trans, int *n_gen);
int **st_lim_f(int max_spd, int term, int flag);
void synth_wind(float *v, float *a, int N);
double **stat_param_f(float *v, int **st_lim, int term, int N, int st);
float **v_fltr(float *v, int N, int *nf, int **st_lim, int term);
int st_new(double **trans, int st0);
double *cdf_v(double **param, double v, int i);
void data_analyze(float **vf_t, int *nf_t, float *v_gen, int ngen, int **st_lim);
int *v_conv(float *v, int n, int *fac);
double bin_srch(float low, float hgh, float u, double *cdf);
int plt_sctr(float **x, int n, char *xlab, char *ylab, char *ttl);
int plt_hist(float *x, int n, float x_min, float x_max, int nbin, char *xlab, char *ylab, char *ttl);
int plt_bin(float *x, float *y, int nbin, char *xlab, char *ylab, char *ttl);
int plt_lin(float **x, int n, char *xlab, char *ylab, char *ttl);
