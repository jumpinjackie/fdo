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
#include "OraColumnReader.h"
#include "../ColTypeMapper.h"
#include "../Mgr.h"
#include "../../../../SchemaMgr/Ph/Rd/QueryReader.h"
#include "Inc/Rdbi/proto.h"

struct FdoSmPhOraOdbcColTypeMapEntry
{
    FdoSmPhColType  m_colType;
    FdoStringP      m_colTypeString;
    FdoInt32        m_size;             // -1 == don't care
    FdoInt32        m_scale;            // -1 == don't care
};

static FdoSmPhOraOdbcColTypeMapEntry typeMap_S[] =
{
    { FdoSmPhColType_String,    L"VARCHAR",      -1, -1 },
    { FdoSmPhColType_String,    L"VARCHAR2",     -1, -1 },
    { FdoSmPhColType_String,    L"CHAR",         -1, -1 },
    { FdoSmPhColType_BLOB,      L"BLOB",         -1, -1 },
    { FdoSmPhColType_String,    L"UNKNOWN",       0,  0 },
    { FdoSmPhColType_Date,      L"DATE",         -1, -1 }
};
static FdoInt32 localTypeMapSize_S = sizeof(typeMap_S) / sizeof(typeMap_S[0]);



FdoSmPhRdOraOdbcColumnReader::FdoSmPhRdOraOdbcColumnReader(
    FdoSmPhMgrP mgr,
    FdoSmPhDbObjectP    dbObject
) 
{
    FdoStringP objectName = FdoStringP(dbObject->GetName());
    const FdoSmPhOwner*owner = (const FdoSmPhOwner*)(dbObject->GetParent());
    FdoStringP ownerName = owner->GetName();
    FdoStringP databaseName = owner->GetParent()->GetName();

    bool dbSet = databaseName.GetLength() > 0;

    // Generate SQL statement for selecting the columns.
    FdoStringP sqlString = FdoStringP::Format(
        L"select table_name, column_name as name, data_type as type, data_length as \"size\", data_scale as scale, nullable\n"
        L"         from  all_tab_columns\n"
        L"         where\n"
        L"         owner     = :1 \n"
        L"      %ls \n"
        L"        order by table_name, column_id",
        dbObject ? L"and  table_name      = :2" : L""
    );

    // Create a field object for each field in the select list
    FdoSmPhRowsP rows = MakeRows(mgr);

    // Create and set the bind variables
    FdoSmPhRowP binds = new FdoSmPhRow( mgr, L"Binds" );

    FdoSmPhFieldP field = new FdoSmPhField(
        binds,
        L"owner_name",
        binds->CreateColumnDbObject(L"owner_name",false)
    );

    field->SetFieldValue(ownerName);

    if ( dbObject ) {
        field = new FdoSmPhField(
            binds,
            L"object_name",
            binds->CreateColumnDbObject(L"object_name",false)
        );

        field->SetFieldValue(objectName);
    }

    FdoSmPhRdGrdQueryReader* reader =
        new FdoSmPhRdGrdQueryReader( rows->GetItem(0), sqlString, mgr, binds );

    SetSubReader( reader );
}

FdoSmPhColType FdoSmPhRdOraOdbcColumnReader::String2Type( FdoString* colTypeString, int size, int scale )
{
    if ( (wcscmp(colTypeString, L"CHAR") == 0) && (size < 1) ) {
        // The OCI driver maps unhandled types to CHAR but sometimes sets the 
        // length to 0. Zero or undetermined length string column types cannot be 
        // handled so consider these to have unknown type
        return FdoSmPhColType_Unknown;
    }

    if ( wcscmp(colTypeString, L"NUMBER") == 0 ) {
        if ( size == 0 )
            return FdoSmPhColType_Double;
        else
            return FdoSmPhColType_Decimal;
    }
    else {

        // Find the datatype enum facet in the map
         for ( int i=0; i < localTypeMapSize_S;  i++ )
        {
            FdoSmPhOraOdbcColTypeMapEntry* mapEntry = &typeMap_S[i];

            if ( mapEntry->m_colTypeString == colTypeString &&
                (mapEntry->m_scale == -1 || mapEntry->m_scale == scale) &&
                (mapEntry->m_size == -1 || mapEntry->m_size == size) )
            {
                return typeMap_S[i].m_colType;
            }
        }
    }

    // Get to here only if type not found.
    // Return the Unknown type.

    return(FdoSmPhColType_Unknown);
}

bool FdoSmPhRdOraOdbcColumnReader::ReadNext()
{
    int length;
    int scale;
    bool eof = true;

    if ( IsEOF() ) 
        return false;

    if( FdoSmPhReader::ReadNext() )
        eof = false;

    // Check if we're done
    if ( eof ) {
        SetEOF(true);
        return(false);
    }

    // Set the field values according to the current column.
    FdoStringP wColType(GetString(L"", L"type")); 

    SetString( L"", L"name", GetString(L"", L"name") );
    SetString( L"", L"type", wColType );
    SetBoolean( L"", L"nullable", wcscmp(GetString(L"", L"nullable"),L"Y")==0 );
	SetBoolean( L"", L"is_autoincremented", false );
    SetLong( L"", L"scale", scale=GetLong(L"", L"scale") );
    SetLong( L"", L"size", length=GetLong(L"", L"size") );

    mColType = String2Type( wColType, length, scale );

    SetBOF(false);

    return true;
}

FdoSmPhColType FdoSmPhRdOraOdbcColumnReader::GetType()
{
    CheckGet();

    return mColType;
}
