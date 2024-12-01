#include "test.h"

void func1(void){
    printf("[func1] hello kuo\n");
}

Cmdhandler handler = {
    .p_func = func1
};