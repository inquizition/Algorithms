CC=gcc
CFLAGS=-I. -lm
DEPS = Trueskill.h Message.h
OBJ = Trueskill.o Message.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Trueskill: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
