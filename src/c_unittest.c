/*
 *
 * File Name: c_unittest.c
 * Author: Seree Rakwong
 * Date: 12-NOV-2021
 * Purpose:
 *   Main entry to run C unit tests
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "c_unittest.h"


typedef void (*run_test_fn)(char*);
struct test_function_s
{
    run_test_fn func;
    char title[64];
};

struct test_function_s c_functions[] =
{
    /* ADD THE NEW RUN FUNCTION HERE */
    { test_safedb_run, "test_safedb" },
    /* DO NOT DELETE THE LAST ELEMENT */
    { 0, "" }
};

int main(int argc, char* argv[])
{
    int i = 0;
    printf("ETS Unit Test running...\n");

    for(;; ++i)
    {
        if (!c_functions[i].func)
        {
            break;
        }
        c_functions[i].func(c_functions[i].title);
    }
    printf("\nCompleted!!\n");
    return 0;
}
