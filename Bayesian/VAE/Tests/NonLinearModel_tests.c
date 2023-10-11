#include "NonLinearModel_tests.h"

void test_init_NonLinear(void)
{

	NLM *non_linear = InitNLM(2,4,4);
	double expected = 1.0;
	int r;
	int c;
	for(r = 0; r < non_linear->decoder_fc1->A->rows; r++)
	{
		for(c = 0; c < non_linear->decoder_fc1->A->columns; c++)
		{
			CU_ASSERT_EQUAL(non_linear->decoder_fc1->A->data[r][c], expected);
		}
	}

	for(r = 0; r < non_linear->decoder_fc1->b->rows; r++)
	{
		for(c = 0; c < non_linear->decoder_fc1->b->columns; c++)
		{
			CU_ASSERT_EQUAL(non_linear->decoder_fc1->b->data[r][c], expected);
		}
	}

	for(r = 0; r < non_linear->decoder_fc2->A->rows; r++)
	{
		for(c = 0; c < non_linear->decoder_fc2->A->columns; c++)
		{
			CU_ASSERT_EQUAL(non_linear->decoder_fc2->A->data[r][c], expected);
		}
	}

	for(r = 0; r < non_linear->decoder_fc2->b->rows; r++)
	{
		for(c = 0; c < non_linear->decoder_fc2->b->columns; c++)
		{
			CU_ASSERT_EQUAL(non_linear->decoder_fc2->b->data[r][c], expected);
		}
	}

	freeNonLinear(non_linear);
}
