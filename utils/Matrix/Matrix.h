#include "math.h"
#include "stdio.h"
#include <stdbool.h>

typedef struct Matrix
{
    int rows, columns;
    double **data;
} Matrix;

union Result {
    struct Matrix m;
    double res;
};

struct Node {
    struct Matrixt *ptr;
    struct Node *next;
};

void zeros(Matrix *matrix);
void fillMatrix(Matrix *matrix, double *data);
void eye(Matrix *matrix);
void dot(Matrix m1, Matrix m2, union Result *res);
void ones(Matrix *matrix);
void matMult(Matrix m1, Matrix m2, Matrix *res);
double vectorMult(double *v1, double *v2, int length);
void print_matrix(Matrix m);
void print_dim(Matrix m);

void freeMatrix(Matrix *m);
void InitRandomMatrix(Matrix *matrix);
void transpose(Matrix **m);
void matrixAdd(Matrix matrix, Matrix a, Matrix *res);
void copyMatrix(Matrix src, Matrix *dest);

Matrix *allocateMatrix(int rows, int columns);
bool cmpMatrix(Matrix m1, Matrix m2);
void hadamard_prod(Matrix m1, Matrix m2, Matrix* res);
void matrixSubtract(Matrix matrix, Matrix a, Matrix *res);

void reLu_matrix(Matrix *m);
