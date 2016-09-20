//
// Copyright (C) 2006 Refractions Research, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include "ColumnInt32.h"

FdoSmPhPostGisColumnInt32::FdoSmPhPostGisColumnInt32(FdoStringP columnName,
    FdoSchemaElementState state,
    FdoSmPhDbObject* parentObject,
    bool isNullable,
    bool isAutoincremented,
    FdoStringP rootColumnName,
    FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* reader)
    : FdoSmPhPostGisColumn(reader),
      FdoSmPhColumn(columnName, L"INTEGER", state, parentObject,
        isNullable, rootColumnName, defaultValue)
{
    FdoSmPhColumn::SetAutoincrement(isAutoincremented);
}

FdoSmPhPostGisColumnInt32::~FdoSmPhPostGisColumnInt32()
{
    // idle
}

int FdoSmPhPostGisColumnInt32::GetRdbType()
{
    return RDBI_INT;
}

FdoStringP FdoSmPhPostGisColumnInt32::GetTypeSql()
{
    if ( this->GetAutoincrement() ) 
        return L"";

    return FdoSmPhColumnInt32::GetTypeSql();
}

FdoStringP FdoSmPhPostGisColumnInt32::GetNullabilitySql()
{
    if ( this->GetAutoincrement() ) 
        return L"";

    return FdoSmPhColumnInt32::GetNullabilitySql();
}

FdoStringP FdoSmPhPostGisColumnInt32::GetAutoincrementSql()
{
    if ( this->GetAutoincrement() ) 
    	return L" serial";

    return FdoSmPhColumnInt32::GetAutoincrementSql();
}

FdoStringP FdoSmPhPostGisColumnInt32::GetDefaultValueSql()
{
    if ( this->GetAutoincrement() ) 
        return L"";

    return FdoSmPhColumnInt32::GetDefaultValueSql();
}

