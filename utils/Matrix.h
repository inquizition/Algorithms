#include "math.h"
#include "stdio.h"

typedef struct Matrix
{
    int rows, columns;
    double **data;
} Matrix;

void zeros(struct Matrix *matrix);
void fillMatrix(struct Matrix *matrix, float *data);
void eye(struct Matrix *matrix);
float dot(struct Matrix m1, struct Matrix m2);
void ones(struct Matrix *matrix);
void matMult(struct Matrix m1, struct Matrix m2, struct Matrix *res);
float vectorMult(double *v1, float *v2, int length);
void print_matrix(struct Matrix m);
void freeMatrix(struct Matrix *m);
void InitRandomMatrix(struct Matrix *matrix);
void transpose(struct Matrix *m);