#include "NonLinearModel_tests.h"

static CU_TestInfo test_cases[] = {
    { "test_init_non_linear", test_init_NonLinear },
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
