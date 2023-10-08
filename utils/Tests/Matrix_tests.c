#include "Matrix_tests.h"

void test_createMatrix(void) 
{
    Matrix *m;
    m = allocateMatrix(2, 2);

    CU_ASSERT_PTR_NOT_NULL(m);

    CU_ASSERT_EQUAL(m->columns, 2);
    CU_ASSERT_EQUAL(m->rows, 2);

    CU_ASSERT_EQUAL(m->data[0][0], 0);
    CU_ASSERT_EQUAL(m->data[1][0], 0);
    CU_ASSERT_EQUAL(m->data[0][1], 0);
    CU_ASSERT_EQUAL(m->data[1][1], 0);

    freeMatrix(m);
}

void test_matrixAdd(void)
{
    Matrix *m = allocateMatrix(2, 2);
    Matrix *a = allocateMatrix(2, 2);
    Matrix *res = allocateMatrix(2,2);

    double data[2][2] = {{1,2},{3,4}};
    fillMatrix(a, *data);

    matrixAdd(*m, *a, res);

    double expected[][4] = {{1,2},{3,4}};
    int r;
    int c;
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
            CU_ASSERT_EQUAL(res->data[r][c], expected[r][c]);
        }
    }

    freeMatrix(m);
    freeMatrix(a);
    freeMatrix(res);
}

void test_matrixAdd_EqualRow(void)
{
    Matrix *m = allocateMatrix(2, 4);
    Matrix *a = allocateMatrix(2, 1);
    Matrix *res = allocateMatrix(2,4);

    double data[2][1] = {{1},{2}};
    fillMatrix(a, *data);
    matrixAdd(*m, *a, res);

    double expected[][4] = {{1,1,1,1},{2,2,2,2}};
    int r;
    int c;
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
            CU_ASSERT_EQUAL(res->data[r][c], expected[r][c]);
        }
    }

    freeMatrix(m);
    freeMatrix(a);
    freeMatrix(res);
}

void test_matrixAdd_EqualColumn(void)
{
    Matrix *m = allocateMatrix(2, 4);
    Matrix *a = allocateMatrix(1, 4);
    Matrix *res = allocateMatrix(2,4);

    double data[1][4] = {{1,2,3,4}};
    fillMatrix(a, *data);
    matrixAdd(*m, *a, res);

    double expected[][4] = {{1,2,3,4},{1,2,3,4}};
    int r;
    int c;
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
            CU_ASSERT_EQUAL(res->data[r][c], expected[r][c]);
        }
    }

    freeMatrix(m);
    freeMatrix(a);
    freeMatrix(res);
}

void test_matrixAdd_addConst(void)
{
    Matrix *m = allocateMatrix(2, 4);
    Matrix *a = allocateMatrix(1, 1);
    Matrix *res = allocateMatrix(2,4);

    double data[1][1] = {{5}};
    fillMatrix(a, *data);
    matrixAdd(*m, *a, res);

    double expected[][4] = {{5,5,5,5},{5,5,5,5}};
    int r;
    int c;
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
            CU_ASSERT_EQUAL(res->data[r][c], expected[r][c]);
        }
    }

    freeMatrix(m);
    freeMatrix(a);
    freeMatrix(res);
}

void test_matrixSubtract(void)
{
    Matrix *m = allocateMatrix(2, 2);
    Matrix *a = allocateMatrix(2, 2);
    Matrix *res = allocateMatrix(2, 2);

    double data[2][2] = {{1,2},{3,4}};
    fillMatrix(a, *data);
    matrixSubtract(*m, *a, res);

    double expected[][4] = {{-1,-2},{-3,-4}};
    int r;
    int c;
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
            CU_ASSERT_EQUAL(res->data[r][c], expected[r][c]);
        }
    }

    freeMatrix(m);
    freeMatrix(a);
    freeMatrix(res);
}

void test_matrixSubtract_EqualRow(void)
{
    Matrix *m = allocateMatrix(2, 4);
    Matrix *a = allocateMatrix(2, 1);
    Matrix *res = allocateMatrix(2,4);

    double data[2][1] = {{1},{2}};
    fillMatrix(a, *data);
    matrixSubtract(*m, *a, res);

    double expected[][4] = {{-1,-1,-1,-1},{-2,-2,-2,-2}};
    int r;
    int c;
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
            CU_ASSERT_EQUAL(res->data[r][c], expected[r][c]);
        }
    }

    freeMatrix(m);
    freeMatrix(a);
    freeMatrix(res);
}

void test_matrixSubtract_EqualColumn(void)
{
    Matrix *m = allocateMatrix(2, 4);
    Matrix *a = allocateMatrix(1, 4);
    Matrix *res = allocateMatrix(2,4);

    double data[1][4] = {{1,2,3,4}};
    fillMatrix(a, *data);
    matrixSubtract(*m, *a, res);

    double expected[][4] = {{-1,-2,-3,-4},{-1,-2,-3,-4}};
    int r;
    int c;
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
            CU_ASSERT_EQUAL(res->data[r][c], expected[r][c]);
        }
    }

    freeMatrix(m);
    freeMatrix(a);
    freeMatrix(res);
}

void test_matrixSubtract_addConst(void)
{
    Matrix *m = allocateMatrix(2, 4);
    Matrix *a = allocateMatrix(1, 1);
    Matrix *res = allocateMatrix(2,4);

    double data[1][1] = {{5}};
    fillMatrix(a, *data);
    matrixSubtract(*m, *a, res);

    double expected[][4] = {{-5,-5,-5,-5},{-5,-5,-5,-5}};
    int r;
    int c;
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
            CU_ASSERT_EQUAL(res->data[r][c], expected[r][c]);
        }
    }

    freeMatrix(m);
    freeMatrix(a);
    freeMatrix(res);
}

void tests_transposeMatrix_2x4(void)
{
    Matrix *m = allocateMatrix(2, 4);

    double data[2][4] = {{1,3,5,7},{2,4,6,8}};

    fillMatrix(m, *data);
    transpose(&m);

    double expected[][4] = {{1,2},{3,4},{5,6},{7,8}};
    int r;
    int c;
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
            CU_ASSERT_EQUAL(m->data[r][c], expected[r][c]);
        }
    }

    freeMatrix(m);
}

void tests_transposeMatrix_2x2(void)
{
    Matrix *m = allocateMatrix(2, 2);

    double data[2][2] = {{1,2},{3,4}};

    fillMatrix(m, *data);
    transpose(&m);

    double expected[][2] = {{1,3},{2,4}};
    int r;
    int c;
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
            CU_ASSERT_EQUAL(m->data[r][c], expected[r][c]);
        }
    }

    freeMatrix(m);
}

void test_eyeMatrix(void)
{

    Matrix *m = allocateMatrix(4, 4);
    eye(m);
    double expected[][4] = {{1,0,0,0},
                            {0,1,0,0},
                            {0,0,1,0},
                            {0,0,0,1},};
    int r;
    int c;
    for(r = 0; r < m->rows; r++)
    {
        for(c = 0; c < m->columns; c++)
        {
            CU_ASSERT_EQUAL(m->data[r][c], expected[r][c]);
        }
    }

    freeMatrix(m);
}

void test_swapMatrix(void)
{
    Matrix *m1 = allocateMatrix(4,4);
    Matrix *m2 = allocateMatrix(4,4);
    InitRandomMatrix(m1);

    int r;
    int c;
    for(r = 0; r < m1->rows; r++)
    {
        for(c = 0; c < m1->columns; c++)
        {
            CU_ASSERT_NOT_EQUAL(m1->data[r][c], m2->data[r][c]);
        }
    }

    copyMatrix(*m1, m2);

    for(r = 0; r < m1->rows; r++)
    {
        for(c = 0; c < m1->columns; c++)
        {
            CU_ASSERT_EQUAL(m1->data[r][c], m2->data[r][c]);
        }
    }

    freeMatrix(m1);
    freeMatrix(m2);
}

void test_matrixDot_2x2(void)
{
    Matrix *m1 = allocateMatrix(2,2);
    Matrix *m2 = allocateMatrix(2,2);
    Matrix *res = allocateMatrix(2,2);

    double data[2][2] = {   {3.5, 2.3},
                            {8.9, 9.5},   };

    double data2[2][2] = {  {5.3, 6.2},
                            {8.6, 8.7},   };

    fillMatrix(m1, *data);
    fillMatrix(m2, *data2);

    dot(*m1, *m2, (union Result*)res);

    double expected[2][2] = {   { 38.33, 41.71 },
                                {128.87, 137.83},   };
    int r;
    int c;
    for(r = 0; r < res->rows; r++)
    {
        for(c = 0; c < res->columns; c++)
        {
            CU_ASSERT_EQUAL((int)(10*res->data[r][c]), (int)(10*expected[r][c]));
        }
    }

    freeMatrix(m1);
    freeMatrix(m2);
    freeMatrix(res);
}

void test_matrixDot_2x4(void)
{
    Matrix *m1 = allocateMatrix(2,2);
    Matrix *m2 = allocateMatrix(2,4);
    Matrix *res = allocateMatrix(2,4);

    double data[2][2] = {   {3.5, 2.3},
                            {8.9, 9.5},   };

    double data2[2][4] = {  {5.3, 6.2, 4.6, 7.8},
                            {8.6, 8.7, 2.2, 1.5},   };

    fillMatrix(m1, *data);
    fillMatrix(m2, *data2);

    dot(*m1, *m2, (union Result*)res);

    double expected[2][4] = {   { 38.33,  41.71,  21.16,  30.75 },
                                {128.87, 137.83,  61.84,  83.67},   };
    int r;
    int c;
    for(r = 0; r < res->rows; r++)
    {
        for(c = 0; c < res->columns; c++)
        {
            CU_ASSERT_EQUAL((int)(10*res->data[r][c]), (int)(10*expected[r][c]));
        }
    }

    freeMatrix(m1);
    freeMatrix(m2);
    freeMatrix(res);
}

void test_matrixDot_1x1(void)
{
    Matrix *m1 = allocateMatrix(1,4);
    Matrix *m2 = allocateMatrix(4,1);
    Matrix *res = allocateMatrix(1,1);

    double data[1][4] = {   {3.5, 2.3, 8.9, 9.5} };

    double data2[4][1] = {  { 5.3 },
                            { 6.2 },
                            { 4.6 },
                            { 7.8 }     };

    fillMatrix(m1, *data);
    fillMatrix(m2, *data2);
    dot(*m1, *m2, (union Result*)res);
    double expected = 147.85;

    CU_ASSERT_EQUAL((int)(10*res->data[0][0]), (int)(10*expected));

    freeMatrix(m1);
    freeMatrix(m2);
}

void test_matrixDot_4x4(void)
{
    Matrix *m1 = allocateMatrix(4,2);
    Matrix *m2 = allocateMatrix(2,4);
    Matrix *res = allocateMatrix(4,4);

    double data[4][2] = {  { 5.3, 3.5 },
                            { 6.2, 6.7 },
                            { 4.6, 9.9 },
                            { 7.8, 5.4 }    };

    double data2[2][4] = {  {3.5, 2.3, 8.9, 9.5},
                            {6.7, 3.6, 9.2, 4.4}    };

    fillMatrix(m1, *data);
    fillMatrix(m2, *data2);

    dot(*m1, *m2, (union Result*)res);

    double expected[4][4] = {   { 42.  ,  24.79,  79.37,  65.75 },
                                {66.59,  38.38, 116.82,  88.38  },
                                {82.43,  46.22, 132.02,  87.26  },
                                {63.48,  37.38, 119.1 ,  97.86  },   };
    int r;
    int c;
    for(r = 0; r < res->rows; r++)
    {
        for(c = 0; c < res->columns; c++)
        {
            CU_ASSERT_EQUAL((int)(10*res->data[r][c]), (int)(10*expected[r][c]));
        }
    }

    freeMatrix(m1);
    freeMatrix(m2);
    freeMatrix(res);
}

void test_hadamardProd_2x2(void)
{
    Matrix *m1 = allocateMatrix(2,2);
    Matrix *m2 = allocateMatrix(2,2);
    Matrix *res = allocateMatrix(2,2);

    double data[2][2] = {   { 5.3, 3.5 },
                            { 6.2, 6.7 },   };

    double data2[2][2] = {  {3.5, 2.3},
                            {6.7, 3.6}    };

    fillMatrix(m1, *data);
    fillMatrix(m2, *data2);

    hadamard_prod(*m1, *m2, res);

    double expected[2][2] = {   { 5.3*3.5   ,  3.5*2.3,  },
                                {6.2*6.7    ,  6.7*3.6,  },  };
    int r;
    int c;
    for(r = 0; r < res->rows; r++)
    {
        for(c = 0; c < res->columns; c++)
        {
            CU_ASSERT_EQUAL((int)(10*res->data[r][c]), (int)(10*expected[r][c]));
        }
    }

    freeMatrix(m1);
    freeMatrix(m2);
    freeMatrix(res);
}

void test_hadamardProd_1x2(void)
{
    Matrix *m1 = allocateMatrix(1,2);
    Matrix *m2 = allocateMatrix(1,2);
    Matrix *res = allocateMatrix(1,2);

    double data[1][2] = {   { 5.3, 3.5 }  };

    double data2[1][2] = {  {3.5, 2.3}   };

    fillMatrix(m1, *data);
    fillMatrix(m2, *data2);

    hadamard_prod(*m1, *m2, res);

    double expected[1][2] = {   { 5.3*3.5   ,  3.5*2.3,  }  };
    int r;
    int c;
    for(r = 0; r < res->rows; r++)
    {
        for(c = 0; c < res->columns; c++)
        {
            CU_ASSERT_EQUAL((int)(10*res->data[r][c]), (int)(10*expected[r][c]));
        }
    }

    freeMatrix(m1);
    freeMatrix(m2);
    freeMatrix(res);
}

void tests_compareMatrix(void)
{
    Matrix *m1 = allocateMatrix(2,2);
    Matrix *m2 = allocateMatrix(2,2);

    double data[2][2] = {   { 5.3, 3.5 },
                            { 6.2, 6.7 },   };

    double data2[2][2] = {  { 5.3, 3.5 },
                            { 6.2, 6.7 },   };

    fillMatrix(m1, *data);
    fillMatrix(m2, *data2);

    CU_ASSERT_TRUE(cmpMatrix(*m1, *m2));
    int r;
    int c;
    double temp;
    for(r = 0; r < m1->rows; r++)
    {
        for(c = 0; c < m1->columns; c++)
        {
            temp = m1->data[r][c];
            m1->data[r][c] = 1.1;
            CU_ASSERT_FALSE(cmpMatrix(*m1, *m2));
            m1->data[r][c] = temp;
        }
    }

    CU_ASSERT_TRUE(cmpMatrix(*m1, *m2));

    freeMatrix(m1);
    freeMatrix(m2);
}

void test_relu_matrix(void)
{
    Matrix *m1 = allocateMatrix(2,2);
    Matrix *m2 = allocateMatrix(2,2);
    Matrix *e_m1 = allocateMatrix(2,2);
    Matrix *e_m2 = allocateMatrix(2,2);


    double data[2][2] = {   { 5.3, -3.5 },
                            { -6.2, 0.0 },   };

    double data2[2][2] = {  { -5.3, 3.5 },
                            { 6.2, -6.7 },   };

     double exp[2][2] = {   { 5.3, 0.0 },
                            { 0.0, 0.0 },   };

    double exp2[2][2] = {  { 0.0, 3.5 },
                            { 6.2, 0.0 },   };
    fillMatrix(m1, *data);
    fillMatrix(m2, *data2);
    fillMatrix(e_m1, *exp);
    fillMatrix(e_m2, *exp2);

    reLu_matrix(m1);
    reLu_matrix(m2);

    CU_ASSERT_TRUE(cmpMatrix(*m1, *e_m1));
    CU_ASSERT_TRUE(cmpMatrix(*m2, *e_m2));

    freeMatrix(m1);
    freeMatrix(m2);
    freeMatrix(e_m1);
    freeMatrix(e_m2);
}
