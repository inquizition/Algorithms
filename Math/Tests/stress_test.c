#include "stress_test.h"

void stress_test_dot(void) {

  double data[2][2] = {
      {3.5, 2.3},
      {8.9, 9.5},
  };

  double data2[2][2] = {
      {5.3, 6.2},
      {8.6, 8.7},
  };

  double expected[2][2] = {
      {38.33, 41.71},
      {128.87, 137.83},
  };

  int i;
  for (i = 0; i < 50000; i++) {
    Matrix *m1 = allocateMatrix(2, 2);
    Matrix *m2 = allocateMatrix(2, 2);
    Matrix *res = allocateMatrix(2, 2);

    fillMatrix(m1, *data);
    fillMatrix(m2, *data2);

    dot(*m1, *m2, (union Result *)res);

    int r;
    int c;
    for (r = 0; r < res->rows; r++) {
      for (c = 0; c < res->columns; c++) {
        CU_ASSERT_EQUAL((int)(10 * res->data[r][c]),
                        (int)(10 * expected[r][c]));
      }
    }

    freeMatrix(m1);
    freeMatrix(m2);
    freeMatrix(res);
  }
}
