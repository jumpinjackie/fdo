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
#include <Sm/Ph/Cfg/PropertyReader.h>
#include <FdoCommonGeometryUtil.h>

FdoSmPhCfgPropertyReader::FdoSmPhCfgPropertyReader(
    FdoStringP schemaName, 
    FdoStringP className, 
    FdoSmPhDbObjectP dbObject, 
    FdoSmPhMgrP mgr
) :
	FdoSmPhRdPropertyReader(dbObject, mgr),
    mSchemaName(schemaName),
    mClassName(className),
    mDbObject(dbObject)
{
}


FdoSmPhCfgPropertyReader::~FdoSmPhCfgPropertyReader(void)
{
}

bool FdoSmPhCfgPropertyReader::ReadNext()
{
    bool found = false;

    // Search for the next property
    while ( !found && FdoSmPhRdPropertyReader::ReadNext() ) {

        SetString( 
            L"f_classdefinition", 
            L"classname", 
            GetClassName()
        );

        FdoStringP colType = GetString(L"", L"columntype");

        if ( colType == L"Association" ) {

            FdoStringsP rdbClassName = FdoStringCollection::Create( 
                GetString( L"", L"attributetype" ),
                L":"
            );

            // The RDB reader makes the association class name the same as the primary
            // key table name. However, the primary table's class might be different
            // (a prefix might have been stripped from the table name). The following
            // figures out the actual class name for the primary key table base on 
            // the config document's autogeneration directives.
            FdoStringP className = GetAssocClassName( rdbClassName->GetString(1) );

            if ( className.GetLength() > 0 ) {

                SetString( 
                    L"", 
                    L"attributetype", 
                    className
                );

                found = true;
            }
        }
        else {
            if ( GetString(L"", L"attributetype") == L"15" ) {
                // Current property is geometric so set its geometric attributes.
                SetGeomTypes();
            }

            found = true;
        }
    }

	return(!IsEOF());
}

void FdoSmPhCfgPropertyReader::SetGeomTypes()
{
    // Default geometric property settings. Provider must override if settings can 
    // be determined from datastore.
    SetString( L"", L"attributetype", FdoStringP::Format(L"%d", FdoGeometricType_Point | FdoGeometricType_Curve | FdoGeometricType_Surface) );
    SetString( L"", L"geometrytype", FdoStringP::Format(L"%d", FdoCommonGeometryUtil::GetAllGeometryTypesCode()));
    SetString( L"", L"hasmeasure", L"0" );
    SetString( L"", L"haselevation", L"0" );
}

