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
	LM *linear = InitLinear(8,4); // in_features x out_features
        double A_data[4][8] = {{1.1,2.1,3.1,4.1,5.1,6.1,7.1,8.1},{1.1,2.1,3.1,4.1,5.1,6.1,7.1,8.1},{1.1,2.1,3.1,4.1,5.1,6.1,7.1,8.1},{1.1,2.1,3.1,4.1,5.1,6.1,7.1,8.1}};
        double b_data[1][4] = {{1.1,2.1,3.1,4.1}};
	fillMatrix(linear->A, *A_data);
	fillMatrix(linear->b, *b_data);
	
	Matrix *input = allocateMatrix(1,8);
        double input_data[1][8] = {{1.1,2.1,3.1,4.1,5.1,6.1,7.1,8.1}};
	fillMatrix(input, *input_data);

	Linear(linear, input);

	Matrix *expected = allocateMatrix(1,4);
        double expected_data[1][4] = {{212.3800, 213.3800, 214.3800, 215.3800}};
	fillMatrix(expected, *expected_data);

	print_matrix(*linear->output, "output");
	print_matrix(*expected, "expected");

	CU_ASSERT_TRUE(cmpMatrix(*linear->output, *expected));

	freeLinear(linear);
	freeMatrix(input);
}

void test_backward(void)
{

}
