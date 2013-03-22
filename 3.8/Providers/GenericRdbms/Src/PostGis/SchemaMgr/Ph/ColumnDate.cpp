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
#include "ColumnDate.h"

FdoSmPhPostGisColumnDate::FdoSmPhPostGisColumnDate(FdoStringP columnName,
    FdoSchemaElementState state,
    FdoSmPhDbObject* parentObject,
    bool isNullable,
    FdoStringP rootColumnName,
    FdoPtr<FdoDataValue> defaultValue,
    FdoSmPhRdColumnReader* reader)
    : FdoSmPhPostGisColumn(reader),
      FdoSmPhColumn(columnName, L"TIMESTAMP", state, parentObject,
        isNullable, rootColumnName, defaultValue)
{
    // idle
}

FdoSmPhPostGisColumnDate::~FdoSmPhPostGisColumnDate()
{
    // idle
}

int FdoSmPhPostGisColumnDate::GetRdbType()
{
    return RDBI_DATE;
}

FdoInt64 FdoSmPhPostGisColumnDate::GetDbBinarySize()
{
    // Storat size of TIMESTAMP in bytes
    return 8;
}

FdoStringP FdoSmPhPostGisColumnDate::GetDefaultValueSql()
{
    FdoPtr<FdoDataValue> defaultValue = GetDefaultValue();

    if ( defaultValue && !(defaultValue->IsNull()) )
	{		
        FdoDateTimeValue* dt = dynamic_cast<FdoDateTimeValue*>(defaultValue.p);
        if( dt != NULL )
        {
            FdoStringP dateStr = dt->ToString();
            dateStr = dateStr.Replace(L"TIMESTAMP",L"");
            dateStr = dateStr.Replace(L"TIME",L"");
            dateStr = dateStr.Replace(L"DATE",L"");
		    FdoStringP	defaultValueSql = FdoStringP(L"DEFAULT ") + dateStr;
                    
		    return defaultValueSql;
        }
	}

    return L"";
}
