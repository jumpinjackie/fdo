#ifndef FDOSMPHODBCCOLUMNDATE_H
#define FDOSMPHODBCCOLUMNDATE_H      1
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
#include <Sm/Ph/ColumnDate.h>

// Represents a Odbc Date type column.
class FdoSmPhOdbcColumnDate :
    public FdoSmPhColumnDate, public FdoSmPhOdbcColumn
{
public:
    FdoSmPhOdbcColumnDate(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        FdoSmPhDbObject* parentObject,
        bool bNullable,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* reader = NULL
    ) :
        FdoSmPhOdbcColumn (reader),
        FdoSmPhColumn    ( columnName, L"DATETIME", elementState, parentObject, bNullable, rootColumnName)
    {}

    ~FdoSmPhOdbcColumnDate(void) {}

    virtual int GetRdbType()
    {
        return RDBI_DATE;
    }
};

typedef FdoPtr<FdoSmPhOdbcColumnDate> FdoSmPhOdbcColumnDateP;

#endif
