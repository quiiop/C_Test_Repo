#include <dlfcn.h>
#include "test.h"

static void *handle = NULL;

int use_libtest(void)
{
    handle = dlopen("./libtest.so", RTLD_LAZY);
    if (!handle){
        fprintf(stderr, "%s\n", dlerror());
        printf("dlopen err\n");
        return 0;
    }

    return 1;
}

DynamicLoader_T loader = {
    .symbol = "handler"
};

int main(void)
{
    Test *p_test = NULL;
    //Test *p_test = &g_test;
    void *func = NULL;

    if (use_libtest()){
        func = dlsym(handle, "init_test");

        if ( ((int (*)(void))func)() ){
            func = dlsym(handle, "set_Test");
            ((void (*)(int, unsigned char *))func)(300, "hello kuo v2");
        }

        func = dlsym(handle, "g_test");
        p_test = (Test *)func;
        printf("Hi Kuo %d , %s\n", p_test->n, p_test->buff);    
    }   

    // char *SymbolName = loader.symbol;
    // printf("%s\n", SymbolName);
    func = dlsym(handle, (char *)loader.symbol);

    Cmdhandler *p_handler = (Cmdhandler *)func;
    p_handler->p_func();

    /*if (init_test()){
        set_Test(200, "hello kuo");
        printf("%d , %s\n", p_test->n, p_test->buff);
    }else{
        printf("init err\n");
    }*/
}


