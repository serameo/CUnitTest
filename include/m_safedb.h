/*
 * File name: m_safedb.h
 * Author: Seree Rakwong
 * Date: 10-NOV-2021
 * Purpose:
 *  SafeDB object is to map global or file by using sqlite3
 *
 * NOTE:
 *  Date        By      Reason
 *  =========   ====    ================================================
 *
 */
#ifndef __M_SAFEDB_H__
#define __M_SAFEDB_H__

#include <sqlite3.h>

#include "m_safedb.h"

#ifdef __cplusplus
extern "C" {
#endif


struct _safedb
{
    sqlite3 *db;
};
typedef struct _safedb safedb_t;
typedef struct _safedb *safedb_ptr;

safedb_ptr  safedb_new();
void        safedb_del(safedb_ptr db);

long        safedb_open(safedb_ptr db, const char *dbfile);
void        safedb_close(safedb_ptr db);

/*
CREATE TABLE student(
    id      INTEGER PRIMARY KEY AUTOINCREMENT,
    name    TEXT,
    age     REAL,
    height  REAL,
    weight  REAL
    );

INSERT INTO student(name,age,height,weight) VALUES('michael',12.5,157.2,58.2);
INSERT INTO student(name,age,height,weight) VALUES('neo',12.3,158.2,52.3);
INSERT INTO student(name,age,height,weight) VALUES('trinity',12.6,162.2,55.8);
INSERT INTO student(name,age,height,weight) VALUES('morphoues',13.5,163.9,65.3);
INSERT INTO student(name,age,height,weight) VALUES('oracle',33.8,160.9,60.3);
INSERT INTO student(name,age,height,weight) VALUES('tank',10.5,152.9,45.3);

UPDATE student SET name='smith' WHERE id=1;

DELETE student WHERE id=6;
*/
typedef int (*safedb_exec_callback)(void* userptr, int argc, char **argv, char **name);
long        safedb_exec(safedb_ptr db, const char* sqlcmd, safedb_exec_callback callback, void* userptr);

/*
Example to call copy from callback function
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
*/
typedef long (*safedb_copy_callback)(void* userptr, long col, char *value);

struct _safedb_select
{
    long    size;   /*size of an item per record*/
    long    nrows;  /*a number of rows*/
    void*   items;  /*an array of items*/
};
typedef struct _safedb_select safedb_sel_t;
typedef struct _safedb_select *safedb_sel_ptr;
/*
safedb_select() would return an array of records
SQL Example:
    const char *sqlcmd = "SELECT * FROM STUDENT_TABLE WHERE AGE > 7;";

Return:
    safedb_sel_ptr if sqlcmd was successfully called. Please double-check nrows and items if there
    is no row returned.
    Otherwise, NULL if sqlcmd is incorrect statement. Such as no table in database and so on.

Example:
    safedb_sel_ptr selptr = safedb_select(db, sqlcmd, copy_student_callback, sizeof(student_t));
    if (selptr)
    {
        if (selptr->nrows > 0 && selptr->items)
        {
            printf("data found\n");
        }
        else
        {
            printf("data not found\n");
        }
    }
    else
    {
        printf("statement is incorrect or table not found\n");
    }
*/
safedb_sel_ptr  safedb_select(safedb_ptr db, const char *sqlcmd, safedb_copy_callback callback, long size);
/*
safedb_sel_get() to read a specific record with a zero base
Example:
    student_t *student = safedb_sel_get(selptr, 0);
*/
void*           safedb_sel_get(safedb_sel_ptr selptr, long row);
/*
safedb_sel_free() to release safedb_sel_ptr object
*/
void            safedb_sel_free(safedb_sel_ptr selptr);
/*
safedb_aggregate() would only one aggregate SQL function
SQL Example:
    const char *sqlcmd1 = "SELECT COUNT(*) FROM student;";
    const char *sqlcmd2 = "SELECT AVG(age) FROM student;";
    const char *sqlcmd3 = "SELECT MAX(age) FROM student;";
    const char *sqlcmd4 = "SELECT SUM(age) FROM student;";

Example:
    long    count_students  = (long)safedb_aggregate(db, sqlcmd1);
    double  avg_age         = safedb_aggregate(db, sqlcmd2);
    double  max_age         = safedb_aggregate(db, sqlcmd3);
    double  sum_age         = safedb_aggregate(db, sqlcmd4);
*/
double          safedb_aggregate(safedb_ptr db, const char *sqlcmd);

#ifdef __cplusplus
}
#endif

#endif /* __M_SAFEDB_H__ */
