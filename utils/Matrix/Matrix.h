#include "math.h"
#include "stdio.h"
#include <stdbool.h>

typedef struct Matrix
{
    int rows, columns;
    double **data;
} Matrix;

typedef struct Matrix_2
{
    int rows, columns;
    double *data;
} Matrix_2;

union Result {
    struct Matrix m;
    double res;
};

struct Node {
    struct Matrixt *ptr;
    struct Node *next;
};

void zeros(struct Matrix *matrix);
void fillMatrix(struct Matrix *matrix, double *data);
void eye(struct Matrix *matrix);
void dot(struct Matrix m1, struct Matrix m2, union Result *res);
void ones(struct Matrix *matrix);
void matMult(struct Matrix m1, struct Matrix m2, struct Matrix *res);
double vectorMult(double *v1, double *v2, int length);
void print_matrix(struct Matrix m);

void freeMatrix(struct Matrix *m);
void InitRandomMatrix(struct Matrix *matrix);
void transpose(struct Matrix **m);
void matrixAdd(struct Matrix matrix, struct Matrix a, struct Matrix *res);
void copyMatrix(struct Matrix src, struct Matrix *dest);

struct Matrix *allocateMatrix(int rows, int columns);
bool cmpMatrix(struct Matrix m1, struct Matrix m2);
void hadamard_prod(struct Matrix m1, struct Matrix m2, struct Matrix* res);
void matrixSubtract(struct Matrix matrix, struct Matrix a, struct Matrix *res);
