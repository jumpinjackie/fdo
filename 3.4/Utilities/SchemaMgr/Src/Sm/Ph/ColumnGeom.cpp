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
#include <Sm/Ph/ColumnGeom.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/SpatialIndex.h>
#include <Sm/Error.h>

FdoSmPhColumnGeom::FdoSmPhColumnGeom(
    FdoSmPhScInfoP AssociatedSCInfo,
    bool bHasElevation, 
    bool bHasMeasure
) :
    mbHasElevation(bHasElevation),
    mbHasMeasure(bHasMeasure),
    mScInfo(AssociatedSCInfo),
    mSpatialIndex(NULL),
    mbIsPrimary(false)
{
}

FdoSmPhColumnGeom::~FdoSmPhColumnGeom(void)
{
}

FdoSmPhScInfoP	FdoSmPhColumnGeom::GetSpatialContextInfo()
{
    if ( !mScInfo ) {
        FdoSmPhSpatialContextP sc = GetSpatialContext();

        if ( sc ) {
            // Associated Spatial Context was found.
            mScInfo = FdoSmPhScInfo::Create();
            mScInfo->mSrid = sc->GetSrid();
            mScInfo->mCoordSysName = sc->GetCoordinateSystem();
            mScInfo->mExtent = sc->GetExtent();
            mScInfo->mXYTolerance = sc->GetXYTolerance();
            mScInfo->mZTolerance = sc->GetZTolerance();            
        }
    }

    return mScInfo;
}

FdoSmPhSpatialContextP FdoSmPhColumnGeom::GetSpatialContext()
{
    if ( !mSpatialContext ) {
        // No Spatial context info set yet so set it from the associated spatial context.
        FdoSmSchemaElement* dbObject = (FdoSmSchemaElement*)(GetParent());
        
        if ( dbObject ) {
            FdoSmPhOwner* owner         = (FdoSmPhOwner*)(dbObject->GetParent());
        
            if ( owner ) {
                FdoStringP    dbObjectName  = dbObject->GetName();

                // Get Spatial Context Geometry assocation, use it to get the spatial context
                FdoSmPhSpatialContextGeomP scGeom = owner->FindSpatialContextGeom(dbObjectName, GetName());

                if ( scGeom )
                    mSpatialContext = scGeom->GetSpatialContext();
            }
        }
    }

    return mSpatialContext;
}

void FdoSmPhColumnGeom::SetPrimary( bool isPrimary )
{
    mbIsPrimary = isPrimary;

    // Is-primary status is persisted in the spatial index name.
    FdoSmPhSpatialIndexP si = GetSpatialIndex();

    if ( si ) {
        if ( isPrimary != si->GetIsPrimary() )
            // New status and status on current spatial index are different.
            // Regenerate the spatial index. This causes the spatial index name to change.
            RegenSpatialIndex();
    }
}

void FdoSmPhColumnGeom::RegenSpatialIndex()
{
}

int FdoSmPhColumnGeom::GetDimensionality()
{
    int  dimensionality = -1;
/*
    if (      GetHasElevation() && GetHasMeasure())
        dimensionality = DBI_XYZM;
    else if ( GetHasElevation() && !GetHasMeasure())
        dimensionality = DBI_XYZ;
    else if ( !GetHasElevation() && GetHasMeasure())
        dimensionality = DBI_XYM;
    else
        dimensionality = DBI_XY;
*/
    return dimensionality;
}

FdoSmPhSpatialIndexP FdoSmPhColumnGeom::GetSpatialIndex()
{
    int idx;

    if ( (!mSpatialIndex) && (GetElementState() != FdoSchemaElementState_Added) ) {

        // This column exists but does not yet reference its spatial index,
        // look for the spatial index in the column's table.
        FdoSmPhTableP table = 
            FDO_SAFE_ADDREF( (FdoSmPhTable*)(dynamic_cast<const FdoSmPhTable*>(GetParent())) );

        if ( table ) {
            FdoSmPhIndexesP indexes = table->GetIndexes();
            FdoStringP thisName( GetName() );

            // Check each index
            for ( idx = 0; idx < indexes->GetCount(); idx++ ) {
                // Only interested in spatial indexes
                FdoSmPhSpatialIndexP currIndex = indexes->GetItem(idx)->SmartCast<FdoSmPhSpatialIndex>(true);

                if ( currIndex ) {
                    FdoSmPhColumnsP columns = currIndex->GetColumns();

                    // If this column is the only column in the current spatial then
                    // it is the index we're looking for.
                    if ( columns->GetCount() == 1 ) {
                        FdoSmPhColumnP column = columns->GetItem(0);

                        if ( thisName == column->GetName() ) {
                            mSpatialIndex = currIndex;
                            break;
                        }
                    }
                }
            }
        }
    }

    return FDO_SAFE_ADDREF(mSpatialIndex);
}

void FdoSmPhColumnGeom::DiscardSpatialIndex( FdoSmPhSpatialIndex* index )
{
    // back-pointer not ref-counted.
    mSpatialIndex = NULL;
}

bool FdoSmPhColumnGeom::DefinitionEquals( FdoSmPhColumnP otherColumn )
{
    bool equals = FdoSmPhColumn::DefinitionEquals( otherColumn );

    if ( equals ) {
        equals = false;

        FdoSmPhColumnGeomP otherGeomColumn = otherColumn->SmartCast<FdoSmPhColumnGeom>();

        if ( otherGeomColumn &&
             (GetDimensionality() == otherGeomColumn->GetDimensionality()) &&
             (GetSRID() == otherGeomColumn->GetSRID())
        ) 
            equals = true;
    }

    return equals;
}

void FdoSmPhColumnGeom::SetSpatialIndex( FdoSmPhSpatialIndexP spatialIndex )
{
    FdoSmPhTableP table = 
        FDO_SAFE_ADDREF( (FdoSmPhTable*)(dynamic_cast<const FdoSmPhTable*>(GetParent())) );

    if ( !table ) 
        throw FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				FDO_NLSID(FDOSM_22),
				(FdoString*) GetQName(), 
                (FdoString*) GetParent()->GetQName()
			)
		);

    // Delete any previous spatial index
    if ( mSpatialIndex ) 
        mSpatialIndex->SetElementState( FdoSchemaElementState_Deleted );

    // Associate this column with the new spatial index
    if ( spatialIndex ) {
        spatialIndex->AddColumn( FDO_SAFE_ADDREF(this) );
        table->GetIndexes()->Add( spatialIndex );
    }

    mSpatialIndex = spatialIndex;
}

FdoStringP FdoSmPhColumnGeom::UniqueIndexName()
{
    const FdoSmPhTable* table = 
        dynamic_cast<const FdoSmPhTable*>(GetParent());

    const FdoSmPhOwner* owner = 
        dynamic_cast<const FdoSmPhOwner*>(table->GetParent());

    // Start with a name that is a concatenation of the table and column
    // names plus a suffix. The primary status of this column is encoded by
    // the suffix:
    //      gsi - is primary
    //      is - is not primary

    FdoStringP suffix = mbIsPrimary ? FdoStringP( L"_gsi", true ): FdoStringP( L"_si", true );
    FdoStringP tableName = table->GetName();
    FdoStringP colName = GetName();
    FdoSize tableLen = tableName.GetLength();
    FdoSize colLen = colName.GetLength();
    FdoSize suffixLen = suffix.GetLength();

    // Get maximum index name length
    FdoSize maxLen = GetManager()->DbObjectNameMaxLen();

    // Check if initial concatenated name would be too long 
    double aspect = ((double)(tableLen + colLen )) / ((double)(maxLen - suffixLen - 1));

    if ( aspect > 1 ) {
        // Too long so reduce table and column name parts proportionately
        tableLen = (int)(tableLen / aspect);
        colLen = (int)(tableLen / aspect);
    }

    // Create initial name
    // It might still be one or two characters too long, but 
    // UniqueDbObjectName will trim it if it is.
    FdoStringP indexName = FdoStringP::Format( 
        L"%ls_%ls%ls",
        (FdoString*) tableName.Mid(0, tableLen),
        (FdoString*) colName.Mid(0, colLen),
        (FdoString*) suffix
    );

    // Adjust it to be unique if the datastore has MetaSchema.
    indexName = ((FdoSmPhOwner*)owner)->UniqueDbObjectName( indexName );

    return indexName;
}

void FdoSmPhColumnGeom::Commit( bool fromParent, bool isBeforeParent  )
{
    FdoSmPhColumn::Commit( fromParent, isBeforeParent );

}

void FdoSmPhColumnGeom::Finalize()
{

   // Finalize is not re-entrant.
	if ( GetState() == FdoSmObjectState_Finalizing) {
		// Don't worry about errors for elements that are going away.
		if ( GetElementState() != FdoSchemaElementState_Deleted ) 
			AddFinalizeLoopError();
		return;
	}

	if ( GetState() == FdoSmObjectState_Initial ) {
		SetState( FdoSmObjectState_Finalizing);

        PostFinalize();

		SetState( FdoSmObjectState_Final);
    }
}

void FdoSmPhColumnGeom::PostFinalize()
{
}
