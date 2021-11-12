/*
 *
 * File Name: test_safedb.c
 * Author: Seree Rakwong
 * Date: Fri Nov  12 15:31:49 GMT 2021

 * Purpose:
 *   Test safedb_xxx() functions
 *
 */

#ifdef __UNIX__
#undef __UNIX__
#define __UNIX
#endif

#include "cuteng.h"
#include "test_safedb.h"
#include <string.h>
#include <time.h>
/* forward declaration */
#include <m_safedb.h>
#include <stdio.h>
#include <stdlib.h>

safedb_ptr dbptr = 0;
safedb_sel_ptr selptr = 0;


/*****************************************************************/

struct safedb_new_parms_s
{
    long expected;
};

struct safedb_new_parms_s safedb_new_parms[] =
{
  /* original is not a number */
  { 0 }
};


long test_safedb_new(char* alias, void* args)
{
    long lReturned = 0L;
    struct safedb_new_parms_s* params = 
        (struct safedb_new_parms_s*)args;

    dbptr = safedb_new();
    lReturned = (dbptr ? 1 : 0);
    return lReturned;
}



BEGIN_UNIT_TEST_PARAMS(test_safedb_100)
    ON_UNIT_TEST_PARAMS("safedb_new", test_safedb_new, 
        &safedb_new_parms[0], kEQ, 1L)
END_UNIT_TEST_PARAMS()

/*****************************************************************/

struct safedb_del_parms_s
{
    safedb_ptr db;
};

struct safedb_del_parms_s safedb_del_parms[] =
{
    { 0 },
  /* original is not a number */
  { 0 }
};


long test_safedb_del(char* alias, void* args)
{
    long lReturned = 0L;
    struct safedb_del_parms_s* params = 
        (struct safedb_del_parms_s*)args;

    safedb_del(dbptr);
    lReturned = 1;
    return lReturned;
}


BEGIN_UNIT_TEST_PARAMS(test_safedb_1000)
    ON_UNIT_TEST_PARAMS("safedb_del", test_safedb_del, 
        &safedb_del_parms[0], kEQ, 1L)
END_UNIT_TEST_PARAMS()


/*****************************************************************/

/*
long        safedb_open(safedb_ptr db, const char *dbfile);
*/

struct safedb_open_parms_s
{
    safedb_ptr db;
    const char *dbfile;
};

struct safedb_open_parms_s safedb_open_parms[] =
{
    { 0, "./student.db" },
  /* original is not a number */
  { 0 }
};


long test_safedb_open(char* alias, void* args)
{
    long lReturned = 0L;
    struct safedb_open_parms_s* params = 
        (struct safedb_open_parms_s*)args;

    lReturned = safedb_open(dbptr, params->dbfile);
    return lReturned;
}


BEGIN_UNIT_TEST_PARAMS(test_safedb_110)
    ON_UNIT_TEST_PARAMS("safedb_open", test_safedb_open, 
        &safedb_open_parms[0], kEQ, 0L)
END_UNIT_TEST_PARAMS()


/*****************************************************************/

/*
void        safedb_close(safedb_ptr db);
*/

struct safedb_close_parms_s
{
    safedb_ptr db;
};

struct safedb_close_parms_s safedb_close_parms[] =
{
    { 0 },
  /* original is not a number */
  { 0 }
};


long test_safedb_close(char* alias, void* args)
{
    long lReturned = 1L;
    struct safedb_close_parms_s* params = 
        (struct safedb_close_parms_s*)args;

    safedb_close(dbptr);
    return lReturned;
}


BEGIN_UNIT_TEST_PARAMS(test_safedb_990)
    ON_UNIT_TEST_PARAMS("safedb_close", test_safedb_close, 
        &safedb_close_parms[0], kEQ, 1L)
END_UNIT_TEST_PARAMS()

/*****************************************************************/

struct _student
{
    long    id;
    char    name[32];
    double  age;
    double  height;
    double  weight;
};
typedef struct _student student_t;

static long copy_student_callback(void* userptr, long col, char *value)
{
    student_t *student = (student_t*)userptr;
    switch(col)
    {
        case 0: student->id     = (value ? atol(value) : 0);        break;
        case 1: strcpy(student->name, (value ? value   : ""));      break;
        case 2: student->age    = (value ? atof(value) : 0);        break;
        case 3: student->height = (value ? atof(value) : 0);        break;
        case 4: student->weight = (value ? atof(value) : 0);        break;
    }
    return 0;
}


/*
safedb_sel_ptr  safedb_select(safedb_ptr db, const char *sqlcmd, safedb_copy_callback callback, long size);
*/

struct safedb_select_parms_s
{
    safedb_ptr db;
    const char *cmd;
    safedb_copy_callback callback;
    long size;
};

struct safedb_select_parms_s safedb_select_parms[] =
{
    { 0, "SELECT * FROM student;", copy_student_callback, sizeof(student_t) },
    { 0, "SELECT * FROM student ORDER BY name;", copy_student_callback, sizeof(student_t) },
    { 0, "SELECT * FROM student WHERE age > 30;", copy_student_callback, sizeof(student_t) },
  /* original is not a number */
  { 0 }
};


long test_safedb_select(char* alias, void* args)
{
    long lReturned = 0L;
    struct safedb_select_parms_s* params = 
        (struct safedb_select_parms_s*)args;

    printf("\n---[%s]---\n", alias);
    safedb_sel_ptr sel = safedb_select(dbptr, params->cmd, params->callback, params->size);
    if (sel)
    {
        int i = 0;
        student_t* student = 0;
        for (i = 0; i < sel->nrows; ++i)
        {
            student = (student_t*)safedb_sel_get(sel, i);
            printf("id=%ld, name=%s, age=%.2f, height=%.2f, weight=%.2f\n",
                student->id,
                student->name,
                student->age,
                student->height,
                student->weight);
        }
    }
    lReturned = (sel ? 1 : 0);
    safedb_sel_free(sel);
    
    return lReturned;
}


BEGIN_UNIT_TEST_PARAMS(test_safedb_200)
    ON_UNIT_TEST_PARAMS("SELECT * FROM student;", test_safedb_select, 
        &safedb_select_parms[0], kEQ, 1L)
    ON_UNIT_TEST_PARAMS("SELECT * FROM student ORDER BY name;", test_safedb_select, 
        &safedb_select_parms[1], kEQ, 1L)
    ON_UNIT_TEST_PARAMS("SELECT * FROM student WHERE age > 12;", test_safedb_select, 
        &safedb_select_parms[2], kEQ, 1L)
END_UNIT_TEST_PARAMS()

/*****************************************************************/

/*
1) SELECT COUNT(*) FROM STUDENT_TABLE;
2) SELECT AVG(age) FROM STUDENT_TABLE;
3) SELECT MAX(age) FROM STUDENT_TABLE;
3) SELECT SUM(age) FROM STUDENT_TABLE;

double          safedb_aggregate(safedb_ptr db, const char *sqlcmd);
*/

struct safedb_aggregate_parms_s
{
    safedb_ptr  db;
    const char* cmd;
};

struct safedb_aggregate_parms_s safedb_aggregate_parms[] =
{
    { 0, "SELECT COUNT(*) FROM student;" },
    { 0, "SELECT AVG(age) FROM student;" },
    { 0, "SELECT MAX(age) FROM student;" },
    { 0, "SELECT MIN(age) FROM student;" },
    { 0, "SELECT SUM(age) FROM student;" },
  /* original is not a number */
  { 0 }
};


long test_safedb_aggregate(char* alias, void* args)
{
    long lReturned = 1L;
    struct safedb_aggregate_parms_s* params = 
        (struct safedb_aggregate_parms_s*)args;
    double agg = 0.0;

    agg = safedb_aggregate(dbptr, params->cmd);
    printf("\n---[%s] = %.2f---\n", alias, agg);
    return lReturned;
}


BEGIN_UNIT_TEST_PARAMS(test_safedb_300)
    ON_UNIT_TEST_PARAMS("SELECT COUNT(*) FROM student;", test_safedb_aggregate, 
        &safedb_aggregate_parms[0], kEQ, 1L)
    ON_UNIT_TEST_PARAMS("SELECT AVG(age) FROM student;", test_safedb_aggregate, 
        &safedb_aggregate_parms[1], kEQ, 1L)
    ON_UNIT_TEST_PARAMS("SELECT MAX(age) FROM student;", test_safedb_aggregate, 
        &safedb_aggregate_parms[2], kEQ, 1L)
    ON_UNIT_TEST_PARAMS("SELECT MIN(age) FROM student;", test_safedb_aggregate, 
        &safedb_aggregate_parms[3], kEQ, 1L)
    ON_UNIT_TEST_PARAMS("SELECT SUM(age) FROM student;", test_safedb_aggregate, 
        &safedb_aggregate_parms[4], kEQ, 1L)
END_UNIT_TEST_PARAMS()

/*
CREATE TABLE student(
    id      INTEGER PRIMARY KEY AUTOINCREMENT,
    name    TEXT,
    age     REAL,
    height  REAL,
    weight  REAL,
    grade   TEXT
);
INSERT INTO student(name, age, height, weight, grade) values('micheal',     15, 170.2, 80.2, 'grade-12');
INSERT INTO student(name, age, height, weight, grade) values('tank',        16, 168.8, 60.3, 'grade-13');
INSERT INTO student(name, age, height, weight, grade) values('neo',         35, 180.6, 86.1, 'master');
INSERT INTO student(name, age, height, weight, grade) values('trinity',     30, 176.1, 70.2, 'master');
INSERT INTO student(name, age, height, weight, grade) values('morphoues',   45, 180.6, 90.2, 'bachaler');
*/



void test_safedb_run(char* title)
{
    FILE* fp = NULL;
    BEGIN_RUN_TEST_HTML(fp, "test_safedb_utr.html" )
        RUN_TEST_HTML(fp, "safedb_new",     test_safedb_100);
        RUN_TEST_HTML(fp, "safedb_open",    test_safedb_110);
        RUN_TEST_HTML(fp, "safedb_select",  test_safedb_200);
        RUN_TEST_HTML(fp, "safedb_select",  test_safedb_300);
        RUN_TEST_HTML(fp, "safedb_close",   test_safedb_990);
        RUN_TEST_HTML(fp, "safedb_del",     test_safedb_1000);
    END_RUN_TEST_HTML(fp)
}
