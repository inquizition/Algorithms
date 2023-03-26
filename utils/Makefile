CC=gcc
current_dir = $(shell pwd)
VPATH = Matrix test_folder C:\cygwin64\usr\include\CUnit
CFLAGS +=-IMatrix -lm -Wall -Itest_folder
LDFLAGS = -lcunit -g 
SNDFLAGS = $$(pkg-config sndfile --cflags --libs)
SOURCES_PROGRAM = Matrix.c plot_lib.c read_audio.c
SOURCES_TESTS = Matrix_test.c
OBJ_DIR = build
OBJECTS_PROGRAM = $(SOURCES_PROGRAM:%.c=$(OBJ_DIR)/%.o)
OBJECTS_TESTS = $(SOURCES_TESTS:%.c=$(OBJ_DIR)/%.o)
EXECUTABLE_TESTS = build/test_matrix

.PHONY:

all: $(EXECUTABLE_TESTS) run_tests

matrix_tests: check_dir $(EXECUTABLE_TESTS) run_tests

$(EXECUTABLE_TESTS): $(OBJECTS_PROGRAM) $(OBJECTS_TESTS)
	$(CC) $(OBJECTS_PROGRAM) $(OBJECTS_TESTS) -o $@ $(LDFLAGS) $(SNDFLAGS)

build/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

run_tests: $(EXECUTABLE_TESTS)
	 ./$(EXECUTABLE_TESTS)

read_audio:
	$(CC) read_audio.c plot_lib.c $$(pkg-config sndfile --cflags --libs) && ./a.out

plot_lib:
	$(CC) plot_lib.c && ./a.out

fft:
	$(CC) FFT.c plot_lib.c $$(pkg-config sndfile --cflags --libs) -lm -fno-stack-protector && ./a.out
clean:
	rm -f $(EXECUTABLE_TESTS) $(OBJECTS_PROGRAM) $(OBJECTS_TESTS)

check_dir:
	@[ -d "build" ] || { mkdir build; }
