CC=gcc
current_dir = $(shell pwd)
VPATH = ../../utils/Matrix
CFLAGS +=-I../../utils/Matrix -I. -lm -Wall
DEPS = Matrix.h NN.h
OBJ = Matrix.o NN.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

NN: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)