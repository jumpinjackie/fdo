#ifndef FDOSMPHMYSQLCOLUMNBLOB_H
#define FDOSMPHMYSQLCOLUMNBLOB_H      1
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

#ifdef _WIN32
#pragma once
#endif

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnBLOB.h>

// Represents a MySql BLOB type column.
class FdoSmPhMySqlColumnBLOB :
    public FdoSmPhColumnBLOB, public FdoSmPhMySqlColumn
{
public:
    FdoSmPhMySqlColumnBLOB(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        FdoSmPhDbObject* parentObject,
        bool bNullable,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* reader = NULL
    ) :
        FdoSmPhMySqlColumn (reader),
        FdoSmPhColumn    ( columnName, L"LONGBLOB", elementState, parentObject, bNullable, rootColumnName)
    {}

    virtual int GetRdbType()
    {
        return RDBI_BLOB_REF;
    }

    ~FdoSmPhMySqlColumnBLOB(void) {}
private:

};

typedef FdoPtr<FdoSmPhMySqlColumnBLOB> FdoSmPhMySqlColumnBLOBP;

#endif
