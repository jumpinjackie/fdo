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
 */

#ifndef ARCSDELOCKUTILITY_H
#define ARCSDELOCKUTILITY_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "Fdo.h"

class ArcSDELockUtility
{

public:

    /// <summary>Check if rows in a table can be locked.</summary>
    /// <param name="connection">The connection to use.</param>
    /// <param name="table">Input the table name to check for lockability.</param>
    /// <param name="row_id_column">Output the row id column name. Should be CHAR[SE_MAX_COLUMN_LEN].</param>
    static bool IsLockable (SE_CONNECTION connection, const CHAR* table, CHAR* row_id_column = NULL);

    /// <summary>Make sure the given table is lock enabled.</summary>
    /// <param name="connection">The connection to use.</param>
    /// <param name="table">Input the table name to lock enable.</param>
    /// <param name="row_id_column">Output the row id column name. Should be CHAR[SE_MAX_COLUMN_LEN].</param>
    static void LockEnable (SE_CONNECTION connection, const CHAR* table, CHAR* row_id_column = NULL);

    /// <summary>Gather up the conflicts from the stream.</summary>
    /// Gets the row id given by column at the position given by column_number,
    /// and adds each of them to the lock conflict reader.
    /// <param name="stream">The stream that has "select row_id from..." just executed on it.</param>
    /// <param name="column">Input the column name which is being fetched.</param>
    /// <param name="column_number">Input the column number, columns start with the number 1.</param>
    /// <param name="reader">Input the reader to load with the row id values.</param>
    static void GatherConflicts (SE_STREAM stream, const CHAR* column, int column_number, ArcSDELockConflictReader* reader);

    /// <summary>Get the name of the lock table for a given table.</summary>
    /// <param name="locktable">Output the lock table name.
    /// Should be at least SE_QUALIFIED_TABLE_NAME characters in size.</param>
    /// <param name="connection">The connection to use.</param>
    /// <param name="table">Input the table name to lock the lock table for.</param>
    static void LockTableName (CHAR* locktable, ArcSDEConnection* connection, const CHAR* table);

    /// <summary>Perform an SQL command to transactionally lock the lock table (select for update).</summary>
    /// <param name="connection">The connection to use.</param>
    /// <param name="table">Input the table name to lock the lock table for.</param>
    static FdoISQLDataReader* LockLockTable (ArcSDEConnection* connection, const CHAR* table);

    /// <summary>Set up a temporary log file for the given table.</summary>
    /// <param name="log">Output the log file created.</param>
    /// <param name="connection">The connection to use.</param>
    /// <param name="table">Input the table name to log for.</param>
    static void MakeLog (SE_LOG* log, SE_CONNECTION connection, const CHAR* table);

    /// <summary>Get the name of the log file from a log object.</summary>
    /// <param name="logfile">Output the log file name.</param>
    /// <param name="connection">The connection to use.</param>
    /// <param name="log">Input the log to get the name of.</param>
    static void GetLogFile (CHAR* logfile, SE_CONNECTION connection, SE_LOG log);

    /// <summary>Get the list of ids from a log file.</summary>
    /// <param name="connection">The connection to use.</param>
    /// <param name="logfile">Input the log file name.</param>
    /// <param name="count">Input the number of row_ids to get, output the number of row_ids recieved.</param>
    static LONG* GetRowIdList (SE_CONNECTION connection, CHAR* logfile, LONG &count);
};

#endif // ARCSDELOCKUTILITY_H

