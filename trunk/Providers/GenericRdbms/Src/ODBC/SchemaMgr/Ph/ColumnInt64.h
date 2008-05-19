#ifndef FDOSMPHODBCCOLUMNINT64_H
#define FDOSMPHODBCCOLUMNINT64_H       1
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
#include <Sm/Ph/ColumnInt64.h>

// Represents an int64 column, using the BIGINT Odbc type. 
class FdoSmPhOdbcColumnInt64 :
    public FdoSmPhColumnInt64, public FdoSmPhOdbcColumn
{
public:
    // Create an int64 columntype.
    FdoSmPhOdbcColumnInt64(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        FdoSmPhDbObject* parentObject,
        bool bNullable,
        bool bIsAutoincremented,
        FdoStringP rootColumnName = L"",
		FdoStringP defaultValue = L"",
        FdoSmPhRdColumnReader* reader = NULL
    ) :
        FdoSmPhOdbcColumn (reader),
        FdoSmPhColumn     ( columnName, L"BIGINT", elementState, parentObject, bNullable, rootColumnName, defaultValue)
    {
        FdoSmPhColumn::SetAutoincrement( bIsAutoincremented );
    }

    virtual ~FdoSmPhOdbcColumnInt64(void) {}

    virtual int GetRdbType()
    {
        return RDBI_DOUBLE;
    }
};

typedef FdoPtr<FdoSmPhOdbcColumnInt64> FdoSmPhOdbcColumnInt64P;

#endif
