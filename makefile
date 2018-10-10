all: synthwind

synthwind: main.o mgd_f.o mgd_param.o gauss_f.o markov.o st_lim_f.o data_gen_f.o stat_param_f.o v_fltr.o synth_wind.o data_analyze.o bin_srch.o cdf_v.o plt_sctr.o plt_hist.o plt_bin.o plt_lin.o
	cc main.o mgd_f.o mgd_param.o gauss_f.o markov.o st_lim_f.o data_gen_f.o stat_param_f.o v_fltr.o synth_wind.o data_analyze.o bin_srch.o cdf_v.o plt_sctr.o plt_hist.o plt_bin.o plt_lin.o -g -o synthwind -lm -L /usr/lib/libplplot*

main.o: main.c
	cc -g -c main.c

mgd_f.o: mgd_f.c
	cc -g -c mgd_f.c

mgd_param.o: mgd_param.c
	cc -g -c mgd_param.c

gauss_f.o: gauss_f.c
	cc -g -c gauss_f.c

markov.o: markov.c
	cc -g -c markov.c

st_lim_f.o: st_lim_f.c
	cc -g -c st_lim_f.c

data_gen_f.o: data_gen_f.c
	cc -g -c data_gen_f.c

stat_param_f.o: stat_param_f.c
	cc -g -c stat_param_f.c

v_fltr.o: v_fltr.c
	cc -g -c v_fltr.c

synth_wind.o: synth_wind.c
	cc -g -c synth_wind.c

data_analyze.o: data_analyze.c
	cc -g -c data_analyze.c

bin_srch.o: bin_srch.c
	cc -g -c bin_srch.c

cdf_v.o: cdf_v.c
	cc -g -c cdf_v.c

plt_sctr.o: plt_sctr.c
	cc -g -c plt_sctr.c

plt_hist.o: plt_hist.c
	cc -g -c plt_hist.c

plt_bin.o: plt_bin.c
	cc -g -c plt_bin.c

plt_lin.o: plt_lin.c
	cc -g -c plt_lin.c

clean:
	rm -rf *.o synthwind
