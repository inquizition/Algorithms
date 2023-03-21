CC=gcc

.PHONY:

matrix_tests: 
	cd utils/ && $(MAKE) all

read_wav:
	cd utils/ && $(MAKE) read_audio

plot_lib:
	cd utils/ && $(MAKE) plot_lib 

clean:
	cd utils/ &&  $(MAKE) clean
