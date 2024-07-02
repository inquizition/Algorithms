#ifndef M_MATRIX_H
#define M_MATRIX_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <omp.h>

typedef struct Matrix
{
    int rows, columns;
    double **data;
    struct Matrix *next;
} Matrix;

union Result {
    struct Matrix m;
    double res;
};

struct Node {
    struct Matrixt *ptr;
    struct Node *next;
};

static Matrix *allocated_matrices = NULL;

void d_reLu_matrix(Matrix *m);
double matrix_sum(Matrix *m);
void zeros(Matrix *matrix);
void fillMatrix(Matrix *matrix, double *data);
void eye(Matrix *matrix);
void dot(Matrix m1, Matrix m2, union Result *res);
void ones(Matrix *matrix);
void matMult(Matrix m1, Matrix m2, Matrix *res);
double vectorMult(double *v1, double *v2, int length);
void flatten(Matrix *m, Matrix *flattened);
void dump_matrix(Matrix *m, double* data);
void print_matrix(Matrix m, char* header);
void print_dim(Matrix m);

void freeMatrix(Matrix *m);
void printAllocatedMatricesCount(void);
void freeAllMatrices(void);
void InitRandomMatrix(Matrix *matrix);
void transpose(Matrix **m);
void matrixAdd(Matrix matrix, Matrix a, Matrix *res);
void copyMatrix(Matrix src, Matrix *dest);

Matrix *allocateMatrix(int rows, int columns);
bool cmpMatrix(Matrix m1, Matrix m2);
void hadamard_prod(Matrix m1, Matrix m2, Matrix* res);
void matrixSubtract(Matrix matrix, Matrix a, Matrix *res);

void reLu_matrix(Matrix *m);
void sigmoid_matrix(Matrix *m);
void exp_matrix(Matrix *m);
void const_mult_matrix(Matrix *m, double c);
void matrix_pow(Matrix *m, int power);
double logsumexp(Matrix *m);

#endif // M_MATRIX_H
