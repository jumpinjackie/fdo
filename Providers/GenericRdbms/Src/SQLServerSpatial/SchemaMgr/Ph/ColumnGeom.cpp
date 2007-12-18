/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#include "ColumnGeom.h"
#include "SpatialIndex.h"
#include "Table.h"
#include "Mgr.h"
#include <Sm/Ph/Rd/QueryReader.h>
#include <FdoCommonStringUtil.h>
#include <limits>       // For quiet_NaN()
#include <math.h>      // For _isnan()
using namespace std;

#define NUMBER_STRING_SIZE  (100)


FdoSmPhSqsColumnGeom::FdoSmPhSqsColumnGeom(
	FdoStringP columnName, 
	FdoSchemaElementState elementState,
	FdoSmPhDbObject* parentObject,
    FdoSmPhScInfoP AssociatedSCInfo,        
    bool bNullable,
    bool bHasElevation, 
    bool bHasMeasure, 
    FdoStringP rootColumnName,
    FdoSmPhRdColumnReader* reader
) :
    FdoSmPhSqsColumn (reader),
    FdoSmPhColumn    ( columnName, L"GEOMETRY", elementState, parentObject, bNullable, rootColumnName),
    FdoSmPhColumnGeom( AssociatedSCInfo, bHasElevation, bHasMeasure )
{
    if ( elementState == FdoSchemaElementState_Added ) {
        //Create a spatial index if this is a new geometric column
        FdoSmPhSpatialIndexP currIndex = GetSpatialIndex();

        if ( !currIndex ) {
            CreateSpatialIndex();
        }
    }
}

FdoSmPhSqsColumnGeom::~FdoSmPhSqsColumnGeom(void)
{
}


FdoSmPhSpatialIndexP FdoSmPhSqsColumnGeom::CreateSpatialIndex( FdoStringP indexName)
{

    // Error if column already indexed
    FdoSmPhSpatialIndexP currIndex = GetSpatialIndex();

    if ( currIndex ) 
        throw FdoSchemaException::Create( 
			NlsMsgGet1(
				FDORDBMS_131,
				"Cannot add spatial index to column %1$ls, it already has one.", 
                (FdoString*) GetQName()
            )
		);

    FdoSmPhTableP table = GetContainingDbObject().p->SmartCast<FdoSmPhTable>();

    if ( table ) {
        FdoSmPhSqsSpatialIndexP newIndex = new FdoSmPhSqsSpatialIndex(
            (indexName == L"") ? UniqueIndexName() : indexName,
            table,
            FdoSchemaElementState_Added
        );

        currIndex = newIndex->SmartCast<FdoSmPhSpatialIndex>();

        SetSpatialIndex( currIndex );
    }

    return currIndex;
}


bool FdoSmPhSqsColumnGeom::Add() 
{
    FdoSmPhSqsMgrP mgr = GetManager()->SmartCast<FdoSmPhSqsMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();
    FdoSmPhSqsTable* table = (FdoSmPhSqsTable*)(dynamic_cast<const FdoSmPhSqsTable*>(GetParent()));
    FdoSmPhOwner* owner = (FdoSmPhOwner*)(static_cast<const FdoSmPhOwner*>(GetParent()->GetParent()));

    // Nothing to do here?
    return true;
}

void FdoSmPhSqsColumnGeom::SetElementState(FdoSchemaElementState elementState)
{
    FdoSmPhColumnGeom::SetElementState(elementState);

    if ( GetElementState() == FdoSchemaElementState_Deleted ) {

        // TODO: Is the index dropped along with the column?

        //FdoSmPhSpatialIndexP currIndex = GetSpatialIndex();

        //if ( currIndex ) 
        //    // Deleting the column, so must delete the spatial index.
        //    currIndex->SetElementState( FdoSchemaElementState_Deleted );
    }
}

void FdoSmPhSqsColumnGeom::PostFinalize()
{
    if ( GetElementState() != FdoSchemaElementState_Added ) {
        bool hasMeasure = false;
        bool hasElevation = false;
        bool foundMetaData = false;

        FdoSmPhOwner* owner         = (FdoSmPhOwner*)(FdoSmSchemaElement*)(GetParent()->GetParent());
        FdoStringP    dbObjectName  = GetParent()->GetName();

        FdoSmPhSpatialContextGeomsP scGeoms = owner->GetSpatialContextGeoms();

        FdoSmPhSpatialContextGeomP scGeom = scGeoms->FindItem( FdoSmPhSpatialContextGeom::MakeName(dbObjectName, GetName()) );

        if ( scGeom ) {
            foundMetaData = true;
                
            hasElevation = scGeom->GetHasElevation();
            hasMeasure = scGeom->GetHasMeasure();
        }

        if ( foundMetaData ) {
            // Set the geometric attribute fields for this reader.
            SetHasMeasure( hasMeasure );
            SetHasElevation( hasElevation );
        }
    }
}
