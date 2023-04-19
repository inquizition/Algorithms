CC=gcc
CFLAGS += -Iutils\Matrix -IBayesian\VAE\Models -lm -Wall
LDFLAGS = -g
SOURCES = utils\Matrix\Matrix.c Bayesian\VAE\Models\LinearModel.c main.c 
OBJECTS = $(SOURCES:.c=.o)
.PHONY:

main: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

matrix_tests: 
	cd utils/ && $(MAKE) all

clean:
	cd utils/ &&  $(MAKE) clean
