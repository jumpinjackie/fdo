#ifndef FDOSMPHSQSCOLUMNBLOB_H
#define FDOSMPHSQSCOLUMNBLOB_H      1
/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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

// Represents a SqlServer BLOB type column.
class FdoSmPhSqsColumnBLOB :
    public FdoSmPhColumnBLOB, public FdoSmPhSqsColumn
{
public:
    FdoSmPhSqsColumnBLOB(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        FdoSmPhDbObject* parentObject,
        bool bNullable,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* reader = NULL
    ) :
        FdoSmPhSqsColumn (reader),
        FdoSmPhColumn    ( columnName, L"IMAGE", elementState, parentObject, bNullable, rootColumnName)
    {}

    virtual int GetRdbType()
    {
        return RDBI_BLOB_REF;
    }

    ~FdoSmPhSqsColumnBLOB(void) {}
private:

};

typedef FdoPtr<FdoSmPhSqsColumnBLOB> FdoSmPhSqsColumnBLOBP;

#endif
