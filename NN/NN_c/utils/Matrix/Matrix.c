#include "Matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

static void swap(double* arg1, double* arg2);
static double **allocate2Darray(int rows, int columns);
static void free2Darray(double **arr, int rows, int columns);

static struct Node list_of_allocated_matrices = {NULL, NULL};


void add_to_list(struct Matrix ptr)
{
    struct Node *new_node = malloc(sizeof(struct Node));
    new_node->ptr = *ptr;
    new_node->next = NULL;

    struct Node *curr = &list_of_allocated_matrices;
    while(curr->next != NULL)
    {
        curr = curr->next;
    } 
    curr->next = new_node;
}

void remove_all_allocated( void )
{
    struct Node *curr = &list_of_allocated_matrices;
    while(curr != NULL)
    {
        freeMatrix((struct Matrix*)curr->ptr);
        free(curr->ptr);
        curr = curr->next;
    }
}

void print_matrix(struct Matrix m)
{
    int M = m.columns;
    int N = m.rows;
    int r;
    int c;

    printf("\n");
    for(r = 0; r < N; r++)
    {
        for(c = 0; c < M; c++)
        {
            printf("%.4f   ", m.data[r][c]);
        }
        printf("\n\n");
    }
    printf("\n");
    
}

struct Matrix *allocateMatrix(int rows, int columns)
{
    struct Matrix *m_ptr;
    m_ptr = (struct Matrix*) malloc(sizeof(struct Matrix));


    m_ptr->rows = rows;
    m_ptr->columns = columns;

    m_ptr->data = allocate2Darray(rows, columns);

    int r;
    int c;
    for(r = 0; r < m_ptr->rows; r++)
    {
        for(c = 0; c < m_ptr->columns; c++)
        {
            m_ptr->data[r][c] = 0.00;
        }
    }

    return m_ptr;
}

static double **allocate2Darray(int rows, int columns)
{
    double **arr_ptr;
    arr_ptr = (double**) malloc(rows * sizeof(*arr_ptr));

    int r;
    for(r = 0; r < rows; r++)
    {
        arr_ptr[r] = (double*)malloc(columns*sizeof(double));
    }

    return arr_ptr;
}

static void free2Darray(double **arr, int rows, int columns)
{
    int i;
    for(i = 0; i < rows; i++)
    {
        free(arr[i]);
    }
    free(arr);
}

void freeMatrix(struct Matrix *m)
{
    free2Darray(m->data, m->rows, m->columns);
    free(m);
}

static void swap(double* arg1, double* arg2)
{
    double buffer = *arg1;
    *arg1 = *arg2;
    *arg2 = buffer;
}

void matrixAdd(struct Matrix matrix, struct Matrix a, struct Matrix *res)
{
    bool single_element = (a.columns == 1 && a.rows == 1);
    bool equal_rows = (a.rows == matrix.rows);
    bool equal_columns = (a.columns == matrix.columns);
    
    int r;
    int c;
    for(r = 0; r < matrix.rows; r++)
    {
        for(c = 0; c < matrix.columns; c++)
        {
            if(single_element)
            {
              res->data[r][c] = matrix.data[r][c] + a.data[0][0];  
            }
            else if(equal_rows && equal_columns)
            {
                res->data[r][c] = matrix.data[r][c] + a.data[r][c];
            }
            else if(equal_rows)
            {
                res->data[r][c] = matrix.data[r][c] + a.data[r][0];
            }
            else if(equal_columns)
            {
                res->data[r][c] = matrix.data[r][c] + a.data[0][c];
            }
        }
    }
}

void matrixSubtract(struct Matrix matrix, struct Matrix a, struct Matrix *res)
{
    bool single_element = (a.columns == 1 && a.rows == 1);
    bool equal_rows = (a.rows == matrix.rows);
    bool equal_columns = (a.columns == matrix.columns);
    
    int r;
    int c;
    for(r = 0; r < matrix.rows; r++)
    {
        for(c = 0; c < matrix.columns; c++)
        {
            if(single_element)
            {
                res->data[r][c] = matrix.data[r][c] - a.data[0][0];  
            }
            else if(equal_rows && equal_columns)
            {
                res->data[r][c] = matrix.data[r][c] - a.data[r][c];
            }
            else if(equal_rows)
            {
                res->data[r][c] = matrix.data[r][c] - a.data[r][0];
            }
            else if(equal_columns)
            {
                res->data[r][c] = matrix.data[r][c] - a.data[0][c];
            }
        }
    }
}

void transpose(struct Matrix **m)
{
    struct Matrix *temp = *m;
    int M = temp->columns;
    int N = temp->rows;
    int r;
    int c;

    struct Matrix *transposed_matrix = allocateMatrix(M, N);

    for( r = 0; r < N; r++ )
    {
        for ( c = 0; c < M; c++ ) // only the upper is iterated
        {
            swap(&(temp->data[r][c]), &(transposed_matrix->data[c][r]));
        }
    }

    *m = transposed_matrix;
    freeMatrix(temp);
}

void zeros(struct Matrix *matrix)
{
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

void fillMatrix(struct Matrix *matrix, double *data)
{
    int r;
    int c;
    for(r = 0; r < matrix->rows; r++)
    {
        for(c = 0; c < matrix->columns; c++)
        {
            matrix->data[r][c] = data[c + (matrix->columns*r)];
        }
    }
}

void InitRandomMatrix(struct Matrix *matrix)
{
    //allocMatrix(matrix);
    int r;
    int c;
    for(r = 0; r < matrix->rows; r++)
    {
        for(c = 0; c < matrix->columns; c++)
        {
            matrix->data[r][c] = (((double)rand() - RAND_MAX/2) / RAND_MAX/2);
        }
    }
}

void eye(struct Matrix *matrix)
{

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

bool cmpMatrix(struct Matrix m1, struct Matrix m2)
{
    int r;
    int c;
    bool equal = true;
    for(r = 0; r < m1.rows; r++) 
    {
        for(c = 0; c < m1.columns; c++)
        {
            if(m1.data[r][c] != m2.data[r][c])
            {
                equal = false;
            }

        }
    }

    return equal;
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

void dot(struct Matrix m1, struct Matrix m2, union Result *res)
{
    assert(m1.columns == m2.rows);

    int rows = m1.rows;
    int cols = m2.columns;
    int middle = m1.columns;
    
    int r;
    int c;
    int i;

    if( (rows == 1) && (cols == 1) )
    {
        for(c = 0; c < middle; c++)
        {
            res->res += m1.data[0][c] * m2.data[c][0];
        }
    }
    else
    {
        assert(res->m.rows == rows);
        assert(res->m.columns == cols);

        for(r = 0; r < res->m.rows; r++)
        {
            for(c = 0; c < res->m.columns; c++)
            {
                for(i = 0; i < middle; i++)
                {
                    res->m.data[r][c] += m1.data[r][i] * m2.data[i][c];
                }
            }
        }

    }
}

double vectorMult(double *v1, double *v2, int length)
{
    double res;
    int i;
    for(i = 0; i < length; i++)
    {
        res += v1[i] * v2[i];
    }

    return res;
}

void copyMatrix(struct Matrix src, struct Matrix *dest)
{
    dest->columns = src.columns;
    dest->rows = src.rows;
    int r;
    int c;
    for(r = 0; r < src.rows; r++)
    {
        for(c = 0; c < src.columns; c++)
        {
            dest->data[r][c] = src.data[r][c];
        }
    }
}

void matMult(struct Matrix m1, struct Matrix m2, struct Matrix *res)
{
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

void hadamard_prod(struct Matrix m1, struct Matrix m2, struct Matrix* res)
{
    res->rows = m1.rows;
    res->columns = m1.columns;

    assert(m1.columns == m2.columns);
    assert(m1.rows == m2.rows);

    int r;
    int c;

    for(r = 0; r < m1.rows; r++)
    {
        for(c = 0; c < m1.columns; c++)
        {
            res->data[r][c] += m1.data[r][c] * m2.data[r][c];
        }
    }
}

// int main()
// {

//     float data[5][5] = {    {0.1, 0.2, 0.3, 0.4, 0.5},
//                             {0.1, 0.2, 0.3, 0.4, 0.5},
//                             {0.1, 0.2, 0.3, 0.4, 0.5},
//                             {0.1, 0.2, 0.3, 0.4, 0.5},
//                             {0.1, 0.2, 0.3, 0.4, 0.5},};

//     struct Matrix matrix = {.rows = 5, .columns = 5};
//     zeros(&matrix);

//     print_matrix(matrix);

//     fillMatrix(&matrix, *data);

//     print_matrix(matrix);

//     transpose(&matrix);

//     print_matrix(matrix);

//     eye(&matrix);

//     print_matrix(matrix);

//     struct Matrix matrix2 = {.rows = 5, .columns = 5};
//     zeros(&matrix2);
//     ones(&matrix);
//     ones(&matrix2);

//     printf("dot: %.4f\n", dot(matrix, matrix2));

//     struct Matrix matmultrix;
//     matMult(matrix, matrix2, &matmultrix);

//     print_matrix(matmultrix);

//     struct Matrix matrix3 = {.rows = 5, .columns = 1};
//     zeros(&matrix3);
//     ones(&matrix3);

//     struct Matrix matmultrix2;
//     matMult(matrix, matrix3, &matmultrix2);

//     print_matrix(matmultrix2);

//     return 1;
// }