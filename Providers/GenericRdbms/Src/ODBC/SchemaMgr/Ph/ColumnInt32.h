#ifndef FDOSMPHODBCCOLUMNINT32_H
#define FDOSMPHODBCCOLUMNINT32_H       1
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
#include <Sm/Ph/ColumnInt32.h>

// Represents an int32 column, using the INT Odbc type. 
class FdoSmPhOdbcColumnInt32 :
    public FdoSmPhColumnInt32, public FdoSmPhOdbcColumn
{
public:
    // Create an int32 columntype.
    FdoSmPhOdbcColumnInt32(
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
        FdoSmPhColumn     ( columnName, L"INTEGER", elementState, parentObject, bNullable, rootColumnName, defaultValue)
    {
        FdoSmPhColumn::SetAutoincrement( bIsAutoincremented );
    }

    virtual ~FdoSmPhOdbcColumnInt32(void) {}

    virtual int GetRdbType()
    {
        return RDBI_LONG;
    }
};

typedef FdoPtr<FdoSmPhOdbcColumnInt32> FdoSmPhOdbcColumnInt32P;

#endif
