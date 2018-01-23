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
#include "ColumnBool.h"

FdoSmPhPostGisColumnBool::FdoSmPhPostGisColumnBool(FdoStringP columnName,
    FdoSchemaElementState state,
    FdoSmPhDbObject* parentObject,
    bool isNullable,
    FdoStringP rootColumnName,
    FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* reader)
    : FdoSmPhPostGisColumn(reader),
      FdoSmPhColumn(columnName, L"BOOLEAN", state, parentObject,
        isNullable, rootColumnName, defaultValue)
{
    // TODO: mloskot - Does the typeName passed
    // to the FdoSmPhColumn is case-sensitive?

    // idle
}

FdoSmPhPostGisColumnBool::~FdoSmPhPostGisColumnBool()
{
    // idle
}

FdoStringP FdoSmPhPostGisColumnBool::GetDefaultValueSql()
{
    FdoPtr<FdoDataValue> defaultValue = GetDefaultValue();

    if ( defaultValue && !(defaultValue->IsNull()) && defaultValue->GetDataType() == FdoDataType_Boolean )
	{		
        FdoBooleanValue* boolVal = (FdoBooleanValue*)defaultValue.p;

		FdoStringP	defaultValueSql = FdoStringP(L"DEFAULT ");
        defaultValueSql += ((boolVal->GetBoolean())?"TRUE ":"FALSE ");
		return defaultValueSql;
	}

    return L"";
}

int FdoSmPhPostGisColumnBool::GetRdbType()
{
    // TODO: MySQL handles BOOL type as TINYINT.
    // Is this the reason it translates it to RDBI_LONG?

    // PostgreSQL supports real boolean type.
    return RDBI_BOOLEAN;
}
