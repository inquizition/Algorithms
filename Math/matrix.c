#include "matrix.h"

static void swap(double *arg1, double *arg2);

/* Function returns the max value of the Matrix */
static double max_matrix(Matrix *m) {
  double max = m->data[0];
  int r, c;

#pragma omp parallel for private(c) reduction(max : max)
  for (r = 0; r < m->rows; r++) {
    for (c = 0; c < m->columns; c++) {
      double val = m->data[r * m->columns + c];
      if (val > max) {
        max = val;
      }
    }
  }
  return max;
}

double logsumexp(Matrix *m) {
  int i, j;
  double res = 0.0;
  double max = max_matrix(m);

#pragma omp parallel for private(j) reduction(+ : res)
  for (i = 0; i < m->rows; i++) {
    for (j = 0; j < m->columns; j++) {
      res += exp(m->data[i * m->columns + j] - max);
    }
  }

  return log(res) + max;
}

void matrix_pow(Matrix *m, int power) {
  int r, c;

#pragma omp parallel for private(c)
  for (r = 0; r < m->rows; r++) {
    for (c = 0; c < m->columns; c++) {
      int idx = r * m->columns + c;
      m->data[idx] = pow(m->data[idx], power);
    }
  }
}

void exp_matrix(Matrix *m) {
  int r, c;

#pragma omp parallel for private(c)
  for (r = 0; r < m->rows; r++) {
    for (c = 0; c < m->columns; c++) {
      int idx = r * m->columns + c;
      m->data[idx] = exp(m->data[idx]);
    }
  }
}

void const_mult_matrix(Matrix *m, double C) {
  int r, c;

#pragma omp parallel for private(c)
  for (r = 0; r < m->rows; r++) {
    for (c = 0; c < m->columns; c++) {
      m->data[r * m->columns + c] *= C;
    }
  }
}

void reLu_matrix(Matrix *m) {
  int r, c;

#pragma omp parallel for private(c)
  for (r = 0; r < m->rows; r++) {
    for (c = 0; c < m->columns; c++) {
      int idx = r * m->columns + c;
      if (m->data[idx] < 0) {
        m->data[idx] = 0;
      }
    }
  }
}

void sigmoid_matrix(Matrix *m) {
  int r, c;

#pragma omp parallel for private(c)
  for (r = 0; r < m->rows; r++) {
    for (c = 0; c < m->columns; c++) {
      int idx = r * m->columns + c;
      m->data[idx] = 1.0 / (1.0 + exp(-m->data[idx]));
    }
  }
}

void d_reLu_matrix(Matrix *m) {
  int r, c;

#pragma omp parallel for private(c)
  for (r = 0; r < m->rows; r++) {
    for (c = 0; c < m->columns; c++) {
      int idx = r * m->columns + c;
      m->data[idx] = (m->data[idx] > 0) ? 1 : 0;
    }
  }
}

void print_dim(Matrix m) { printf("Dim = (r:%d, c:%d)\n", m.rows, m.columns); }

void print_matrix(Matrix m, char *header) {
  printf("%s: r %d c %d", header, m.rows, m.columns);
  int M = m.columns;
  int N = m.rows;
  int r, c;

  printf("\n");
  for (r = 0; r < N; r++) {
    for (c = 0; c < M; c++) {
      printf("%.4f   ", m.data[r * m.columns + c]);
    }
    printf("\n\n");
  }
  printf("\n");
}

Matrix *allocateMatrix(int rows, int columns) {
  Matrix *m_ptr = (Matrix *)malloc(sizeof(Matrix));
  if (!m_ptr) {
    return NULL;
  }

  m_ptr->rows = rows;
  m_ptr->columns = columns;
  m_ptr->data = (double *)calloc((size_t)rows * columns, sizeof(double));
  m_ptr->next = NULL;

  if (allocated_matrices == NULL) {
    allocated_matrices = m_ptr;
  } else {
    Matrix *current = allocated_matrices;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = m_ptr;
  }

  return m_ptr;
}

void freeMatrix(Matrix *m) {
  if (m == NULL)
    return;

  if (allocated_matrices == m) {
    allocated_matrices = m->next;
  } else {
    Matrix *current = allocated_matrices;
    while (current != NULL && current->next != m) {
      current = current->next;
    }
    if (current != NULL) {
      current->next = m->next;
    }
  }

  free(m->data);
  free(m);
}

void printAllocatedMatricesCount(void) {
  Matrix *current = allocated_matrices;
  int i = 0;
  while (current != NULL) {
    i++;
    Matrix *next = current->next;
    current = next;
  }
  printf("Allocated matrices: %d\n", i);
}

void freeAllMatrices(void) {
  Matrix *current = allocated_matrices;
  int i = 0;
  while (current != NULL) {
    printf("\nCleaning up leftover matrices: %d\n", i);
    i++;
    Matrix *next = current->next;
    freeMatrix(current);
    current = next;
  }
  allocated_matrices = NULL;
}

static void swap(double *arg1, double *arg2) {
  double buffer = *arg1;
  *arg1 = *arg2;
  *arg2 = buffer;
}

void matrixAdd(Matrix matrix, Matrix a, Matrix *res) {
  bool single_element = (a.columns == 1 && a.rows == 1);
  bool equal_rows = (a.rows == matrix.rows);
  bool equal_columns = (a.columns == matrix.columns);

  int r, c;
  int rows = matrix.rows;
  int columns = matrix.columns;

#pragma omp parallel for private(c)
  for (r = 0; r < rows; r++) {
    for (c = 0; c < columns; c++) {
      if (single_element) {
        res->data[r * res->columns + c] =
            matrix.data[r * matrix.columns + c] + a.data[0];
      } else if (equal_rows && equal_columns) {
        res->data[r * res->columns + c] =
            matrix.data[r * matrix.columns + c] +
            a.data[r * a.columns + c];
      } else if (equal_rows) {
        res->data[r * res->columns + c] =
            matrix.data[r * matrix.columns + c] + a.data[r * a.columns + 0];
      } else if (equal_columns) {
        res->data[r * res->columns + c] =
            matrix.data[r * matrix.columns + c] + a.data[0 * a.columns + c];
      }
    }
  }
}

void flatten(Matrix *m, Matrix *flattened) {
  int r, c;

#pragma omp parallel for private(c)
  for (r = 0; r < m->rows; r++) {
    for (c = 0; c < m->columns; c++) {
      flattened->data[c + (r * m->columns)] =
          m->data[r * m->columns + c];
    }
  }
}

void dump_matrix(Matrix m, double *data) {
  int r, c;
#pragma omp parallel for private(c)
  for (r = 0; r < m.rows; r++) {
    for (c = 0; c < m.columns; c++) {
      data[c + (r * m.columns)] = m.data[r * m.columns + c];
    }
  }
}

void matrixSubtract(Matrix matrix, Matrix a, Matrix *res) {
  bool single_element = (a.columns == 1 && a.rows == 1);
  bool equal_rows = (a.rows == matrix.rows);
  bool equal_columns = (a.columns == matrix.columns);

  int r, c;

#pragma omp parallel for private(c)
  for (r = 0; r < matrix.rows; r++) {
    for (c = 0; c < matrix.columns; c++) {
      if (single_element) {
        res->data[r * res->columns + c] =
            matrix.data[r * matrix.columns + c] - a.data[0];
      } else if (equal_rows && equal_columns) {
        res->data[r * res->columns + c] =
            matrix.data[r * matrix.columns + c] -
            a.data[r * a.columns + c];
      } else if (equal_rows) {
        res->data[r * res->columns + c] =
            matrix.data[r * matrix.columns + c] - a.data[r * a.columns + 0];
      } else if (equal_columns) {
        res->data[r * res->columns + c] =
            matrix.data[r * matrix.columns + c] - a.data[0 * a.columns + c];
      }
    }
  }
}

void transpose(Matrix **m) {
  Matrix *temp = *m;
  int M = temp->columns;
  int N = temp->rows;
  int r, c;

  Matrix *transposed_matrix = allocateMatrix(M, N);

#pragma omp parallel for private(r, c)
  for (r = 0; r < N; r++) {
    for (c = 0; c < M; c++) {
      transposed_matrix->data[c * transposed_matrix->columns + r] =
          temp->data[r * temp->columns + c];
    }
  }

  *m = transposed_matrix;
  freeMatrix(temp);
}

void zeros(Matrix *matrix) {
  int r, c;

#pragma omp parallel for private(c)
  for (r = 0; r < matrix->rows; r++) {
    for (c = 0; c < matrix->columns; c++) {
      matrix->data[r * matrix->columns + c] = 0.00;
    }
  }
}

void fillMatrix(Matrix *matrix, double *data) {
  int r, c;

#pragma omp parallel for private(c)
  for (r = 0; r < matrix->rows; r++) {
    for (c = 0; c < matrix->columns; c++) {
      matrix->data[r * matrix->columns + c] =
          data[c + (matrix->columns * r)];
    }
  }
}

void InitRandomMatrix(Matrix *matrix) {
  int r, c;
  double random_number = 0.0;

#pragma omp parallel for private(c, random_number)
  for (r = 0; r < matrix->rows; r++) {
    for (c = 0; c < matrix->columns; c++) {
      random_number =
          ((double)(rand() - ((double)RAND_MAX / 2)) / ((double)RAND_MAX / 2));
      matrix->data[r * matrix->columns + c] = random_number;
    }
  }
}

void eye(Matrix *matrix) {
  int r, c;

#pragma omp parallel for private(c)
  for (r = 0; r < matrix->rows; r++) {
    for (c = 0; c < matrix->columns; c++) {
      if (c == r) {
        matrix->data[r * matrix->columns + c] = 1.0;
      } else {
        matrix->data[r * matrix->columns + c] = 0.0;
      }
    }
  }
}

bool cmpMatrix(Matrix m1, Matrix m2) {
  int r, c;
  bool equal = true;
#define EPSILON 1e-6

#pragma omp parallel for private(c) reduction(&& : equal)
  for (r = 0; r < m1.rows; r++) {
    for (c = 0; c < m1.columns; c++) {
      if (fabs(m1.data[r * m1.columns + c] -
               m2.data[r * m2.columns + c]) > EPSILON) {
        // printf("a: %.4f not equal to b: %.4f, difference: %.4f
        // \n",m1.data[r][c], m2.data[r][c], fabs(m1.data[r][c] -
        // m2.data[r][c]));
        equal = false;
      }
    }
  }

  return equal;
}

void ones(Matrix *matrix) {
  int r, c;

#pragma omp parallel for private(c)
  for (r = 0; r < matrix->rows; r++) {
    for (c = 0; c < matrix->columns; c++) {
      matrix->data[r * matrix->columns + c] = 1.0;
    }
  }
}

void dot(Matrix m1, Matrix m2, union Result *res) {
  assert(m1.columns == m2.rows);

  int rows = m1.rows;
  int cols = m2.columns;
  int middle = m1.columns;

  int r, c, i;

  assert(res->m.rows == rows);
  assert(res->m.columns == cols);

#pragma omp parallel for private(c, i)
  for (r = 0; r < res->m.rows; r++) {
    for (c = 0; c < res->m.columns; c++) {
      for (i = 0; i < middle; i++) {
        res->m.data[r * res->m.columns + c] +=
            m1.data[r * m1.columns + i] * m2.data[i * m2.columns + c];
      }
    }
  }
}

double vectorMult(double *v1, double *v2, int length) {
  double res = 0.0;
  int i;

#pragma omp parallel for reduction(+ : res)
  for (i = 0; i < length; i++) {
    res += v1[i] * v2[i];
  }

  return res;
}

void copyMatrix(Matrix src, Matrix *dest) {
  if ((dest->columns != src.columns) || (dest->rows != src.rows)) {
    printf("Copy matrix mismatch, c [%d : %d], r [%d : %d]\n", dest->columns,
           src.columns, dest->rows, src.rows);
  }
  // dest->columns = src.columns;
  // dest->rows = src.rows;
  int r, c;

#pragma omp parallel for private(c)
  for (r = 0; r < src.rows; r++) {
    for (c = 0; c < src.columns; c++) {
      dest->data[r * dest->columns + c] = src.data[r * src.columns + c];
    }
  }
}

void matMult(Matrix m1, Matrix m2, Matrix *res) {
  assert(m1.columns == m2.rows);

  res->rows = m1.rows;
  res->columns = m2.columns;

  int size = res->rows * res->columns;
  int i;

#pragma omp parallel for
  for (i = 0; i < size; i++) {
    res->data[i] = 0.0;
  }
int r, c, k;
#pragma omp parallel for private(c, k)
  for (r = 0; r < res->rows; r++) {
    for (c = 0; c < res->columns; c++) {
      for (k = 0; k < m1.columns; k++) {
        res->data[r * res->columns + c] +=
            m1.data[r * m1.columns + k] *
            m2.data[k * m2.columns + c];
      }
    }
  }
}

void hadamard_prod(Matrix m1, Matrix m2, Matrix *res) {
  res->rows = m1.rows;
  res->columns = m1.columns;

  assert(m1.columns == m2.columns);
  assert(m1.rows == m2.rows);

  int r;
  int c;

#pragma omp parallel for private(c)
  for (r = 0; r < m1.rows; r++) {
    for (c = 0; c < m1.columns; c++) {
      res->data[r * res->columns + c] +=
          m1.data[r * m1.columns + c] * m2.data[r * m2.columns + c];
    }
  }
}

double matrix_sum(Matrix *m) {
  double res = 0.0;
  int r;
  int c;

#pragma omp parallel for private(c) reduction(+ : res)
  for (r = 0; r < m->rows; r++) {
    for (c = 0; c < m->columns; c++) {
      res += m->data[r * m->columns + c];
    }
  }

  return res;
}
