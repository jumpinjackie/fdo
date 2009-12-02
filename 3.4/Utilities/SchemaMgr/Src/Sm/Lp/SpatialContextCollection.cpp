 //
 // Copyright (C) 2004-2006  Autodesk, Inc.
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

#include "stdafx.h"
#include <Sm/Error.h>
#include <Sm/Lp/SpatialContextCollection.h>
#include <Sm/Ph/SpatialContextReader.h>
#include <Sm/Ph/Rd/SpatialContextReader.h>
#include <Sm/Ph/SpatialContextGroupReader.h>
#include <Sm/Ph/SpatialContextGeomReader.h>
#include <FdoCommonStringUtil.h>


FdoSmLpSpatialContextCollection::FdoSmLpSpatialContextCollection(FdoSmPhMgrP physicalSchema) :
	mPhysicalSchema(physicalSchema),
    mAreLoaded(false)
{
    mSpatialContextGeoms = new FdoSmLpSpatialContextGeomCollection();
    mIdMap = FdoDictionary::Create();
}

FdoSmLpSpatialContextCollection::~FdoSmLpSpatialContextCollection(void)
{
}

FdoSmLpSpatialContextP FdoSmLpSpatialContextCollection::FindSpatialContext( FdoInt64 scId )
{
    // Check if already in cache
    FdoSmLpSpatialContextP sc = FindItemById(scId);

    if ( !sc ) 
    {
        // Not in cache, load the spatial context and try again.
        Load(scId);
        sc = FindItemById(scId);
    }

    return(sc);
}

FdoSmLpSpatialContextP FdoSmLpSpatialContextCollection::FindSpatialContext( FdoStringP scName )
{
    // Check if already in cache
    FdoSmLpSpatialContextP sc = FindItem(scName);

    if ( !sc ) 
    {
        // Not in cache, load the spatial contexts and try again.
        Load();
        sc = FindItem(scName);
    }

    return(sc);
}

FdoSmLpSpatialContextGeomP FdoSmLpSpatialContextCollection::FindSpatialContextGeom( FdoStringP dbObjectName, FdoStringP columnName )
{
    FdoStringP scGeomName = FdoSmLpSpatialContextGeom::MakeName( dbObjectName, columnName );

    // Check if already in cache
    FdoSmLpSpatialContextGeomP scGeom = mSpatialContextGeoms->FindItem( scGeomName );

    if ( !scGeom ) 
    {
        // Not in cache

        // Check if there is a config doc
        FdoIoStreamP configDoc = mPhysicalSchema->GetConfigDoc();
        FdoSmPhOwnerP owner = mPhysicalSchema->GetOwner();

        if ( (!configDoc) && (!owner->GetHasMetaSchema()) ) 
        {
            // Non-FDO datastore without config doc. Get spatial context geom from physical schema
            // For performance, we don't do full load of SCGeoms here, but leave it to the
            // physical schema to decide when full load is needed. 
            FdoSmPhSpatialContextGeomP phScGeom = owner->FindSpatialContextGeom( dbObjectName, columnName );

            if ( phScGeom ) 
            {
                // Make sure we can link up this SCGeom with its Spatial Context.
                FdoSmPhSpatialContextP phSc = phScGeom->GetSpatialContext();

                if ( phSc ) 
                {
				    // Physical SCGeom found. Create and cache a corresponding LogicalPhysical SCGeom.
				    scGeom = new FdoSmLpSpatialContextGeom(
				        								   phSc->GetId(),               // LogicalPhysical and Physical spatial contexts have same id's
														   phScGeom->GetGeomTableName(),
														   phScGeom->GetGeomColumnName(),
														   phScGeom->GetHasElevation(),
														   phScGeom->GetHasMeasure());
			        if (NULL == scGeom.p)
					    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

				    mSpatialContextGeoms->Add( scGeom );	
                }
            }
        }
        else 
        {
            // When schema defined in config doc or metaschema, do full load 
            // try again to find the scgeom.
            Load();
            scGeom = mSpatialContextGeoms->FindItem( scGeomName );
        }
    }

    return(scGeom);
}

FdoSmLpSpatialContextP FdoSmLpSpatialContextCollection::FindItemById( FdoInt64 scid )
{
    FdoSmLpSpatialContextP sc;

    // Use ID Map to convert id to name and then look up sc by name

    FdoDictionaryElementP elem = mIdMap->FindItem( FdoCommonStringUtil::Int64ToString(scid) );

    if ( elem ) {
        FdoStringP scName = elem->GetValue();

        sc = FindItem(scName);
    }

    return sc;
}

FdoInt32 FdoSmLpSpatialContextCollection::Add( FdoSmLpSpatialContext* value)
{
    FdoInt32 ret = FdoSmNamedCollection<FdoSmLpSpatialContext>::Add(value);

    // Keep id map synchronized
    AddToIdMap(value);

    return ret;
}

FdoSmLpSpatialContextP FdoSmLpSpatialContextCollection::AddFromPhysical( FdoSmPhSpatialContextP phSc )
{
    FdoSmLpSpatialContextP sc;

    if ( IndexOf(phSc->GetName() ) < 0 )
    {
        FdoPtr<FdoByteArray> pExtVal = phSc->GetExtent();
        sc = NewSpatialContext(
            phSc->GetName(),
            phSc->GetDescription(),
            phSc->GetCoordinateSystem(),
            phSc->GetCoordinateSystemWkt(),
            phSc->GetExtentType(),
            pExtVal.p,
            phSc->GetXYTolerance(),
            phSc->GetZTolerance(),
            true,
            mPhysicalSchema);

        if (NULL == sc.p)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
        
        sc->SetSrid( phSc->GetSrid() );

        // Avoid cs_name etc. validation in Finalize() against catalog
        sc->SetState( FdoSmObjectState_Final );

        sc->SetId( phSc->GetId() ); 

        this->Add( sc );
    }

    return sc;
}


FdoSmLpSpatialContextP FdoSmLpSpatialContextCollection::CreateSpatialContext(
        FdoString* name,
        FdoString* description,
        FdoString* coordinateSystem,
        FdoString* coordinateSystemWkt,
        FdoSpatialContextExtentType extentType,
        FdoByteArray * extent,
        double xyTolerance,
        double zTolerance,
        bool bIgnoreStates
)
{
    FdoSmLpSpatialContextP sc = NewSpatialContext(
        name, description,
        coordinateSystem, coordinateSystemWkt,
        extentType, extent,
        xyTolerance, zTolerance,
        bIgnoreStates,
        mPhysicalSchema);
    if (NULL == sc.p)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    return sc;
}

void FdoSmLpSpatialContextCollection::Commit()
{
	for ( int i = 0; i < GetCount(); i++ )
    {
        FdoSmLpSpatialContextP sc = GetItem(i);

        FdoSchemaElementState elemState = sc->GetElementState();
        FdoInt64 oldScid = sc->GetId();
        sc->Commit( true );

        if ( elemState == FdoSchemaElementState_Deleted ) {
            // Remove deleted SC's from id map
            RemoveFromIdMap( sc );
        }
        else
        {
            if ( oldScid == -1 ) 
                // Commit can assigns an id to new sc so add it to the id map
                AddToIdMap( sc );
        }
	}
}

FdoSchemaExceptionP FdoSmLpSpatialContextCollection::Errors2Exception(FdoSchemaException* pFirstException ) const
{
	FdoSchemaExceptionP pException = FDO_SAFE_ADDREF(pFirstException);

	for ( int i = 0; i < GetCount(); i++ )
		pException = RefItem(i)->Errors2Exception(pException);

	return pException;
}


void FdoSmLpSpatialContextCollection::Load( FdoInt64 scId )
{
    FdoInt32 idx;
    FdoSmPhSpatialContextsP phScs;
    FdoSmPhSpatialContextGeomsP scGeoms;

	if ( (mPhysicalSchema != NULL) && !mAreLoaded ) {
        // Check if there is a config doc
        FdoIoStreamP configDoc = mPhysicalSchema->GetConfigDoc();
        FdoStringP providerName = mPhysicalSchema->GetProviderName();

        if ( configDoc ) {
            mAreLoaded = true;

            // Calculate next ScId as max(current scid's) + 1. next ScId is 
            // used to assign unique id to spatial contexts read from config doc.
            FdoInt64 nextScId = 0;
            int idx;

            for ( idx = 0; idx < GetCount(); idx++ )
            {
                FdoSmLpSpatialContextP currSc = GetItem(idx);
                FdoInt64 currScId = currSc->GetId();

                if ( currScId >= nextScId )
                    nextScId = currScId + 1;
            }

            // There is a config doc, so load spatial contexts from it exclusively.
            configDoc->Reset();
	        FdoXmlReaderP reader = FdoXmlReader::Create(configDoc);

	        FdoXmlSpatialContextReaderP scReader = FdoXmlSpatialContextReader::Create(reader);

	        while (scReader->ReadNext())
	        {
                FdoPtr<FdoByteArray> pExtVal = scReader->GetExtent();
				FdoSmLpSpatialContextP sc = NewSpatialContext(
                    scReader->GetName(),
                    scReader->GetDescription(),
                    scReader->GetCoordinateSystem(),
                    scReader->GetCoordinateSystemWkt(),
                    scReader->GetExtentType(),
                    pExtVal.p,
                    scReader->GetXYTolerance(),
                    scReader->GetZTolerance(),
                    true,
                    mPhysicalSchema);

                if (NULL == sc.p)
					throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

                sc->SetId( nextScId++ );
                this->Add( sc );
	        }
        }
        else if ( FdoSmPhOwnerP(mPhysicalSchema->GetOwner())->GetHasMetaSchema() )
			// No config document; load from metaschema or reverse-engineering/defaults.
        {
            mAreLoaded = true;

            // This is where we merge physical spatial contexts and spatial context groups
            // into logical/physical spatial contexts.  Spatial context groups will not be
            // seen above this point in the code.  Currently, the relationship between
            // these two objects is Many:1.

            FdoSmPhSpatialContextReaderP scReader = mPhysicalSchema->CreateSpatialContextReader();
            FdoSmPhSpatialContextGroupReaderP scgReader = mPhysicalSchema->CreateSpatialContextGroupReader();

		    // Match the readers by SCGID. The readers return the rows ordered by SCGID.
		    FdoInt64 currScgid = -1;
            while ( scReader->ReadNext() )
            {
			    // Advance the spatial context group reader
			    while ((currScgid != scReader->GetGroupId()) && scgReader->ReadNext() ) 
			    {
				    currScgid = scgReader->GetId();
			    }

			    if ( scReader->GetGroupId() == currScgid  ) 
			    {
				    FdoSmLpSpatialContextP sc = NewSpatialContext( scReader, scgReader, mPhysicalSchema );
				    if (NULL == sc.p)
					    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

				    this->Add( sc );
			    }
			    else 
				    throw FdoSchemaException::Create( 
					    FdoSmError::NLSGetMessage(
						    //Spatial Context '%1$ls' is missing a corresponding Coordinate System
						    FDO_NLSID(FDOSM_413),
						    (FdoString *)scReader->GetName()
					    )
				    );
	        }

			// Load the Spatial Context Geometry associations into the cache.

			FdoSmPhSpatialContextGeomReaderP scGeomReader = mPhysicalSchema->CreateSpatialContextGeomReader();
			while (scGeomReader->ReadNext())
			{
				// Create Spatial context geometry object and associate it with this scId
				FdoSmLpSpatialContextGeomP  scgeom = new FdoSmLpSpatialContextGeom(
																scGeomReader->GetScId(),
																scGeomReader->GetGeomTableName(),
																scGeomReader->GetGeomColumnName(),
																(scGeomReader->GetDimensionality() & FdoDimensionality_Z) != 0,
																(scGeomReader->GetDimensionality() & FdoDimensionality_M) != 0);
			  if (NULL == scgeom.p)
					throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

				mSpatialContextGeoms->Add( scgeom );												
			}
        }
		else
		{
			// Create a LogicalPhysical spatial context from each Physical spatial context

            if ( scId >= 0) 
            {
                // Just load the specified Spatial Context
                FdoSmPhSpatialContextP phSc = mPhysicalSchema->GetOwner()->FindSpatialContext(scId);            
                if ( phSc ) 
                    AddFromPhysical(phSc);
            }
            else 
            {
                // No Spatial Context specified, load all of them.
                mAreLoaded = true;
                phScs = mPhysicalSchema->GetOwner()->GetSpatialContexts();
    			
			    FdoInt32	currSC = 0;

                for ( idx = 0; idx <  phScs->GetCount(); idx++ ) 
                {
                    FdoSmPhSpatialContextP phSc = phScs->GetItem( idx );
	                AddFromPhysical(phSc);
                }
            }
		}
	}
}


FdoSmLpSpatialContextP FdoSmLpSpatialContextCollection::NewSpatialContext(
    FdoSmPhSpatialContextReaderP scReader,
    FdoSmPhSpatialContextGroupReaderP scgReader,
    FdoSmPhMgrP physicalSchema)
{
    FdoSmLpSpatialContextP sc = new FdoSmLpSpatialContext(
        scReader, scgReader, mPhysicalSchema );

    return sc;
}


FdoSmLpSpatialContextP FdoSmLpSpatialContextCollection::NewSpatialContext(
        FdoString* name,
        FdoString* description,
        FdoString* coordinateSystem,
        FdoString* coordinateSystemWkt,
        FdoSpatialContextExtentType extentType,
        FdoByteArray * extent,
        double xyTolerance,
        double zTolerance,
        bool bIgnoreStates,
        FdoSmPhMgrP physicalSchema
)
{
    FdoSmLpSpatialContextP sc = new FdoSmLpSpatialContext(
        name, description,
        coordinateSystem, coordinateSystemWkt,
        extentType, extent,
        xyTolerance, zTolerance,
        physicalSchema);

    return sc;
}

FdoSmLpSpatialContextGeomsP FdoSmLpSpatialContextCollection::GetSpatialContextGeoms()
{
    return mSpatialContextGeoms;
}

void FdoSmLpSpatialContextCollection::AddToIdMap( FdoSmLpSpatialContext* sc )
{
    if ( sc->GetId() > -1 ) {
        FdoStringP idKey = FdoCommonStringUtil::Int64ToString( sc->GetId() );
        FdoStringP idVal = sc->GetName();

        FdoDictionaryElementP elem = FdoDictionaryElement::Create( idKey, idVal );

        mIdMap->Add( elem );
    }
}

void FdoSmLpSpatialContextCollection::RemoveFromIdMap( FdoSmLpSpatialContext* sc )
{
    FdoStringP idKey = FdoCommonStringUtil::Int64ToString( sc->GetId() );
    FdoInt32 ix = mIdMap->IndexOf( idKey );

    if ( ix > -1 ) 
        mIdMap->RemoveAt( ix );
}
