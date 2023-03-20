CC=gcc

.PHONY:

matrix_tests: 
	cd utils/ && $(MAKE) all

read_wav:
	cd utils/ && $(MAKE) read_audio

clean:
	cd utils/ &&  $(MAKE) clean
