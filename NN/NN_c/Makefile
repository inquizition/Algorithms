CC=gcc
current_dir = $(shell pwd)
VPATH = utils/Matrix src test_folder C:\cygwin64\usr\include\CUnit
CFLAGS +=-Iutils/Matrix -Isrc -lm -Wall -Itest_folder
LDFLAGS = -lcunit -g
SOURCES_PROGRAM = Matrix.c
SOURCES_TESTS = Matrix_test.c
OBJECTS_PROGRAM = $(SOURCES_PROGRAM:.c=.o)
OBJECTS_TESTS = $(SOURCES_TESTS:.c=.o)
EXECUTABLE_PROGRAM = NN
EXECUTABLE_TESTS = tests

.PHONY:

$(EXECUTABLE_PROGRAM): $(OBJECTS_PROGRAM)
	$(CC) $(LDFLAGS) $(OBJECTS_PROGRAM) -o $@


$(EXECUTABLE_TESTS): $(OBJECTS_PROGRAM) $(OBJECTS_TESTS)
	$(CC) $(OBJECTS_PROGRAM) $(OBJECTS_TESTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLE_PROGRAM) $(EXECUTABLE_TESTS) $(OBJECTS_PROGRAM) $(OBJECTS_TESTS)