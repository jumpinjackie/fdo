/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "stdafx.h"

#include "fre_cursor.h"
#include "sql.h"

static void print_st_error (MYSQL_STMT *stmt, const char *msg)
{
    int err;
    const char *error;

    if (stmt && mysql_stmt_errno (stmt))
    {
        err = mysql_stmt_errno (stmt);
        error = mysql_stmt_error (stmt);
        printf ("\n[%d] %s\n", err, error);
    }
    else if (msg)
        fprintf(stderr, " [MySQL] %s\n", msg);
}

int mysql_sql (
    mysql_context_def *context,
    char *cursor,
    char *sql,
    int defer, /* not used */
    char *verb,
    void *ptree, /* dbi_prse_tree_def - not used */
    char *cursor_coc) /* not used */
{
    MYSQL *mysql;
    mysql_cursor_def* curs;
    MYSQL_STMT *statement;
    int result;
    int ret;

    if (-1 == context->mysql_current_connect)
        ret = RDBI_NOT_CONNECTED;
    else
    {
        mysql = context->mysql_connections[context->mysql_current_connect];
        if ((MYSQL*)NULL == mysql)
            ret = RDBI_NOT_CONNECTED;
        else
        {
            curs = (mysql_cursor_def*)cursor;
            if ((mysql_cursor_def*)NULL != curs)
            {
                statement = curs->statement;
                if ((MYSQL_STMT *)NULL != statement)
                {
                    mysql_fre_binds (curs);
                    result = mysql_stmt_prepare (statement, sql, (unsigned long)strlen (sql));
                    if (0 != result)
                    {
                        print_st_error (statement, "prepare failed");
                        /* If the prepare operation was unsuccessful, the
                        error message can be obtained by calling mysql_stmt_error()
                        or printed with print_st_error().
                        CR_COMMANDS_OUT_OF_SYNC Commands were executed in an improper order. 
                        CR_OUT_OF_MEMORY Out of memory. 
                        CR_SERVER_GONE_ERROR The MySQL server has gone away. 
                        CR_SERVER_LOST The connection to the server was lost during the query 
                        CR_UNKNOWN_ERROR An unknown error occurred. 
                        */
                        ret = RDBI_GENERIC_ERROR;
                    }
                    else
                        ret = RDBI_SUCCESS;
                }
                else
                    ret = RDBI_GENERIC_ERROR;
            }
        }
    }

    return (ret);
}
