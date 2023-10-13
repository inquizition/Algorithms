CC=gcc
current_dir = $(shell pwd)
VPATH = Matrix test_folder C:\cygwin64\usr\include\CUnit
CFLAGS +=-IMatrix -lm -Wall -Itest_folder -Icunit\CUnit
LDFLAGS = -lcunit -g
SOURCES_PROGRAM = Matrix.c
SOURCES_TESTS = Matrix_test.c
OBJECTS_PROGRAM = $(SOURCES_PROGRAM:.c=.o)
OBJECTS_TESTS = $(SOURCES_TESTS:.c=.o)
EXECUTABLE_TESTS = test_matrix

.PHONY:

all: $(EXECUTABLE_TESTS) run_tests

$(EXECUTABLE_TESTS): $(OBJECTS_PROGRAM) $(OBJECTS_TESTS)
	$(CC) $(OBJECTS_PROGRAM) $(OBJECTS_TESTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

run_tests: $(EXECUTABLE_TESTS)
	 ./$(EXECUTABLE_TESTS)

read_audio:
	$(CC) read_audio.c $$(pkg-config sndfile --cflags --libs) && ./a.out

clean:
	rm -f $(EXECUTABLE_TESTS) $(OBJECTS_PROGRAM) $(OBJECTS_TESTS)
