#include "test.h"

void set_Test(int n, unsigned char *buff)
{
    Test *p_test = &g_test;
    p_test->n = n;
    p_test->buff = buff;
}

