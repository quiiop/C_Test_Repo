#include "test.h"

Test g_test;

int init_test(void)
{
    Test *p_test = &g_test;
    p_test->n = 100;
    p_test->buff = "hello";

    return 1;
}