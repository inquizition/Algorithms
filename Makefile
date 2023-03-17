CC=gcc

.PHONY:

matrix_tests: 
	cd utils/ && $(MAKE) all

clean:
	cd utils/ &&  $(MAKE) clean
