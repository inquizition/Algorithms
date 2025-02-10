#include "stress_test.h"

#define SIZE_500_500 250000
#define SIZE_5000_5000 25000000
#define SIZE_2_4 8

double A_500_500_data[SIZE_500_500];
Matrix *A_500_500;
double B_500_500_data[SIZE_500_500];
Matrix *B_500_500;
double A_DOT_B_500_500_data[SIZE_500_500];
Matrix *A_DOT_B_500_500;

double A_5000_5000_data[SIZE_5000_5000];
Matrix *A_5000_5000;
double B_5000_5000_data[SIZE_5000_5000];
Matrix *B_5000_5000;
double A_DOT_B_5000_5000_data[SIZE_5000_5000];
Matrix *A_DOT_B_5000_5000;

static void fetch_data(double *buffer, char *file_path, int size)
{
    FILE *file;

    file = fopen(file_path, "rb");

    if (!file) {
        printf("Error: Could not open file!\n");
        return;
    }

    size_t elements_read = fread(buffer, sizeof(double), size, file);
    if (elements_read != size) {
        printf("Error: Expected to read %d elements but got %zu\n", size, elements_read);
    }

    fclose(file);

}

int setup_stress_test_data() {
  fetch_data(A_500_500_data,"../Math/Tests/Test_Data/A_500_500.bin", SIZE_500_500);
  A_500_500 = allocateMatrix(500, 500);
  fillMatrix(A_500_500, A_500_500_data);
  printf("Fetched A_500_500\n");
 
  fetch_data(B_500_500_data,"../Math/Tests/Test_Data/B_500_500.bin", SIZE_500_500);
  B_500_500 = allocateMatrix(500, 500);
  fillMatrix(B_500_500, B_500_500_data);
  printf("Fetched B_500_500\n");
  
  fetch_data(A_DOT_B_500_500_data,"../Math/Tests/Test_Data/A_DOT_B_500_500.bin", SIZE_500_500);
  A_DOT_B_500_500 = allocateMatrix(500, 500);
  fillMatrix(A_DOT_B_500_500, A_DOT_B_500_500_data);
  printf("Fetched A_DOT_B_500_500\n");

  fetch_data(A_5000_5000_data,"../Math/Tests/Test_Data/A_5000_5000.bin", SIZE_5000_5000);
  A_5000_5000 = allocateMatrix(5000, 5000);
  fillMatrix(A_5000_5000, A_5000_5000_data);
  printf("Fetched A_5000_5000\n");
 
  fetch_data(B_5000_5000_data,"../Math/Tests/Test_Data/B_5000_5000.bin", SIZE_5000_5000);
  B_5000_5000 = allocateMatrix(5000, 5000);
  fillMatrix(B_5000_5000, B_5000_5000_data);
  printf("Fetched B_5000_5000\n");
  
  fetch_data(A_DOT_B_5000_5000_data,"../Math/Tests/Test_Data/A_DOT_B_5000_5000.bin", SIZE_5000_5000);
  A_DOT_B_5000_5000 = allocateMatrix(5000, 5000);
  fillMatrix(A_DOT_B_5000_5000, A_DOT_B_5000_5000_data);
  printf("Fetched A_DOT_B_5000_5000\n");

  #ifdef _OPENMP
  printf("OpenMP is enabled\n");
  #else
  printf("OpenMP is NOT enabled\n");
  #endif

  return 0;
}

int teardown_stress_test_data() {
  freeMatrix(A_500_500);
  freeMatrix(B_500_500);
  freeMatrix(A_DOT_B_500_500);

  freeMatrix(A_5000_5000);
  freeMatrix(B_5000_5000);
  freeMatrix(A_DOT_B_5000_5000);
  return 0;
}

void stress_test_dot(void) {
    struct timespec start, end;
    int iterations = 2;  // Reduce to a reasonable number

    Matrix *res = allocateMatrix(500, 500);
    clock_gettime(CLOCK_MONOTONIC, &start);  // Start time

    for (int i = 0; i < iterations; i++) {
        dot(*A_500_500, *B_500_500, (union Result *)res);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);  // End time
    freeMatrix(res);

    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("\nExecution time for %d iterations: %f seconds\n", iterations, elapsed);
}
