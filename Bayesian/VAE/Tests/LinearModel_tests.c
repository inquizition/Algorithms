#include "LinearModel_tests.h"

void test_init_linear(void)
{

	LM *linear = InitLinear(4,4);
	double expected = 0.1;
	int r;
	int c;
	for(r = 0; r < linear->A->rows; r++)
	{
		for(c = 0; c < linear->A->columns; c++)
		{
			CU_ASSERT_EQUAL(linear->A->data[r][c], expected);
		}
	}

	for(r = 0; r < linear->b->rows; r++)
	{
		for(c = 0; c < linear->b->columns; c++)
		{
			CU_ASSERT_EQUAL(linear->b->data[r][c], expected);
		}
	}
	

	freeLinear(linear);
}

void test_linear(void)
{
	LM *linear = InitLinear(4,4);
	Matrix *m = allocateMatrix(1,4);

	Linear(linear, m);

	freeLinear(linear);
	freeMatrix(m);
}

void test_backward(void)
{

}
