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
#include <Sm/Ph/Rd/ClassReader.h>
#include <Sm/Ph/Table.h>
#include <Sm/Ph/SpatialIndex.h>
#include <Sm/Ph/Rd/QueryReader.h>

bool FdoSmPhRdClassReader::IsOrdinate(FdoSmPhColumnP column)
{
    bool isOrdinate = false;

    FdoStringP name = column->GetName();

    if (name.ICompare(L"X")==0 ||
        name.ICompare(L"Y")==0 ||
        name.ICompare(L"Z")==0)
    {
        isOrdinate = true;
    }

    return isOrdinate;
}


FdoSmPhRdClassReader::FdoSmPhRdClassReader(
    FdoSmPhRowsP froms, 
    FdoStringP schemaName, 
    FdoStringP className, 
    FdoSmPhMgrP mgr,
    FdoBoolean classifyDefaultTypes,
    FdoStringP database,
    FdoStringP owner

) :
	FdoSmPhReader(mgr, froms),
    mClassifyDefaultTypes(classifyDefaultTypes),
    m_IsGeometryFromOrdinatesWanted(mgr->IsGeometryFromOrdinatesWanted()),
    mSchemaName(schemaName),
    mCurrDbObject(-1)
{
    // Get the RDBMS Schema
    mOwner = mgr->FindOwner(owner, database, false);
    if ( mOwner ) {
        FdoSmPhDbObjectP pObject;
        if( ((const wchar_t*)className)[0] != '\0' )
        {
            // Reading a single class. Assume class table has same name.
            pObject = mOwner->FindDbObject( className );
            if ( pObject ) {
                mDbObjects = new FdoSmPhDbObjectCollection(NULL);
                mDbObjects->Add( pObject );
            }
        }

        if ( !pObject ) 
        {
            // Reading all classes or table for single class not found.
            // When table for single class not found, table might exist
            // but be named differently from class. In this case, must
            // read all tables.

            // Cache all of the objects from the given owner.
            // This pre-load provides better performance than 
            // caching each object individually.
            mDbObjects = mOwner->CacheDbObjects(true);
        }
        FdoSmPhRowP row = froms->GetItem(0);

        FdoSmPhFieldP field = new FdoSmPhField(
            row, 
            L"hasKey",
            row->CreateColumnBool(L"hasKey",true)
        );
    }
}

FdoSmPhRdClassReader::~FdoSmPhRdClassReader(void)
{
}

bool FdoSmPhRdClassReader::ReadNext()
{
    FdoStringP objectName;
    FdoStringP classifiedObjectName;
    int eof = IsEOF() ? 1 : 0;
    bool found = false;

    // Keep going until we find a valid table or run out of objects
    while ( !eof && !found  ) {
        mCurrDbObject++;

        if ( (!mDbObjects) || (mCurrDbObject >= mDbObjects->GetCount()) ) {
            eof = true;
            SetEOF(true);
        }
        else {
            // Get the current object and determine whether table or view 
            FdoSmPhDbObjectP pObject = mDbObjects->GetItem( mCurrDbObject );
            objectName = pObject->GetName();
 
            // Only classification of tables or views is supported
            if ( ClassifyObjectType(pObject, mClassifyDefaultTypes) ) {
                // Check if class can be generated from this table or view.
                classifiedObjectName = ClassifyObject( pObject );
                if ( ((const wchar_t*)classifiedObjectName)[0] != '\0' )  {
                    found = true;

                    FdoSmPhColumnsP cols = pObject->GetColumns();
                    FdoStringP geomPropName;
                    bool hasGeom = false;

                    // Choose the geometryProperty
                    geomPropName = FindGeometryProperty( cols, hasGeom );

                    // Grab the first row and set its fields with 
                    // info from the current table.
                    FdoSmPhRowsP pFroms = GetRows();
                    FdoSmPhRowP pFrom = (pFroms && (pFroms->GetCount() > 0)) ? pFroms->GetItem(0) : NULL;

                    if ( pFrom ) {
                        // Schema name is the one passed in
						FdoSmPhFieldsP pFields = pFrom->GetFields();
                        FdoSmPhFieldP pField = pFields->GetItem(L"schemaname");
                        pField->SetFieldValue( mSchemaName );

                        // Class name is table name
                        pField = pFields->GetItem(L"classname");
                        pField->SetFieldValue( pObject->GetBestClassName() );

                        // Class type id not relevant when no MetaSchema
                        // but set it anyway
                        pField = pFields->GetItem(L"classtype");
                        if ( hasGeom )
                            pField->SetFieldValue( L"2" );
                        else
                            pField->SetFieldValue( L"1" );

                        pField = pFields->GetItem(L"tablename");
                        pField->SetFieldValue( objectName );

                        pField = pFields->GetItem(L"tableowner");
                        FdoSmPhMgrP mgr = this->GetManager();
                        if ( mgr->GetOwner() == mOwner )
                            pField->SetFieldValue( L"" );
                        else
                            pField->SetFieldValue( mOwner->GetName() );

                        // No MetaSchema so table is fixed
                        pField = pFields->GetItem(L"isfixedtable");
                        pField->SetFieldValue( L"1" );

                        // Table may have been created by Schema Manager,
                        // there is no way of knowing for sure.
                        // However, since there is no MetaSchema, assume
                        // that Schema Manager can take ownership of it.
                        pField = pFields->GetItem(L"istablecreator");
                        pField->SetFieldValue( L"1" );

                        pField = pFields->GetItem(L"geometryproperty");
                        if ( ((const wchar_t*)geomPropName)[0] != '\0' )
                            pField->SetFieldValue( geomPropName );
                        else
                            pField->SetFieldValue( L"" );
                    }

                    // 2nd row has class type.
                    pFrom = (pFroms && (pFroms->GetCount() > 1)) ? pFroms->GetItem(1) : NULL;

                    // Feature class if it has geometry.
                    if ( pFrom ) {
						FdoSmPhFieldsP pFields = pFrom->GetFields();
                        FdoSmPhFieldP pField = pFields->GetItem(L"classtypename");
                        if ( hasGeom )
                            pField->SetFieldValue( L"Feature" );
                        else
                            pField->SetFieldValue( L"Class" );
                    }
                }
            }
        }

        SetBOF(false);
    }

	return(!IsEOF());
}

bool FdoSmPhRdClassReader::ClassifyObjectType( FdoSmPhDbObjectP dbObject, FdoBoolean classifyDefaultTypes )
{
    return dbObject->ClassifyObjectType(classifyDefaultTypes);
}

FdoStringP FdoSmPhRdClassReader::ClassifyObject( FdoSmPhDbObjectP dbObject )
{
    FdoStringP classifiedObjectName = dbObject->GetClassifiedObjectName(mSchemaName);

    bool hasKey =false;
    if ( ((const wchar_t*)classifiedObjectName)[0] != '\0' )
    {
        if ( (mSchemaName == L"") || (dbObject->GetBestSchemaName() == mSchemaName) )
        {
            // Find out if the database object has a key.
            hasKey = (dbObject->GetBestIdentity() != NULL);
        }
    }
    SetBoolean( L"", L"hasKey", hasKey );

    return classifiedObjectName;
}

FdoStringP FdoSmPhRdClassReader::FindGeometryProperty( FdoSmPhColumnsP cols, bool& hasGeom )
{
    FdoStringP geomPropName;
    FdoInt32 propScore = 0;
    FdoInt32 highestScore = 0;
    FdoInt32 idx;

    hasGeom = false;

    // Find the best main geometry

    // First, check if table has only one geometric column. If it does then 
    // it is the main geometry. This first loop is done to avoid tripping
    // index loads for these tables.
    for ( idx = 0; idx < cols->GetCount(); idx++ ) {
        FdoSmPhColumnP col = cols->GetItem(idx);
        if ( col->GetType() == FdoSmPhColType_Geom ) {
            if ( geomPropName == L"" ) {
                geomPropName = col->GetName();
                hasGeom = true;
            }
            else {
                geomPropName = L"";
                break;
            }
        }
    }

    
    if ( geomPropName == L"" ) {
        // No single geometric column. Determine the best column to be the main geometry.
        for ( idx = 0; idx < cols->GetCount(); idx++ ) {
            propScore = 0;
            FdoSmPhColumnP col = cols->GetItem(idx);
            if ( col->GetType() == FdoSmPhColType_Geom ) {
                FdoSmPhColumnGeomP colGeom = col->SmartCast<FdoSmPhColumnGeom>();
                if ( colGeom ) {
                    // Found a geometric column, calculate its score.
                    propScore++;
                    hasGeom = true;

                    // When multiple geometric columns, try to select the one
                    // with spatial index (give it a high score).
                    FdoSmPhSpatialIndexP spatialIndex = colGeom->GetSpatialIndex();
                    if ( spatialIndex ) {
                        propScore += 100;

                        // Extra points if the index name is marked as being the index
                        // for the main geometry column.

                        if ( spatialIndex->GetIsPrimary() ) 
                            propScore += 1000;
                    }

                    // If spatial index presence doesn't break the tie
                    // give priority to not nullable columns.
                    if ( !colGeom->GetNullable() )
                        propScore += 10;

                    // Check column's score against previous high score
                    if ( propScore > highestScore ) {
                        // It has the highest score sofar. Make it the 
                        // candidate geometryProperty
                        geomPropName = col->GetName();
                        highestScore = propScore;
                    }
                    else if ( propScore == highestScore ) {
                        // A tie with previous highest score, 
                        // discard current candidate.
                        geomPropName = L"";
                    }
                }
            }
        }
    }

    // If geometryProperty can't be determined from geometric columns,
    // try oordinate columns if provider supports them (mapping to 
    // X,Y and Z columns).
    if ( m_IsGeometryFromOrdinatesWanted && !(geomPropName.GetLength() > 0) ) {
        for ( idx = 0; idx < cols->GetCount(); idx++ ) {
            FdoSmPhColumnP col = cols->GetItem(idx);
            if ( IsOrdinate(col) ) {
                geomPropName = L"Geometry";
                hasGeom = true;
                break;
            }
        }
    }

    return geomPropName;
}

// This function is now obsolete; override or extend ClassifyObject( FdoSmPhDbObjectP ) instead.
FdoStringP FdoSmPhRdClassReader::ClassifyObject( FdoStringP objectName, FdoSmPhDbObjType objectType )
{
    throw FdoException::Create( L"Internal error, obsolete function FdoSmPhRdClassReader::ClassifyObject( FdoStringP , FdoSmPhDbObjType ) should never be called" );
}
