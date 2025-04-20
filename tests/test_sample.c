#include "unity.h"

void setUp(void) {
    // Set up code before each test
}

void tearDown(void) {
    // Clean up code after each test
}

void test_SamplePass(void) {
    TEST_ASSERT_EQUAL_INT(1, 1);
}

void test_SampleFail(void) {
    TEST_ASSERT_EQUAL_INT(1, 2);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_SamplePass);
    // RUN_TEST(test_SampleFail);
    return UNITY_END();
}
