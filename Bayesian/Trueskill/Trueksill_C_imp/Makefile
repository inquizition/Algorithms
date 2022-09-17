CC=gcc
CFLAGS=-I. -lm
DEPS = Message.h Trueskill.h
OBJ = Message.o Trueskill.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Trueskill: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
