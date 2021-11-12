/*
 * File name: m_safedb.c
 * Author: Seree Rakwong
 * Date: 10-NOV-2021
 *
 * NOTE:
 *  Date        By      Reason
 *  =========   ====    ================================================
 *
 */
#include <msesysid.h>
#include <msedef.h>
#include <stidef.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "m_safedb.h"


#ifdef __cplusplus
extern "C"
{
#endif

static long safedb_errno = SAFEDB_OK;

safedb_ptr safedb_new()
{
    safedb_ptr db = (safedb_ptr)malloc(sizeof(safedb_t));
    safedb_errno = SAFEDB_OK;
    if (db)
    {
        db->db = 0;
    }
    else
    {
        safedb_errno = SAFEDB_EMEM;
    }
    return db;
}

void safedb_del(safedb_ptr _db)
{
    safedb_errno = SAFEDB_OK;
    if (_db)
    {
        safedb_close(_db);
        free(_db);
    }
}

long safedb_open(safedb_ptr _db, const char *_dbfile)
{
    long rc = 0;
    sqlite3 *db;

    safedb_errno = SAFEDB_OK;
    if (_db->db)
    {
        return rc;
    }
    rc = sqlite3_open_v2(_dbfile, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0);
    if (rc != SQLITE_OK)
    {
        sqlite3_close(db);
        safedb_errno = SAFEDB_ESQL;
        return rc;
    }
    _db->db = db;
    return rc;
}

void safedb_close(safedb_ptr _db)
{
    safedb_errno = SAFEDB_OK;
    if (_db->db)
    {
        sqlite3_close(_db->db);
        _db->db = 0;
    }
}

long safedb_exec(safedb_ptr _db, const char* _sqlcmd, safedb_exec_callback _callback, void* _userptr)
{
    long rc = 0;
    char *errmsg = 0;
    safedb_errno = SAFEDB_OK;
    if (!_db->db)
    {
        safedb_errno = SAFEDB_ERROR;
        return -1;
    }
    rc = sqlite3_exec(_db->db, _sqlcmd, _callback, _userptr, &errmsg);
    if (rc != SQLITE_OK)
    {
        safedb_errno = SAFEDB_ESQL;
        sqlite3_free(errmsg);
    }
    return rc;
}

safedb_sel_ptr  safedb_select(safedb_ptr _db, const char *_sqlcmd, safedb_copy_callback _callback, long _size)
{
    sqlite3_stmt *stmt;
    safedb_sel_ptr sel = 0;
    int rc = 0;
    long rc2 = 0;
    long ncols = 0;
    long nrows = 0;
    long col = 0;
    long row = 0;
    char *item = 0;
    void *items = 0;
    long done = 0;
    char *value = 0;
    long size = _size;

    safedb_errno = SAFEDB_OK;
L000_INIT:
    if (!_callback || _size <= 0)
    {
        /*required callback and size correctly*/
        safedb_errno = SAFEDB_ERROR;
        return 0;
    }

L100_PREPARE_SQL:
    rc = sqlite3_prepare_v2(_db->db, _sqlcmd, -1, &stmt, 0);
    /*rc = sqlite3_prepare_v3(_db->db, _sqlcmd, -1, 0, &stmt, 0);*/
    if (rc != SQLITE_OK)
    {
        sqlite3_finalize(stmt);
        safedb_errno = SAFEDB_ESQL;
        return 0;
    }

L200_COUNT_ROWS_RETURNED:
    /*count rows*/
    do
    {
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_ROW)
        {
            break;
        }
        ++nrows;
    } while (1);
    sqlite3_reset(stmt);
    
L300_INIT_AN_ARRAY:
    /*create a pointer to an array of records*/
    ncols = sqlite3_column_count(stmt);
    size = _size * nrows;
    if (nrows > 0)
    {
        items = malloc(size);
        if (items)
        {
            memset(items, 0, size);
        }
    }

L400_ALLOC_NEW_OBJECT_RETURNED:
    sel = (safedb_sel_ptr)malloc(sizeof(safedb_sel_t));
    if (!sel)
    {
        free(items);
        safedb_errno = SAFEDB_EMEM;
        return 0;
    }
    sel->nrows  = nrows;
    sel->items  = items;
    sel->size   = _size;
    if (0 == nrows)
    {
        sqlite3_finalize(stmt);
        return sel;
    }

L500_COPY_DATA_BY_USER_CALLBACK:
    row = 0;
    do
    {
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_ROW)
        {
            done = 1;
            break;
        }
        item = (char*)(items) + (row * _size);
        for (col = 0; col < ncols; ++col)
        {
            value = (char*)sqlite3_column_text(stmt, col);
            rc2 = _callback(item, col, value);
            if (rc2 != 0)
            {
                done = 1;
                break;
            }
        }
        ++row;
    } while ((row < nrows) && (0 == done));
    
L600_RELEASE_SQL_STATEMENT:
    sqlite3_finalize(stmt);

L1000_EXIT_FUNCTION:
    return sel;
}

void* safedb_sel_get(safedb_sel_ptr _selptr, long _row)
{
    void* ptr = 0;
    safedb_errno = SAFEDB_ERROR;
    if (_row >= 0 && _row < _selptr->nrows)
    {
        ptr = (void*)((char*)(_selptr->items) + (_row * _selptr->size));
        safedb_errno = SAFEDB_OK;
    }
    return ptr;
}

void safedb_sel_free(safedb_sel_ptr _selptr)
{
    safedb_errno = SAFEDB_OK;
    if (_selptr)
    {
        free(_selptr->items);
        free(_selptr);
    }
}

double safedb_aggregate(safedb_ptr _db, const char *_sqlcmd)
{
    sqlite3_stmt *stmt;
    double agg = 0.0;
    int rc = SQLITE_OK;

    safedb_errno = SAFEDB_OK;
    rc = sqlite3_prepare_v2(_db->db, _sqlcmd, -1, &stmt, 0);
    /*rc = sqlite3_prepare_v3(_db->db, _sqlcmd, -1, 0, &stmt, 0);*/
    if (rc != SQLITE_OK)
    {
        safedb_errno = SAFEDB_ESQL;
        return 0;
    }
    do
    {
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_ROW)
        {
            break;
        }
        agg = sqlite3_column_double(stmt, 0);
    } while (0);
    
    return agg;
}

#ifdef __cplusplus
}
#endif
