#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "Matrix.h"
#include "LinearModel.h"
#include <stdlib.h>
#include <stdio.h>

void test_createMatrix(void) 
{	
	LM *linear = InitLinear(4,4);
	Matrix *input = allocateMatrix(1,4);
	Matrix *expected = allocateMatrix(4,4);
    	
	double expected_data[4][4] = {{-8,-8,-8,-8},{-2,-2,-2,-2},{-1,-1,-1,-1},{-2,-2,-2,-2}};
	fillMatrix(expected, *expected_data);
	double data[1][4] = {{-2, -1, 1, 2}};
    	fillMatrix(input, *data);
	transpose(&input);

	Linear(linear, input);
	
	print_matrix(*linear->output);

	freeLinear(linear);
	freeMatrix(input);
	freeMatrix(expected);
}


static CU_TestInfo test_cases[] = {
    { "test_createMatrix", test_createMatrix },
    { "test_matrixAdd", test_matrixAdd },
    CU_TEST_INFO_NULL,
};

static CU_SuiteInfo suites[] = {
    { "suite_name", NULL, NULL, NULL, NULL, test_cases },
    CU_SUITE_INFO_NULL,
};


int main( void )
{
    if (CUE_SUCCESS != CU_initialize_registry())
    {
        return CU_get_error();
    }

    if (CUE_SUCCESS != CU_register_suites(suites)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}