#include "math.h"
#include "stdio.h"

typedef struct Matrix
{
    int rows, columns;
    double **data;
} Matrix;

typedef struct
{
    int rows, columns;
    double data[];
} Ma;

void zeros(struct Matrix *matrix);
void fillMatrix(struct Matrix *matrix, double *data);
void eye(struct Matrix *matrix);
double dot(struct Matrix m1, struct Matrix m2);
void ones(struct Matrix *matrix);
void matMult(struct Matrix m1, struct Matrix m2, struct Matrix *res);
double vectorMult(double *v1, double *v2, int length);
void print_matrix(struct Matrix m);
void freeMatrix(struct Matrix *m);
void InitRandomMatrix(struct Matrix *matrix);
void transpose(struct Matrix **m);
void matrixAdd(struct Matrix *matrix, struct Matrix a);
void copyMatrix(struct Matrix src, struct Matrix *dest);

struct Matrix *allocateMatrix(int rows, int columns);