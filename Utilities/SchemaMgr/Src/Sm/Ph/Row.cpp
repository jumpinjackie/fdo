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

#include "stdafx.h"
#include <Sm/Ph/Row.h>
#include <Sm/Ph/TempObject.h>

FdoSmPhRow::FdoSmPhRow(
    FdoSmPhMgrP mgr, 
    FdoStringP rowName,
    FdoSmPhDbObjectP dbObject
) :
FdoSmPhSchemaElement(rowName, L"", mgr ),
    mDbObject( dbObject ? dbObject : mgr->CreateTempObject() ) 
{
}

FdoSmPhRow::~FdoSmPhRow(void)
{
}

const FdoSmPhFieldCollection* FdoSmPhRow::RefFields() const
{
    return ((FdoSmPhRow*) this)->GetFields();
}

FdoSmPhFieldsP FdoSmPhRow::GetFields()
{
    if ( !mFields )
        mFields = new FdoSmPhFieldCollection();

	return mFields;
}

FdoSmPhDbObjectP FdoSmPhRow::GetDbObject()
{
	return mDbObject;
}

void FdoSmPhRow::Clear()
{
    FdoSmPhFieldsP fields = GetFields();
    FdoInt32 cidx;
    for ( cidx = 0; cidx < fields->GetCount(); cidx++ ) {
        FdoSmPhFieldP field = fields->GetItem(cidx);
        field->Clear();
    }

}

// These functions first check if the column exists. Existing columns are simply
// returned. If the column does not exist then a temporary column (not added to 
// the database object) is created.

FdoSmPhColumnP FdoSmPhRow::CreateColumnBLOB(
    FdoStringP columnName, 
	bool bNullable, 
    FdoStringP rootColumnName
)
{
    FdoSmPhColumnP column = FindColumn( columnName );

    if (column)
        return column;

    return mDbObject->CreateColumnBLOB( columnName, bNullable, rootColumnName, false );
}

FdoSmPhColumnP FdoSmPhRow::CreateColumnChar(
	FdoStringP columnName, 
	bool bNullable, 
    int length,
    FdoStringP rootColumnName
)
{
    FdoSmPhColumnP column = FindColumn( columnName );
    if (column)
        return column;

    return mDbObject->CreateColumnChar( columnName, bNullable, length, rootColumnName, L"", false );
}

FdoSmPhColumnP FdoSmPhRow::CreateColumnDate(
	FdoStringP columnName, 
	bool bNullable, 
    FdoStringP rootColumnName
)
{
    FdoSmPhColumnP column = FindColumn( columnName );
    if (column)
        return column;

    return mDbObject->CreateColumnDate( columnName, bNullable, rootColumnName, L"", false );
}

FdoSmPhColumnP FdoSmPhRow::CreateColumnDecimal(
	FdoStringP columnName, 
	bool bNullable, 
    int length,
    int scale,
    FdoStringP rootColumnName
)
{
    FdoSmPhColumnP column = FindColumn( columnName );
    if (column)
        return column;

    return mDbObject->CreateColumnDecimal( columnName, bNullable, length, scale, rootColumnName, L"", false );
}

FdoSmPhColumnP FdoSmPhRow::CreateColumnSingle(
	FdoStringP columnName, 
	bool bNullable, 
    FdoStringP rootColumnName
)
{
    FdoSmPhColumnP column = FindColumn( columnName );
    if (column)
        return column;

    return mDbObject->CreateColumnSingle( columnName, bNullable, rootColumnName, L"", false );
}

FdoSmPhColumnP FdoSmPhRow::CreateColumnDouble(
	FdoStringP columnName, 
	bool bNullable, 
    FdoStringP rootColumnName
)
{
    FdoSmPhColumnP column = FindColumn( columnName );
    if (column)
        return column;

    return mDbObject->CreateColumnDouble( columnName, bNullable, rootColumnName, L"", false );
}

FdoSmPhColumnP FdoSmPhRow::CreateColumnGeom(
    FdoStringP columnName, 
    FdoSmPhScInfoP AssociatedSCInfo,
    bool bNullable,
    bool bHasElevation, 
    bool bHasMeasure, 
    FdoStringP rootColumnName
)
{
    FdoSmPhColumnP column = FindColumn( columnName );
    if (column)
        return column;

    return mDbObject->CreateColumnGeom( columnName, AssociatedSCInfo, bNullable, bHasElevation, bHasMeasure, rootColumnName, false );
}

FdoSmPhColumnP FdoSmPhRow::CreateColumnBool(
	FdoStringP columnName, 
	bool bNullable, 
    FdoStringP rootColumnName
)
{
    FdoSmPhColumnP column = FindColumn( columnName );
    if (column)
        return column;

    return mDbObject->CreateColumnBool( columnName, bNullable, rootColumnName, L"", false );
}

FdoSmPhColumnP FdoSmPhRow::CreateColumnByte(
	FdoStringP columnName, 
	bool bNullable, 
    FdoStringP rootColumnName
)
{
    FdoSmPhColumnP column = FindColumn( columnName );
    if (column)
        return column;

    return mDbObject->CreateColumnByte( columnName, bNullable, rootColumnName, L"", false );
}

FdoSmPhColumnP FdoSmPhRow::CreateColumnInt16(
	FdoStringP columnName, 
	bool bNullable, 
    FdoStringP rootColumnName
)
{
    FdoSmPhColumnP column = FindColumn( columnName );
    if (column)
        return column;

    return mDbObject->CreateColumnInt16( columnName, bNullable, false, rootColumnName, L"", false );
}

FdoSmPhColumnP FdoSmPhRow::CreateColumnInt32(
	FdoStringP columnName, 
	bool bNullable, 
    FdoStringP rootColumnName
)
{
    FdoSmPhColumnP column = FindColumn( columnName );
    if (column)
        return column;

    return mDbObject->CreateColumnInt32( columnName, bNullable, false, rootColumnName, L"", false );
}

FdoSmPhColumnP FdoSmPhRow::CreateColumnInt64(
	FdoStringP columnName, 
	bool bNullable, 
    FdoStringP rootColumnName
)
{
    FdoSmPhColumnP column = FindColumn( columnName );
    if (column)
        return column;

    return mDbObject->CreateColumnInt64( columnName, bNullable, false, rootColumnName, L"", false );
}

FdoSmPhColumnP FdoSmPhRow::CreateColumnUnknown(
	FdoStringP columnName, 
    FdoStringP typeName,
	bool bNullable, 
    int length,
    int scale,
    FdoStringP rootColumnName
)
{
    FdoSmPhColumnP column = FindColumn( columnName );
    if (column)
        return column;

    return mDbObject->CreateColumnUnknown( columnName,typeName, bNullable, length, scale, rootColumnName, false );
}

FdoSmPhColumnP FdoSmPhRow::CreateColumnDbObject(
	FdoStringP columnName, 
	bool bNullable, 
    FdoStringP rootColumnName
)
{
    FdoSmPhColumnP column = FindColumn( columnName );
    if (column)
        return column;

    return mDbObject->CreateColumnDbObject( columnName, bNullable, rootColumnName, false );
}

FdoSmPhColumnP FdoSmPhRow::FindColumn( FdoStringP columnName )
{
	FdoSmPhColumnsP columns = mDbObject->GetColumns();
    FdoSmPhColumnP column = columns->FindItem( columnName );
    if ( !column )
        column = columns->FindItem( GetManager()->GetDcColumnName(columnName) );

    return column;
}

