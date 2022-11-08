#include "Matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static void allocMatrix(struct Matrix *m);
static void swap(double* arg1, double* arg2);

void print_matrix(struct Matrix m)
{
    int M = m.columns;
    int N = m.rows;
    int r;
    int c;
    for(r = 0; r < N; r++)
    {
        for(c = 0; c < M; c++)
        {
            printf("%.2f   ", m.data[r][c]);
        }
        printf("\n\n");
    }
    printf("\n");
    
}

static void allocMatrix(struct Matrix *m)
{
    m->data = (double**) malloc(sizeof(double*) * m->rows);
    int i;
    for(i = 0; i < m->rows; i++)
    {
        m->data[i] = (double *)malloc(sizeof(double)*m->columns);
    }
}

void freeMatrix(struct Matrix *m)
{
    int i;
    for (i = 0; i < m->rows; i++)
        free(m->data[i]);
 
    free(m->data);
}

static void swap(double* arg1, double* arg2)
{
    double buffer = *arg1;
    *arg1 = *arg2;
    *arg2 = buffer;
}

void matrixAdd(struct Matrix *matrix, double a)
{
    int r;
    int c;
    for(r = 0; r < matrix->rows; r++)
    {
        for(c = 0; c < matrix->columns; c++)
        {
            matrix->data[r][c] += a;
        }
    }
}

void transpose(struct Matrix *m)
{
    int r;
    int c;
    for( r = 0; r < m->rows; r++ )
    {
        for ( c = r+1; c < m->columns; c++ ) // only the upper is iterated
        {
            swap(&(m->data[r][c]), &(m->data[c][r]));
        }
    }
}

void zeros(struct Matrix *matrix)
{
    allocMatrix(matrix);
    int r;
    int c;
    for(r = 0; r < matrix->rows; r++)
    {
        for(c = 0; c < matrix->columns; c++)
        {
            matrix->data[r][c] = 0.00;
        }
    }
}

void fillMatrix(struct Matrix *matrix, float *data)
{
    int r;
    int c;
    for(r = 0; r < matrix->rows; r++)
    {
        for(c = 0; c < matrix->columns; c++)
        {
            matrix->data[r][c] = data[c + (matrix->rows*r)];
        }
    }
}

void InitRandomMatrix(struct Matrix *matrix)
{
    allocMatrix(matrix);
    int r;
    int c;
    for(r = 0; r < matrix->rows; r++)
    {
        for(c = 0; c < matrix->columns; c++)
        {
            matrix->data[r][c] = randn();
        }
    }
}

void eye(struct Matrix *matrix)
{
    assert(matrix->columns == matrix->rows);

    int r;
    int c;
    for(r = 0; r < matrix->rows; r++)
    {
        for(c = 0; c < matrix->columns; c++)
        {
            if(c == r)
            {
                matrix->data[r][c] = 1.0;
            }
            else
            {
                matrix->data[r][c] = 0.0;
            }
        }
    }

}

void ones(struct Matrix *matrix)
{
    int r;
    int c;
    for(r = 0; r < matrix->rows; r++)
    {
        for(c = 0; c < matrix->columns; c++)
        {
            matrix->data[r][c] = 1.0;
        }
    }
}

float dot(struct Matrix m1, struct Matrix m2)
{
    assert(m1.columns == m2.columns);
    assert(m1.rows == m2.rows);

    float res;
    int r;
    int c;
    for(r = 0; r < m1.rows; r++)
    {
        for(c = 0; c < m1.columns; c++)
        {
            res += m1.data[r][c] * m2.data[r][c];
        }
    }

    return res;
}

float vectorMult(double *v1, float *v2, int length)
{
    float res;
    int i;
    for(i = 0; i < length; i++)
    {
        res += v1[i] * v2[i];
    }

    return res;
}

void matMult(struct Matrix m1, struct Matrix m2, struct Matrix *res)
{
    assert(m1.columns == m2.rows);
    res->rows = m1.rows;
    res->columns = m2.columns;
    zeros(res);

    int r;
    int c;
    int i;

    for(r = 0; r < m1.rows; r++)
    {
        for(c = 0; c < m2.columns; c++)
        {
            for(i = 0; i < m1.columns; i++)
            {
                res->data[r][c] += m1.data[r][i] * m2.data[i][c];
            }
        }
    }
}

int main()
{

    float data[5][5] = {    {0.1, 0.2, 0.3, 0.4, 0.5},
                            {0.1, 0.2, 0.3, 0.4, 0.5},
                            {0.1, 0.2, 0.3, 0.4, 0.5},
                            {0.1, 0.2, 0.3, 0.4, 0.5},
                            {0.1, 0.2, 0.3, 0.4, 0.5},};

    struct Matrix matrix = {.rows = 5, .columns = 5};
    zeros(&matrix);

    print_matrix(matrix);

    fillMatrix(&matrix, *data);

    print_matrix(matrix);

    transpose(&matrix);

    print_matrix(matrix);

    eye(&matrix);

    print_matrix(matrix);

    struct Matrix matrix2 = {.rows = 5, .columns = 5};
    zeros(&matrix2);
    ones(&matrix);
    ones(&matrix2);

    printf("dot: %.4f\n", dot(matrix, matrix2));

    struct Matrix matmultrix;
    matMult(matrix, matrix2, &matmultrix);

    print_matrix(matmultrix);

    struct Matrix matrix3 = {.rows = 5, .columns = 1};
    zeros(&matrix3);
    ones(&matrix3);

    struct Matrix matmultrix2;
    matMult(matrix, matrix3, &matmultrix2);

    print_matrix(matmultrix2);

    return 1;
}