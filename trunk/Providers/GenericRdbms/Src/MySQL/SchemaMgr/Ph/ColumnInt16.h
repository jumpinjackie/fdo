#ifndef FDOSMPHMYSQLCOLUMNINT16_H
#define FDOSMPHMYSQLCOLUMNINT16_H       1
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
#include <Sm/Ph/ColumnInt16.h>

// Represents a int16 column, using the SMALLINT MySql type. 
class FdoSmPhMySqlColumnInt16 :
    public FdoSmPhColumnInt16, public FdoSmPhMySqlColumn
{
public:
    // Create an int16 columntype.
    FdoSmPhMySqlColumnInt16(
        FdoStringP columnName,
        FdoSchemaElementState elementState,
        FdoSmPhDbObject* parentObject,
        bool bNullable,
		bool bIsAutoincremented,
        FdoStringP rootColumnName = L"",
		FdoStringP defaultValue = L"",
        FdoSmPhRdColumnReader* reader = NULL
    ) :
        FdoSmPhMySqlColumn (reader),
        FdoSmPhColumn     ( columnName, L"SMALLINT", elementState, parentObject, bNullable, rootColumnName, defaultValue)
    {
		FdoSmPhColumn::SetAutoincrement(bIsAutoincremented); 
	}

    virtual ~FdoSmPhMySqlColumnInt16(void) {}

    virtual int GetRdbType()
    {
        return RDBI_SHORT;
    }

};

typedef FdoPtr<FdoSmPhMySqlColumnInt16> FdoSmPhMySqlColumnInt16P;

#endif
