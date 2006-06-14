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
#include <Sm/Lp/SpatialContextGeom.h>
#include <Sm/Ph/SpatialContextReader.h>
#include <Sm/Ph/Rd/SpatialContextReader.h>
#include <Sm/Ph/SpatialContextGroupReader.h>
#include <Sm/Ph/SpatialContextGeomReader.h>


FdoSmLpSpatialContextCollection::FdoSmLpSpatialContextCollection(FdoSmPhMgrP physicalSchema) :
	mPhysicalSchema(physicalSchema),
    mAreLoaded(false)
{
}

FdoSmLpSpatialContextCollection::~FdoSmLpSpatialContextCollection(void)
{
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
        sc->Commit( true );
	}
}

FdoSchemaExceptionP FdoSmLpSpatialContextCollection::Errors2Exception(FdoSchemaException* pFirstException ) const
{
	FdoSchemaExceptionP pException = FDO_SAFE_ADDREF(pFirstException);

	for ( int i = 0; i < GetCount(); i++ )
		pException = RefItem(i)->Errors2Exception(pException);

	return pException;
}


void FdoSmLpSpatialContextCollection::Load()
{
	if ( (mPhysicalSchema != NULL) && !mAreLoaded ) {
        mAreLoaded = true;

		// Cache the SpatialContextGeomReader info  
		if ( mSpatialContextGeoms.p == NULL )
			mSpatialContextGeoms = new FdoSmLpSpatialContextGeomCollection();

        // Check if there is a config doc
        FdoIoStreamP configDoc = mPhysicalSchema->GetConfigDoc();
        FdoStringP providerName = mPhysicalSchema->GetProviderName();

        if ( configDoc ) {
            // There is a config doc, so load spatial contexts from it exclusively.
            configDoc->Reset();
	        FdoXmlReaderP reader = FdoXmlReader::Create(configDoc);

	        FdoXmlSpatialContextReaderP scReader = FdoXmlSpatialContextReader::Create(reader);

	        while (scReader->ReadNext())
	        {
				FdoSmLpSpatialContextP sc = NewSpatialContext(
                    scReader->GetName(),
                    scReader->GetDescription(),
                    scReader->GetCoordinateSystem(),
                    scReader->GetCoordinateSystemWkt(),
                    scReader->GetExtentType(),
                    scReader->GetExtent(),
                    scReader->GetXYTolerance(),
                    scReader->GetZTolerance(),
                    true,
                    mPhysicalSchema);

                if (NULL == sc.p)
					throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

                this->Add( sc );
	        }
        }
        else if ( mPhysicalSchema->GetOwner()->GetHasMetaSchema() )
			// No config document; load from metaschema or reverse-engineering/defaults.
        {
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

			// TODO: Caching doesn't work: apparenly when updating later in Logical Physical from SC=L””
			// to L”Default”, we get an error saying that the property is already finalized. 
			// See also FdoSmLpGeometricPropertyDefinition::FixSpatialContextAssociation()

			//FdoSmPhSpatialContextGeomReaderP scGeomReader = mPhysicalSchema->CreateSpatialContextGeomReader();
			//while (scGeomReader->ReadNext())
			//{
			//	// Create Spatial context geometry object and associate it with this scId
			//	FdoSmLpSpatialContextGeomP  scgeom = new FdoSmLpSpatialContextGeom(
			//													scGeomReader->GetScId(),
			//													scGeomReader->GetGeomTableName(),
			//													scGeomReader->GetGeomColumnName(),
			//													scGeomReader->GetDimensionality() );
			//  if (NULL == scgeom.p)
			//		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

			//	mSpatialContextGeoms->Add( scgeom );												
			//}
        }
		else
		{
			// reverse-engineering. The PH schema object will return the appropiate reader or
			// the default one.
            FdoSmPhRdSpatialContextReaderP scReader = mPhysicalSchema->CreateRdSpatialContextReader();
			
			FdoInt32	currSC = 0;

	        while (scReader->ReadNext())
	        {
				FdoStringP	scName = L"Default";
				
				if ( currSC != 0 )
					scName = FdoStringP::Format(L"%ls_%ld", scReader->GetName(), currSC);

				FdoSmLpSpatialContextP sc = NewSpatialContext(
                    scName,
                    scReader->GetDescription(),
                    scReader->GetCoordinateSystem(),
                    scReader->GetCoordinateSystemWkt(),
                    scReader->GetExtentType(),
                    scReader->GetExtent(),
                    scReader->GetXYTolerance(),
                    scReader->GetZTolerance(),
                    true,
                    mPhysicalSchema);

				sc->SetSrid( scReader->GetSrid() );

				// Avoid cs_name etc. validation in Finalize() against catalog
				sc->SetState( FdoSmObjectState_Final );

                if (NULL == sc.p)
					throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

				int indexSC = FindExistingSC( sc );

				// New Spatial context definition, add it to collection
				if ( indexSC == -1 )
				{
					sc->SetId( currSC ); 
					this->Add( sc );
					currSC++;
				}

				// Create Spatial context geometry object and associate it with this scId
				FdoSmLpSpatialContextGeomP  scgeom = new FdoSmLpSpatialContextGeom(
																( indexSC != -1 )? indexSC : currSC - 1,
																scReader->GetGeomTableName(),
																scReader->GetGeomColumnName(),
																-1);
                if (NULL == scgeom.p)
					throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

				mSpatialContextGeoms->Add( scgeom );												
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
	return FDO_SAFE_ADDREF(mSpatialContextGeoms.p);
}

FdoInt32 FdoSmLpSpatialContextCollection::FindExistingSC( FdoSmLpSpatialContextP sc )
{
	FdoInt32 index = -1;

	for (int i = 0; i < this->GetCount() && (index == -1); i++ )
	{
		FdoSmLpSpatialContextP  sc1 = this->GetItem(i);

		if ( ( sc1->GetSrid() == sc1->GetSrid() ) &&
			 ( wcscmp(sc1->GetCoordinateSystem(), sc->GetCoordinateSystem()) == 0 ) &&
			 ( wcscmp(sc1->GetCoordinateSystemWkt(), sc->GetCoordinateSystemWkt()) == 0 ) &&
			 ( sc1->GetXYTolerance() == sc->GetXYTolerance() ) &&
			 ( sc1->GetZTolerance() == sc->GetZTolerance() ) )
		{
			FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

			FdoPtr<FdoIGeometry>          geom = gf->CreateGeometryFromFgf( sc->GetExtent() );
			FdoPtr<FdoIEnvelope>		  env = geom->GetEnvelope();
			FdoPtr<FdoIGeometry>          geom1= gf->CreateGeometryFromFgf( sc1->GetExtent() );
			FdoPtr<FdoIEnvelope>		  env1 = geom1->GetEnvelope();

			bool found = ( env1->GetMinX() == env->GetMinX() &&
						   env1->GetMaxX() == env->GetMaxX() &&
						   env1->GetMinY() == env->GetMinY() &&
						   env1->GetMaxY() == env->GetMaxY() );
			if ( found )
				index = i;
		}
	}
	return (index);
}
