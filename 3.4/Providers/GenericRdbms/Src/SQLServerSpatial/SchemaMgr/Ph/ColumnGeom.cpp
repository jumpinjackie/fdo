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
    FdoSmPhColumn    ( columnName, L"geometry", elementState, parentObject, bNullable, rootColumnName, (FdoDataValue*) NULL, reader),
    FdoSmPhColumnGeom( AssociatedSCInfo, false, false ),
    mSRID(-1)
{
    if ( elementState == FdoSchemaElementState_Added ) {
        //Create a spatial index if this is a new geometric column
        FdoSmPhSpatialIndexP currIndex = GetSpatialIndex();

        if ( !currIndex ) {
            CreateSpatialIndex();
        }

		if (AssociatedSCInfo && parentObject)
		{
			FdoSmPhSqsOwner* owner = (FdoSmPhSqsOwner*)(parentObject->GetParent());
			if (!owner)
				owner = (FdoSmPhSqsOwner*) GetContainingDbObject()->GetParent();

			if (owner)
			{
				FdoSmPhCoordinateSystemP coordSystem = owner->FindCataloguedCoordinateSystem(AssociatedSCInfo->mSrid);
				if (coordSystem)
				{
					FdoStringP wkt = coordSystem->GetWkt();
					if (wkt.Left(L"[").Contains(L"GEOGCS" ))
						SetTypeName(L"geography");
				}
			}
		}
    }
}

FdoSmPhSqsColumnGeom::~FdoSmPhSqsColumnGeom(void)
{
}

FdoInt64 FdoSmPhSqsColumnGeom::GetSRID()
{
    if (mSRID == -1)
    {
        // Check if SRID was encoded into spatial index name,
        mSRID = IndexName2Srid();

        // If not in SI name then sample 1st geometry value.
        // SqlServer SRIDs are associated with geometry values, rather than entire column.
        // The following gets the srid from the first row.
        // The other rows might have different srids but FDO currently does not have
        // a way of handling this. 
        
        if ( (mSRID == -1 ) && (GetElementState() != FdoSchemaElementState_Added) ) {
            FdoSmPhSqsOwner* owner = (FdoSmPhSqsOwner*)(this->GetParent()->GetParent());
            mSRID = owner->SampleColumnSrid( GetParent()->GetName(), GetName() );
        }

        // When SRID was not found ...
        if ( mSRID == -1 ) {
            if ( this->GetTypeName() == L"geography" ) 
                // Default for geography columns is "WGS 84"
                mSRID = 4326;
            else
                // Default for geometry columns is a planar coordinate system
                mSRID = 0;
        }
    }
    return mSRID;
}

FdoSmPhSpatialIndexP FdoSmPhSqsColumnGeom::CreateSpatialIndex( FdoStringP indexName)
{

    // Error if column already indexed
    FdoSmPhSpatialIndexP currIndex = GetSpatialIndex();

    if ( currIndex && (currIndex->GetElementState() != FdoSchemaElementState_Deleted) ) 
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

void FdoSmPhSqsColumnGeom::RegenSpatialIndex()
{
    FdoSmPhSpatialIndexP currIndex = GetSpatialIndex();

    // If spatial index exists, delete and re-create it.
    if ( currIndex && (currIndex->GetElementState() != FdoSchemaElementState_Deleted) ) {
        currIndex->SetElementState( FdoSchemaElementState_Deleted );

        CreateSpatialIndex();
    }
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


        FdoSmPhSpatialIndexP currIndex = GetSpatialIndex();

        if ( currIndex ) 
            // Deleting the column, so must delete the spatial index.
            currIndex->SetElementState( FdoSchemaElementState_Deleted );
    }
}

FdoStringP FdoSmPhSqsColumnGeom::UniqueIndexName()
{
    FdoStringP indexName = FdoSmPhColumnGeom::UniqueIndexName();
    
    FdoInt64 srid = GetSpatialContextInfo() ? GetSpatialContextInfo()->mSrid : 0L;

    // Encode srid into spatial index name. This allows us to remember the srid
    // when no geometry values have yet been added to this column.
    // Skip this step if resulting name would be too long. This should rarely
    // happen since the limit is 128 characters.

    if ( (srid > 0) && indexName.GetLength() < GetManager()->ColNameMaxLen() - ((int) log10((double)srid)) - 7 ) {
        indexName += FdoStringP(L"_srid=") + FdoCommonStringUtil::Int64ToString(srid);
    }

    return indexName;
}

FdoInt64 FdoSmPhSqsColumnGeom::IndexName2Srid()
{
    FdoInt64 srid = -1;
    FdoSmPhSpatialIndexP si = GetSpatialIndex();

    if ( si )
        // Processing centralized on Physical Manager so Spatial Context Reader can 
        // also get srid from index name.
        srid = static_cast<FdoSmPhSqsMgr*>(GetManager().p)->IndexName2Srid( si->GetName() );

    return srid;
}

void FdoSmPhSqsColumnGeom::PostFinalize()
{
    if ( GetElementState() != FdoSchemaElementState_Added ) {
        bool hasMeasure = false;
        bool hasElevation = false;
        bool foundMetaData = false;

        FdoSmPhOwner* owner         = (FdoSmPhOwner*)(FdoSmSchemaElement*)(GetParent()->GetParent());
        FdoStringP    dbObjectName  = GetParent()->GetName();

        FdoSmPhSpatialContextGeomP scGeom = owner->FindSpatialContextGeom(dbObjectName, GetName());

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
