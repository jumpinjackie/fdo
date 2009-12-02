#ifndef FDORDBMSSQLCOMMAND_H
#define FDORDBMSSQLCOMMAND_H        1
#ifdef _WIN32
#pragma once
#endif

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

#include "FdoRdbms.h"
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsCommand.h"
class DbiConnection;

// The SQLCommand supports the execution of a SQL statement against an
// underlying RDBMS. Two execute methods are provided to distingush between
// statements that return table data vs those that execute non query type
// operations.
class FdoRdbmsSQLCommand : public FdoRdbmsCommand<FdoISQLCommand>
{
    friend class FdoRdbmsConnection;

private:
    // Constructs a default instance of a SQLCommand.
    FdoRdbmsSQLCommand();

    // Constructs an instance of a SQLCommand using the specified arguments.
    FdoRdbmsSQLCommand(FdoIConnection* connection);
protected:
    // Default destructor for the SQLCommand command.
    virtual ~FdoRdbmsSQLCommand();

public:

    // Gets the SQL statement to be executed as a string.
    FDORDBMS_API const wchar_t* GetSQLStatement();

    // Sets the SQL statement to be executed as a string.
    FDORDBMS_API void SetSQLStatement(const wchar_t* value);

    // Executes the SQL statement against the connection object and returns
    // the number of rows affected.
    virtual FdoInt32 ExecuteNonQuery();

    // Executes the SQL statement against the connection object and returns
    // an ISQLDataReader.
    virtual FdoISQLDataReader* ExecuteReader();
private:
    wchar_t         *m_SqlString;
    FdoRdbmsConnection *mFdoConnection;
    DbiConnection   *m_DbiConnection;
};

#endif // FDORDBMSSQLCOMMAND_H
