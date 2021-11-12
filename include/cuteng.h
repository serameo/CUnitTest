#ifndef _CUNITTEST_ENGINE_H_
#define _CUNITTEST_ENGINE_H_
/*
 * File Name: cuteng.h
 * Author: Seree Rakwong
 * Date: 12-NOV-2021
 */
#include <stdio.h>

enum const_operation_enum
{
    kEQ = 0,    /* == */
    kLT,        /* <  */
    kLE,        /* <= */
    kGT,        /* >  */
    kGE,        /* >= */
    kNE,        /* != */
    kAND,       /* && */
    kOR,        /* || */
    kNOT,       /* !  */
    kXOR,       /* ^  */
    kEQS,       /* strcmp == 0 */
    kLTS,       /* strcmp <  0 */
    kGTS,       /* strcmp >  0 */
    kNES        /* strcmp != 0 */
};

typedef long    (*unit_test_fn)(char*);
typedef long    (*unit_test_parms_fn)(char*, void*);

struct unit_test_s
{
    char            alias[64];
    unit_test_fn    func;
    int             operation;
    long            expected;
};
typedef struct unit_test_s unit_test_t;

struct unit_test_parms_s
{
    char*                   alias;
    unit_test_parms_fn      func;
    void*                   args;
    int                     operation;
    long                    expected;
};
typedef struct unit_test_parms_s unit_test_parms_t;

struct results_s
{
    int passed_cases;
    int failed_cases;
};
typedef struct results_s results_t;

void run_test( char *title, unit_test_t* test_cases );
void run_test_params( char *title, unit_test_parms_t* test_cases );
void run_test_to_file( FILE* fp, char *title, unit_test_parms_t* test_cases );
void run_test_to_html( FILE* fp, char *title, unit_test_parms_t* test_cases );

#define BEGIN_UNIT_TEST( test_case, test_type ) \
test_type test_case[] = \
{

#define BEGIN_UNIT_TEST_INT( test_case ) \
unit_test_t test_case[] = \
{

#define ON_UNIT_TEST( alias, func, op, val ) \
    { alias, func, op, val },

#define END_UNIT_TEST() \
    { "", 0, 0, 0L }   /* end of test cases */ \
};

#define BEGIN_UNIT_TEST_PARAMS( test_case ) \
unit_test_parms_t test_case[] = \
{

#define ON_UNIT_TEST_PARAMS( alias, func, args, op, val ) \
    { alias, func, args, op, val },
    

#define END_UNIT_TEST_PARAMS() \
    { "", 0, 0, 0, 0L }   /* end of test cases */ \
};


#define RUN_TEST( alias, test_case ) \
run_test( alias, test_case );

#define RUN_TEST_PARAMS( alias, test_case ) \
run_test_params( alias, test_case );

#define RUN_TEST_FILE( fp, alias, test_case ) \
run_test_to_file( fp, alias, test_case );

#define RUN_TEST_HTML( fp, alias, test_case ) \
run_test_to_html( fp, alias, test_case );


#define BEGIN_RUN_TEST_FILE(fp, fname) \
{ \
    char buffer[64]; \
    if ( fp == NULL ) \
    { \
        sprintf( buffer, "./log/%s", fname ); \
        fp = fopen( buffer, "w+" ); \
    }

#define END_RUN_TEST_FILE(fp) \
    fflush( fp ); \
    fclose( fp ); \
}

#define BEGIN_RUN_TEST_HTML(fp, fname) \
{ \
    char buffer[64]; \
    if ( fp == NULL ) \
    { \
        sprintf( buffer, "./log/%s", fname ); \
        fp = fopen( buffer, "w+" ); \
    } \
    fprintf( fp, "<!DOCTYPE html><html><body>" ); \
    fprintf( fp, "<head><style>" \
                 "#reports {font-family:\"Trebuchet MS\",Arial,Helvetica,sans-serif;border-collapse:collapse;width:100%%;}" \
                 "#reports td,#reports th{border:1px solid #ddd;padding:8px;}" \
                 "#reports tr:nth-child(even){background-color:#f2f2f2;}" \
                 "#reports tr:hover{background-color:#ddd;}" \
                 "#reports th{padding-top:12px;padding-bottom:12px;text-align:center;background-color:#4CAF50;color:white;}" \
                 "#reports th.passed{padding-top:12px;padding-bottom:12px;text-align:center;background-color:#4CAF50;color:white;}" \
                 "#reports th.failed{padding-top:12px;padding-bottom:12px;text-align:center;background-color:#AF4C50;color:white;}" \
                 "</style></head>" ); \
    fflush( fp );

#define END_RUN_TEST_HTML(fp) \
    fprintf( fp, "</body></html>" ); \
    fflush( fp ); \
    fclose( fp ); \
}

void dump_bin( FILE* fp, char* bin, int len );

#endif /* _CUNITTEST_ENGINE_H_ */
