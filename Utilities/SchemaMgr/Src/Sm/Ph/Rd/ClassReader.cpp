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
    FdoBoolean keyedOnly,
    FdoStringP database,
    FdoStringP owner

) :
	FdoSmPhReader(mgr, froms),
    mKeyedOnly(keyedOnly),
    m_IsGeometryFromOrdinatesWanted(mgr->IsGeometryFromOrdinatesWanted()),
    mSchemaName(schemaName),
    mCurrDbObject(-1)
{
    // Get the RDBMS Schema
    mOwner = mgr->FindOwner(owner, database, false);
    if ( mOwner ) {
        if( ((const wchar_t*)className)[0] != '\0' )
        {
            mDbObjects = new FdoSmPhDbObjectCollection(NULL);
            FdoSmPhDbObjectP pObject = mOwner->FindDbObject( className );
            mDbObjects->Add( pObject );
        }
        else
        {
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
            FdoSmPhTableP pTable = pObject->SmartCast<FdoSmPhTable>();
            FdoSmPhViewP pView = pObject->SmartCast<FdoSmPhView>();

            // Only classification of tables or views is supported
            if ( pTable || pView ) {
                // Check if class can be generated from this table or view.
                classifiedObjectName = ClassifyObject( objectName, pTable ? FdoSmPhDbObjType_Table : FdoSmPhDbObjType_View );
                if ( classifiedObjectName.GetLength() > 0 ) {
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

                        // No MetaSchema so table is fixed and was not created by
                        // Schema Manager.
                        pField = pFields->GetItem(L"isfixedtable");
                        pField->SetFieldValue( L"1" );

                        pField = pFields->GetItem(L"istablecreator");
                        pField->SetFieldValue( L"0" );

                        pField = pFields->GetItem(L"geometryproperty");
                        if ( geomPropName.GetLength() > 0 )
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

FdoStringP FdoSmPhRdClassReader::ClassifyObject( FdoStringP objectName, FdoSmPhDbObjType objectType )
{
    FdoStringP classifiedObjectName = objectName;
    bool hasKey = false;

    if ( (classifiedObjectName.GetLength() > 0) &&
         (objectType == FdoSmPhDbObjType_Table) 
    ) {
        FdoSmPhDbObjectP dbObject;

        dbObject = mOwner->FindDbObject( classifiedObjectName );

        if ( dbObject ) {
            if ( (mSchemaName == L"") || (dbObject->GetBestSchemaName() == mSchemaName) ) {
                // Find out if the table has a key.
                FdoSmPhTable* pTable = dbObject ? dynamic_cast<FdoSmPhTable*>((FdoSmPhDbObject*) dbObject) : NULL;
                if ( pTable ) {
                    hasKey = (pTable->GetBestIdentity() != NULL);
                }
            }
            else {
                // DbObject is for a different feature schema.
                classifiedObjectName = L"";
            }
        }
        else {
             // DbObject can't be found, skip it.
            // This can happen in the Oracle Provider for a number of 
            // tables named 'bin$...'. These definitely need to be skipped.
            classifiedObjectName = L"";
        }
    }

    SetBoolean( L"", L"hasKey", hasKey  );

    // Classify only if the object is a table or we were asked to 
    // include any objects.
    // TODO: change the name of mKeyedOnly to reflect its new purpose.
    return ( (!mKeyedOnly) || (objectType == FdoSmPhDbObjType_Table)) ? classifiedObjectName : FdoStringP(L"");
}

FdoStringP FdoSmPhRdClassReader::FindGeometryProperty( FdoSmPhColumnsP cols, bool& hasGeom )
{
    FdoStringP geomPropName;
    FdoInt32 propScore = 0;
    FdoInt32 highestScore = 0;
    FdoInt32 idx;

    hasGeom = false;

    // Find the best main geometry
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
                if ( colGeom->GetSpatialIndex() ) 
                    propScore += 100;

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
