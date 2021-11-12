/*
 * File Name: cuteng.c
 * Author: Seree Rakwong
 * Date: 12-NOV-2021
 */
#include "cuteng.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* op_to_str[] =
{
    "EQ ",
    "LT ",
    "LE ",
    "GT ",
    "GE ",
    "NE ",
    "AND",
    "OR ",
    "NOT",
    "XOR",
    "EQS",
    "LTS",
    "GTS",
    "NES",
    0
};

void run_test( char *title, unit_test_t* test_cases )
{
    int  i      = 0;
    long rc     = 0;
    long rc2    = 0;
    results_t   results = { 0, 0 };
    char line_break[] = "---------------------------------------------------"
                        "---------------------------------------------------";
    /* title */
    printf( "\n\n" );
    printf( "%s\n", line_break );
    printf( "%*s\n", ( strlen( line_break ) + strlen( title ) ) / 2, title );
    printf( "%s\n", line_break );
    printf( "%4s %-64s %s  %8s %8s %s\n", "No", "Test cases", "Op", "Expected", "Returned", "Result" );
    printf( "%s\n", line_break );
    for ( ; ; ++i )
    {
        if ( !test_cases[i].func )
            break;

        rc = test_cases[i].func( test_cases[i].alias );
        switch ( test_cases[i].operation )
        {
            case kEQ:  rc2 = ( rc == test_cases[i].expected ); break;
            case kLT:  rc2 = ( rc <  test_cases[i].expected ); break;
            case kLE:  rc2 = ( rc <= test_cases[i].expected ); break;
            case kGT:  rc2 = ( rc >  test_cases[i].expected ); break;
            case kGE:  rc2 = ( rc >= test_cases[i].expected ); break;
            case kNE:
            default:   rc2 = ( rc != test_cases[i].expected ); break;
        }

        printf( "%4d %-64s %s %8ld %8ld [ %s ]\n", 
                (i+1),
                test_cases[i].alias,
                op_to_str[test_cases[i].operation], 
                test_cases[i].expected,
                rc,
                ( rc2 == 0 ? "failed" : "PASSED" ) );
        if ( rc2 == 0 )
        {
            ++results.failed_cases;
        }
        else
        {
            ++results.passed_cases;
        }
    }
    printf( "%s\n", line_break );
    printf( "[PASSED: %4d] | [FAILED: %4d]\n", results.passed_cases, results.failed_cases );
    printf( "%s\n", line_break );
}

void run_test_params( char *title, unit_test_parms_t* test_cases )
{
    int  i      = 0;
    long rc     = 0;
    long rc2    = 0;
    char        alias[64];
    results_t   results = { 0, 0 };
    char line_break[] = "---------------------------------------------------"
                        "---------------------------------------------------";
    /* title */
    printf( "\n\n" );
    printf( "%s\n", line_break );
    printf( "%*s\n", ( strlen( line_break ) + strlen( title ) ) / 2, title );
    printf( "%s\n", line_break );
    printf( "%4s %-64s %s  %8s %8s %s\n", "No", "Test cases", "Op", "Expected", "Returned", "Result" );
    printf( "%s\n", line_break );
    for ( ; ; ++i )
    {
        if ( !test_cases[i].func )
            break;

        rc = test_cases[i].func( test_cases[i].alias, test_cases[i].args );
        switch ( test_cases[i].operation )
        {
            case kEQ:  rc2 = ( rc == test_cases[i].expected ); break;
            case kLT:  rc2 = ( rc <  test_cases[i].expected ); break;
            case kLE:  rc2 = ( rc <= test_cases[i].expected ); break;
            case kGT:  rc2 = ( rc >  test_cases[i].expected ); break;
            case kGE:  rc2 = ( rc >= test_cases[i].expected ); break;
            case kNE:
            default:   rc2 = ( rc != test_cases[i].expected ); break;
        }

        strncpy( alias, test_cases[i].alias, 63 );
        printf( "%4d %-64s %s %8ld %8ld [ %s ]\n", 
                (i+1),
                alias,
                op_to_str[test_cases[i].operation], 
                test_cases[i].expected,
                rc,
                ( rc2 == 0 ? "failed" : "PASSED" ) );
        if ( rc2 == 0 )
        {
            ++results.failed_cases;
        }
        else
        {
            ++results.passed_cases;
        }
    }
    printf( "%s\n", line_break );
    printf( "[PASSED: %4d] | [FAILED: %4d]\n", results.passed_cases, results.failed_cases );
    printf( "%s\n", line_break );
}

void run_test_to_file( FILE* fp, char *title, unit_test_parms_t* test_cases )
{
    int  i      = 0;
    long rc     = 0;
    long rc2    = 0;
    char        alias[64];
    results_t   results = { 0, 0 };
    char line_break[] = "---------------------------------------------------"
                        "---------------------------------------------------";
    /* title */
    fprintf( fp, "\n\n" );
    fprintf( fp, "%s\n", line_break );
    fprintf( fp, "%*s\n", ( strlen( line_break ) + strlen( title ) ) / 2, title );
    fprintf( fp, "%s\n", line_break );
    fprintf( fp, "%4s %-64s %s  %8s %8s %s\n", "No", "Test cases", "Op", "Expected", "Returned", "Result" );
    fprintf( fp, "%s\n", line_break );
    for ( ; ; ++i )
    {
        if ( !test_cases[i].func )
            break;

        rc = test_cases[i].func( test_cases[i].alias, test_cases[i].args );
        switch ( test_cases[i].operation )
        {
            case kEQ:  rc2 = ( rc == test_cases[i].expected ); break;
            case kLT:  rc2 = ( rc <  test_cases[i].expected ); break;
            case kLE:  rc2 = ( rc <= test_cases[i].expected ); break;
            case kGT:  rc2 = ( rc >  test_cases[i].expected ); break;
            case kGE:  rc2 = ( rc >= test_cases[i].expected ); break;
            case kNE:
            default:   rc2 = ( rc != test_cases[i].expected ); break;
        }

        strncpy( alias, test_cases[i].alias, 63 );
        fprintf( fp, "%4d %-64s %s %8ld %8ld [ %s ]\n", 
                (i+1),
                alias,
                op_to_str[test_cases[i].operation], 
                test_cases[i].expected,
                rc,
                ( rc2 == 0 ? "failed" : "PASSED" ) );
        if ( rc2 == 0 )
        {
            ++results.failed_cases;
        }
        else
        {
            ++results.passed_cases;
        }
    }
    fprintf( fp, "%s\n", line_break );
    fprintf( fp, "[PASSED: %4d] | [FAILED: %4d]\n", results.passed_cases, results.failed_cases );
    fprintf( fp, "%s\n", line_break );
    fflush( fp );
    
    printf( "\n\n" );
    printf( "%s\n", line_break );
    printf( "%*s\n", ( strlen( line_break ) + strlen( title ) ) / 2, title );
    printf( "%s\n", line_break );
    printf( "[PASSED: %4d] | [FAILED: %4d]\n", results.passed_cases, results.failed_cases );
    printf( "%s\n", line_break );
}

void run_test_to_html( FILE* fp, char *title, unit_test_parms_t* test_cases )
{
    int  i      = 0;
    long rc     = 0;
    long rc2    = 0;
    results_t   results = { 0, 0 };
    char line_break[] = "---------------------------------------------------"
                        "---------------------------------------------------";
    /* title */
    fprintf( fp, "<table id=\"reports\"><tr><th colspan=\"6\">%s</th></tr>", title );
    fprintf( fp, "<tr><th>%s</th><th>%s</th><th>%s</th><th>%s</th><th>%s</th><th>%s</th></tr>", "No", "Test cases", "Op", "Expected", "Returned", "Result" );
    
    for ( ; ; ++i )
    {
        if ( !test_cases[i].func )
            break;

        rc = test_cases[i].func( test_cases[i].alias, test_cases[i].args );
        switch ( test_cases[i].operation )
        {
            case kEQ:  rc2 = ( rc == test_cases[i].expected ); break;
            case kLT:  rc2 = ( rc <  test_cases[i].expected ); break;
            case kLE:  rc2 = ( rc <= test_cases[i].expected ); break;
            case kGT:  rc2 = ( rc >  test_cases[i].expected ); break;
            case kGE:  rc2 = ( rc >= test_cases[i].expected ); break;
            case kNE:
            default:   rc2 = ( rc != test_cases[i].expected ); break;
        }

        fprintf( fp, "<tr><td align=\"right\">%d</td><td>%s</td><td>%s</td><td align=\"center\">%ld</td><td align=\"center\">%ld</td><td>%s</td></tr>", 
                (i+1),
                test_cases[i].alias,
                op_to_str[test_cases[i].operation], 
                test_cases[i].expected,
                rc,
                ( rc2 == 0 ? "<span style=\"color:red\">failed</span>" : "<span style=\"color:green\">PASSED</span>" ) );
        if ( rc2 == 0 )
        {
            ++results.failed_cases;
        }
        else
        {
            ++results.passed_cases;
        }
    }
    fprintf( fp, 
        "<tr><th>PASSED:</th><td colspan=\"5\">%d</td></tr>" 
        "<tr><th class=\"failed\">FAILED:</th><td colspan=\"5\">%d</td></tr>", 
        results.passed_cases, results.failed_cases );
    fprintf( fp, "</table><br>" );
    fflush( fp );
    
    printf( "\n\n" );
    printf( "%s\n", line_break );
    printf( "%*s\n", ( strlen( line_break ) + strlen( title ) ) / 2, title );
    printf( "%s\n", line_break );
    printf( "[PASSED: %4d] | [FAILED: %4d]\n", results.passed_cases, results.failed_cases );
    printf( "%s\n", line_break );
}

void dump_bin( FILE* fp, char* bin, int len )
{
    int i = 0;
    int j = 0;

    /* parameters or buffer */
    for (; i < (int)len; i += 16 )
    {
        /* address */
        fprintf( fp, "%08x ", i );

        /* hex */
        for( j = 0; j < 16; ++j )
        {
            if ( i+j >= (int)len )
            {
                if (j == 8 )
                {
                    fprintf( fp, "    " );
                }
                else
                {
                    fprintf( fp, "   " );
                }
            }
            else
            {
                if (j == 8 )
                {
                    fprintf( fp, "  %02X", bin[i+j] & 0xff );
                }
                else
                {
                    fprintf( fp, " %02X", bin[i+j] & 0xff );
                }
            }
        }

        /* characters */
        fprintf( fp, "  " );

        for( j = 0; j < 16; ++j )
        {
            if ( i+j >= (int)len )
            {
                fprintf( fp, " " );
            }
            else
            {
                if ( bin[i+j] < 0x20 )
                {
                    fprintf( fp, "." );
                }
                else
                {
                    fprintf( fp, "%c", bin[i+j] & 0xff );
                }
            }
        }

        /* next line */
        fprintf( fp, "\n" );
    }

}

