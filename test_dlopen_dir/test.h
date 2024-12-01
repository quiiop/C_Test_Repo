#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int n;
    unsigned char *buff;
}Test;

typedef struct{
    void (*p_func)(void);
}Cmdhandler;

typedef struct{
    char symbol[256];
}DynamicLoader_T;

extern Test g_test;
extern DynamicLoader_T loader;
extern Cmdhandler handler;

extern int init_test(void);
extern void set_Test(int n, unsigned char *buff);