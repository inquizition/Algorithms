#include "Matrix_tests.h"

static CU_TestInfo test_cases[] = {
    { "test_createMatrix", test_createMatrix },
    { "test_matrixAdd", test_matrixAdd },
    { "test_matrixAdd_EqualRow", test_matrixAdd_EqualRow },
    { "test_matrixAdd_EqualColumn", test_matrixAdd_EqualColumn },
    { "test_matrixAdd_addConst", test_matrixAdd_addConst },
    { "test_matrixSubtract", test_matrixSubtract },
    { "test_matrixSubtract_EqualRow", test_matrixSubtract_EqualRow },
    { "test_matrixSubtract_EqualColumn", test_matrixSubtract_EqualColumn },
    { "test_matrixSubtract_addConst", test_matrixSubtract_addConst },
    { "tests_transposeMatrix_2x4", tests_transposeMatrix_2x4 },
    { "tests_transposeMatrix_2x2", tests_transposeMatrix_2x2 },
    { "test_eyeMatrix", test_eyeMatrix },
    { "test_swapMatrix", test_swapMatrix },
    { "test_matrixDot_2x2", test_matrixDot_2x2 },
    { "test_matrixDot_2x4", test_matrixDot_2x4 },
    { "test_matrixDot_1x1", test_matrixDot_1x1 },
    { "test_matrixDot_4x4", test_matrixDot_4x4 },
    { "test_hadamardProd_2x2", test_hadamardProd_2x2 },
    { "test_hadamardProd_1x2", test_hadamardProd_1x2 },
    { "tests_compareMatrix", tests_compareMatrix },
    { "test_relu_matrix", test_relu_matrix },

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
    CU_pRunSummary runSummary = CU_get_run_summary();
    unsigned int numTestsFailed = runSummary->nTestsFailed + runSummary->nAssertsFailed;

    CU_pFailureRecord failure = CU_get_failure_list();
    while (failure) {
        numTestsFailed++;
        failure = failure->pNext;
    }

    CU_cleanup_registry();

    if (numTestsFailed > 0)
    {
        return 1;
    }
    else
    {
       return 0;
    }
}
