#include "q_format_tests.h"

void test_round_trip_q15(void) {
    float values[] = {0.5f, -0.75f, 1.0f};
    for (int i = 0; i < 3; ++i) {
        int32_t q = to_q_format(values[i], 15);
        float back = from_q_format(q, 15);
        CU_ASSERT_DOUBLE_EQUAL(back, values[i], 1e-5);
    }
}

void test_round_trip_q8(void) {
    float values[] = {123.25f, -3.5f, 0.5f};
    for (int i = 0; i < 3; ++i) {
        int32_t q = to_q_format(values[i], 8);
        float back = from_q_format(q, 8);
        CU_ASSERT_DOUBLE_EQUAL(back, values[i], 1e-5);
    }
}

void test_basic_arith_q15(void) {
    int32_t a = to_q_format(0.25f, 15);
    int32_t b = to_q_format(0.5f, 15);
    int32_t sum = Q_ADD(a, b);
    float fsum = from_q_format(sum, 15);
    CU_ASSERT_DOUBLE_EQUAL(fsum, 0.75, 1e-5);

    int32_t prod = Q_MUL(a, b, 15);
    float fprod = from_q_format(prod, 15);
    CU_ASSERT_DOUBLE_EQUAL(fprod, 0.125, 1e-5);
}

void test_basic_arith_q8(void) {
    int32_t a = to_q_format(3.0f, 8);
    int32_t b = to_q_format(-1.0f, 8);
    int32_t sum = Q_ADD(a, b);
    float fsum = from_q_format(sum, 8);
    CU_ASSERT_DOUBLE_EQUAL(fsum, 2.0, 1e-5);

    int32_t prod = Q_MUL(a, b, 8);
    float fprod = from_q_format(prod, 8);
    CU_ASSERT_DOUBLE_EQUAL(fprod, -3.0, 1e-5);
}
