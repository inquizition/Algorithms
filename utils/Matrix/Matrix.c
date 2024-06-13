#include "Matrix.h"


static void swap(double* arg1, double* arg2);
static double **allocate2Darray(int rows, int columns);
static void free2Darray(double **arr, int rows, int columns);

/* Function returns the max value of the Matrix */
static double max_matrix(Matrix *m)
{
    double max = m->data[0][0];
    int r, c;

    #pragma omp parallel for private(c) reduction(max : max)
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
            if(m->data[r][c] > max)
            {
                max = m->data[r][c];
            }
        }
    }
    return max;
}

double logsumexp(Matrix *m)
{
    int i, j;
    double res = 0.0;
    double max = max_matrix(m);

    #pragma omp parallel for private(j) reduction(+:res)
    for(i = 0; i < m->rows; i++)
    {
        for(j = 0; j < m->columns; j++)
        {
            res += exp(m->data[i][j] - max);
        }
    }

    return log(res) + max;
}

void matrix_pow(Matrix *m, int power)
{
    int r, c;

    #pragma omp parallel for private(c)
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
            m->data[r][c] = pow(m->data[r][c], power);
        }
    }
}

void exp_matrix(Matrix *m)
{
    int r, c;

    #pragma omp parallel for private(c)
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
            m->data[r][c] = exp(m->data[r][c]);
        }
    }
}

void const_mult_matrix(Matrix *m, double C)
{
    int r, c;

    #pragma omp parallel for private(c)
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
            m->data[r][c] *= C;
        }
    }
}

void reLu_matrix(Matrix *m)
{
    int r, c;

    #pragma omp parallel for private(c)
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

void d_reLu_matrix(Matrix *m)
{
    int r, c;

    #pragma omp parallel for private(c)
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
            m->data[r][c] = (m->data[r][c] > 0) ? 1 : 0;
        }
    }
}

void print_dim(Matrix m)
{
    printf("Dim = (r:%d, c:%d)\n", m.rows, m.columns);
}

void print_matrix(Matrix m, char* header)
{
    printf("%s: r %d c %d", header, m.rows, m.columns);
    int M = m.columns;
    int N = m.rows;
    int r, c;

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
    m_ptr->next = NULL;

    int r, c;

    #pragma omp parallel for private(c)
    for(r = 0; r < m_ptr->rows; r++)
    {
        for(c = 0; c < m_ptr->columns; c++)
        {
            m_ptr->data[r][c] = 0.00;
        }
    }

    if (allocated_matrices == NULL)
    {
        allocated_matrices = m_ptr;
    }
    else
    {
	Matrix *current = allocated_matrices;
	while (current->next != NULL)
	{
	    current = current->next;
	}
	current->next = m_ptr;
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
        arr_ptr[r] = (double*)malloc(columns * sizeof(double));
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
    if(m == NULL) return;

    if(allocated_matrices == m)
    {
	allocated_matrices = m->next;
    }
    else
    {
	Matrix *current = allocated_matrices;
	while(current != NULL && current->next != m)
	{
	    current = current->next;
	}
	if(current != NULL)
	{
	    current->next = m->next;
	}
    }

    free2Darray(m->data, m->rows, m->columns);
    free(m);
}

void freeAllMatrices(void)
{
    Matrix *current = allocated_matrices;
    while (current != NULL) {
        Matrix *next = current->next;
        freeMatrix(current);
        current = next;
    }
    allocated_matrices = NULL; 

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

    int r, c;
    int rows = matrix.rows;
    int columns = matrix.columns;

    #pragma omp parallel for private(c)
    for(r = 0; r < rows; r++)
    {
        for(c = 0; c < columns; c++)
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

void flatten(Matrix *m, Matrix *flattened)
{
    int r, c;

    #pragma omp parallel for private(c)
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
            flattened->data[0][c + (r * m->columns)] = m->data[r][c];
        }
    }
}

void matrixSubtract(Matrix matrix, Matrix a, Matrix *res)
{
    bool single_element = (a.columns == 1 && a.rows == 1);
    bool equal_rows = (a.rows == matrix.rows);
    bool equal_columns = (a.columns == matrix.columns);

    int r, c;

    #pragma omp parallel for private(c)
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
    int r, c;

    Matrix *transposed_matrix = allocateMatrix(M, N);

    #pragma omp parallel for private(r, c)
    for(r = 0; r < N; r++)
    {
        for(c = 0; c < M; c++)
        {
            transposed_matrix->data[c][r] = temp->data[r][c];
        }
    }

    *m = transposed_matrix;
    freeMatrix(temp);
}

void zeros(Matrix *matrix)
{
    int r, c;

    #pragma omp parallel for private(c)
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
    int r, c;

    #pragma omp parallel for private(c)
    for(r = 0; r < matrix->rows; r++)
    {
        for(c = 0; c < matrix->columns; c++)
        {
            matrix->data[r][c] = data[c + (matrix->columns * r)];
        }
    }
}

void InitRandomMatrix(Matrix *matrix)
{
    int r, c;
    double random_number = 0.0;

    #pragma omp parallel for private(c, random_number)
    for(r = 0; r < matrix->rows; r++)
    {
        for(c = 0; c < matrix->columns; c++)
        {
            random_number = ((double)(rand() - ((double)RAND_MAX / 2)) / ((double)RAND_MAX / 2));
            matrix->data[r][c] = random_number;
        }
    }
}

void eye(Matrix *matrix)
{
    int r, c;

    #pragma omp parallel for private(c)
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
    int r, c;
    bool equal = true;
#define EPSILON 1e-9

    #pragma omp parallel for private(c) reduction(&&:equal)
    for(r = 0; r < m1.rows; r++)
    {
        for(c = 0; c < m1.columns; c++)
        {
            if(fabs(m1.data[r][c] - m2.data[r][c]) > EPSILON)
            {
		printf("a: %.4f not equal to b: %.4f, difference: %.4f \n",m1.data[r][c], m2.data[r][c], fabs(m1.data[r][c] - m2.data[r][c]));
                equal = false;
            }
        }
    }

    return equal;
}

void ones(Matrix *matrix)
{
    int r, c;

    #pragma omp parallel for private(c)
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

    int r, c, i;

    assert(res->m.rows == rows);
    assert(res->m.columns == cols);

    #pragma omp parallel for private(c, i)
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

double vectorMult(double *v1, double *v2, int length)
{
    double res = 0.0;
    int i;

    #pragma omp parallel for reduction(+:res)
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
    int r, c;

    #pragma omp parallel for private(c)
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

    assert(m1.columns == m2.columns);
    assert(m1.rows == m2.rows);

    int r;
    int c;
	
    #pragma omp parallel for private(c)
    for(r = 0; r < m1.rows; r++)
    {
        for(c = 0; c < m1.columns; c++)
        {
            res->data[r][c] += m1.data[r][c] * m2.data[r][c];
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

    #pragma omp parallel for private(c)
    for(r = 0; r < m1.rows; r++)
    {
        for(c = 0; c < m1.columns; c++)
        {
            res->data[r][c] += m1.data[r][c] * m2.data[r][c];
        }
    }
}

double matrix_sum(Matrix *m)
{
    double res;
    int r;
    int c;

    #pragma omp parallel for private(c)
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
            res += m->data[r][c];
        }
    }

    return res;
}
