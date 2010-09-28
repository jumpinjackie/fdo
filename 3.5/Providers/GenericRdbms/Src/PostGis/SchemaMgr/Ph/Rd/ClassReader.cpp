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
#include "ClassReader.h"

FdoSmPhRdPostGisClassReader::FdoSmPhRdPostGisClassReader(
    FdoSmPhRowsP froms, 
    FdoStringP schemaName, 
    FdoStringP className, 
    FdoSmPhMgrP mgr,
    FdoBoolean classifyDefaultTypes,
    FdoStringP database,
    FdoStringP owner

) :
	FdoSmPhRdClassReader(froms, schemaName, className, mgr, classifyDefaultTypes, database, owner),
    //BR - TODO - Base class should provide an accessor to schema name.
    mPgSchemaName(schemaName)
{
}

FdoSmPhRdPostGisClassReader::~FdoSmPhRdPostGisClassReader(void)
{
}

bool FdoSmPhRdPostGisClassReader::ReadNext()
{
    mDbObject = NULL; 

    bool ret = FdoSmPhRdClassReader::ReadNext();

    if ( ret ) {
        // BR - TODO - base class should reset this
        SetString( L"", L"parentclassname", L"" );
        if ( mDbObject) {
            FdoSmPhTableP table = mDbObject->SmartCast<FdoSmPhTable>();

            if ( table ) {
                FdoSmPhBaseObjectsP baseObjects = mDbObject->GetBaseObjects();
                if ( baseObjects->GetCount() > 0 ) {
                    FdoSmPhDbObjectP baseObject = FdoSmPhBaseObjectP(baseObjects->GetItem(0))->GetDbObject();

                    if ( baseObject && (baseObject->GetParent()->GetQName() == mDbObject->GetParent()->GetQName()) ) {
                        if ( (mPgSchemaName == L"") || (baseObject->GetBestSchemaName() == mPgSchemaName) ) {
                            // PostgreSQL allows a table with geometry to inherit from
                            // a table without geometry. 
                            // The Generic ClassReader classifies:
                            //      - tables with geometry as feature classes
                            //      - tables without geometry as non-feature classes
                            // but FDO disallows a feature class from inheriting from a
                            // non-feature class. 
                            // Therefore, do not set base for FDO class if this class and
                            // base class have different class types.

                            FdoStringP classType = GetString( L"", L"classtype" );
                            bool hasGeom         = (classType == L"2") ? true : false;
                            bool baseHasGeom     = false;
                            FindGeometryProperty( baseObject->GetColumns(), baseHasGeom );

                            if ( hasGeom == baseHasGeom ) 
                                SetString( L"", L"parentclassname", baseObject->GetBestSchemaName() + L":" + baseObject->GetBestClassName() );
                        }
                    }
                }
            }
        }
    }

    return ret;
}

FdoStringP FdoSmPhRdPostGisClassReader::ClassifyObject( FdoSmPhDbObjectP dbObject )
{
    FdoStringP classifiedName = FdoSmPhRdClassReader::ClassifyObject( dbObject );

    if ( classifiedName != L"" ) 
        mDbObject = dbObject;

    return classifiedName;
}

