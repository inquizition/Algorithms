#include "Matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

static void swap(double* arg1, double* arg2);
static double **allocate2Darray(int rows, int columns);
static void free2Darray(double **arr, int rows, int columns);

// static Node list_of_allocated_matrices = {NULL, NULL};


// void add_to_list(Matrix ptr)
// {
//     Node *new_node = malloc(sizeof(Node));
//     new_node->ptr = *ptr;
//     new_node->next = NULL;
// 
//     Node *curr = &list_of_allocated_matrices;
//     while(curr->next != NULL)
//     {
//         curr = curr->next;
//     } 
//     curr->next = new_node;
// }
// 
// void remove_all_allocated( void )
// {
//     Node *curr = &list_of_allocated_matrices;
//     while(curr != NULL)
//     {
//         freeMatrix((Matrix*)curr->ptr);
//         free(curr->ptr);
//         curr = curr->next;
//     }
// }

void reLu_matrix(Matrix *m)
{
    int r;
    int c;
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
	    if(m->data[r][c] < 0)
	    {
            	m->data[r][c] = 0;
	    }
        }
    }
}

void print_dim(Matrix m)
{
	printf("Dim = (r:%d, c:%d)\n", m.rows, m.columns);
}

void print_matrix(Matrix m)
{
	printf("hellu: r %d c %d", m.rows, m.columns);
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

Matrix *allocateMatrix(int rows, int columns)
{
    Matrix *m_ptr;
    m_ptr = (Matrix*) malloc(sizeof(Matrix));


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

void freeMatrix(Matrix *m)
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

void matrixAdd(Matrix matrix, Matrix a, Matrix *res)
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

void matrixSubtract(Matrix matrix, Matrix a, Matrix *res)
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

void transpose(Matrix **m)
{
    Matrix *temp = *m;
    int M = temp->columns;
    int N = temp->rows;
    int r;
    int c;

    Matrix *transposed_matrix = allocateMatrix(M, N);

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

void zeros(Matrix *matrix)
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

void fillMatrix(Matrix *matrix, double *data)
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

void InitRandomMatrix(Matrix *matrix)
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

void eye(Matrix *matrix)
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

bool cmpMatrix(Matrix m1, Matrix m2)
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

void ones(Matrix *matrix)
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

void dot(Matrix m1, Matrix m2, union Result *res)
{
    assert(m1.columns == m2.rows);

    int rows = m1.rows;
    int cols = m2.columns;
    int middle = m1.columns;
    
    int r;
    int c;
    int i;

    //if( (rows == 1) && (cols == 1) && 0 )
    //{
    //    res->res = 0;
    //    for(c = 0; c < middle; c++)
    //    {
    //        res->res += m1.data[0][c] * m2.data[c][0];
    //    }
    //}
    //else
    //{
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

    //}
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

void copyMatrix(Matrix src, Matrix *dest)
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

void matMult(Matrix m1, Matrix m2, Matrix *res)
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

void hadamard_prod(Matrix m1, Matrix m2, Matrix* res)
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
