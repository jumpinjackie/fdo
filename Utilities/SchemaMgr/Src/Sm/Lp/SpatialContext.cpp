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
#include <Sm/Lp/SpatialContext.h>
#include <Sm/Error.h>
#include <FdoCommonStringUtil.h>
#include <Sm/Ph/Rd/CoordSysReader.h>

FdoSmLpSpatialContext::FdoSmLpSpatialContext(
    FdoSmPhSpatialContextReaderP scReader,
    FdoSmPhSpatialContextGroupReaderP scgReader,
    FdoSmPhMgrP physicalSchema)
    : 
	FdoSmLpSchemaElement(scReader->GetName(), scReader->GetDescription(), NULL),
	mPhysicalSchema(physicalSchema),
    mId(scReader->GetId()),
    mScgId(scgReader->GetId()),
    mCoordSysName(scgReader->GetCrsName()),
    mCoordSysWkt(scgReader->GetCrsWkt()),
	mSrid(scgReader->GetSrid()),
    mXYTolerance(scgReader->GetXTolerance()),
    mZTolerance(scgReader->GetZTolerance())
{
    // Check that these two readers match.
    if (mScgId != scReader->GetGroupId())
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_135_SPATIAL_CONTEXT_ERROR_ID_MISSMATCH),
                                                               L"FdoSmLpSpatialContext::FdoSmLpSpatialContext"));

    // Convert the extent type from string to enumeration.

    FdoStringP extentType = scgReader->GetExtentType();
    if (extentType == L"D")
        mExtentType = FdoSpatialContextExtentType_Dynamic;
    else if (extentType == L"S")
        mExtentType = FdoSpatialContextExtentType_Static;
    else
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_136_SPATIAL_CONTEXT_ERROR_UNKNOWN_EXTENT_TYPE),
                                                               L"FdoSmLpSpatialContext::FdoSmLpSpatialContext"));

    // Convert the extent from basic types to a geometry in FGF format.
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIEnvelope>          env = gf->CreateEnvelopeXY(
                                                scgReader->GetXMin(),
                                                scgReader->GetYMin(),
                                                scgReader->GetXMax(),
                                                scgReader->GetYMax());
    FdoPtr<FdoIGeometry> geom = gf->CreateGeometry(env); 
    
    FdoPtr<FdoByteArray>ext = gf->GetFgf(geom);
    SetExtent(ext);

    ext = NULL;
}

FdoSmLpSpatialContext::FdoSmLpSpatialContext(
    FdoString* name,
    FdoString* description,
    FdoString* coordinateSystem,
    FdoString* coordinateSystemWkt,
    FdoSpatialContextExtentType extentType,
    FdoByteArray * extent,
    double xyTolerance,
    double zTolerance,
    FdoSmPhMgrP physicalSchema
) : 
	FdoSmLpSchemaElement(name, description, NULL),
	mPhysicalSchema(physicalSchema),
    mId(-1),
    mScgId(-1),
	mSrid(0),
    mCoordSysName(coordinateSystem),
    mCoordSysWkt(coordinateSystemWkt),
    mExtentType(extentType),
    mXYTolerance(xyTolerance),
    mZTolerance(zTolerance)
{
    mExtent = FDO_SAFE_ADDREF(extent);
}


FdoSmLpSpatialContext::~FdoSmLpSpatialContext(void)
{
}


FdoInt64 FdoSmLpSpatialContext::GetId() const
{
    ((FdoSmLpSpatialContext*) this)->Finalize();

	return mId;
}

FdoString* FdoSmLpSpatialContext::GetCoordinateSystem()
{
    Finalize();

    return mCoordSysName;
}

FdoString* FdoSmLpSpatialContext::GetCoordinateSystemWkt()
{
    Finalize();

    return Get_CoordinateSystemWkt();
}

FdoSpatialContextExtentType FdoSmLpSpatialContext::GetExtentType()
{
    Finalize();

    return mExtentType;
}

FdoByteArray* FdoSmLpSpatialContext::GetExtent()
{
    Finalize();

    // Make a copy of the extent byte array as the caller may insist on putting it in 
    // the geometry pool which may cause it to be shared by aother threads.
    // the extent byte array is not in the pool and should not be in there as the spatial context
    // can be used by many threads.
    FdoByteArray* ext = NULL;
    if( mExtent )
        ext = FdoByteArray::Create( mExtent->GetData(), mExtent->GetCount() );

    return ext;
}

double FdoSmLpSpatialContext::GetXYTolerance()
{
    Finalize();

    return mXYTolerance;
}

double FdoSmLpSpatialContext::GetZTolerance()
{
    Finalize();

    return mZTolerance;
}


void FdoSmLpSpatialContext::SetElementState(FdoSchemaElementState elementState)
{
	// Set state for this element.
	FdoSmSchemaElement::SetElementState( elementState );
}

void FdoSmLpSpatialContext::Commit( bool fromParent )
{
    FdoSmPhSpatialContextWriterP scWriter;
    FdoSmPhSpatialContextGroupWriterP scgWriter;

    Finalize();

    // Here is where we split a logical/physical spatial context
    // into physical spatial context and spatial context group.

    if ( mPhysicalSchema->FindOwner()->GetHasMetaSchema() ) {
	    switch ( GetElementState() ) {
  	    case FdoSchemaElementState_Added:

		    // Do not duplicate SC group definitions. Find an existing one.
		    if ( (mScgId = GetMatchingScgid()) == -1 ) {
			    scgWriter = GetPhysicalScgAddWriter();
			    scgWriter->Add();
			    mScgId = scgWriter->GetId();
		    }
            scWriter = GetPhysicalScAddWriter();
            scWriter->Add();
		    mId = scWriter->GetId();

		    break;

	    case FdoSchemaElementState_Deleted:
		    // ToDo: remove a SCG if and only if there is not SC associated with it
            //scgWriter = GetPhysicalScgAddWriter();
            //scgWriter->Delete( mScgId );
            scWriter = GetPhysicalScAddWriter();
            scWriter->Delete( GetId() );

		    break;

	    case FdoSchemaElementState_Modified:
            scgWriter = GetPhysicalScgModifyWriter();
            scgWriter->Modify( mScgId );
            scWriter = GetPhysicalScModifyWriter();
            scWriter->Modify( mId );

            break;
	    }
    }
    else {
        // Datastore has no MetaSchema, so there is a chicken and egg situation:
        //  - spatial contexts is reverse-engineered from geometric columns so spatial context
        //    can't exist if not referenced by a geometric column.
        //  - spatial context must exist before it can be associated with a geometric column.
        //
        // This is resolved by creating a special table with one geometric column per spatial 
        // context that was created by FDO.

        // TODO: centralize the special table handling in the Physical Schema Manager.

        FdoSmPhOwnerP owner = mPhysicalSchema->FindOwner();

	    switch ( GetElementState() ) {
  	    case FdoSchemaElementState_Added:
            AddNoMeta();
            break;

	    case FdoSchemaElementState_Deleted:
            DeleteNoMeta();
            break;

	    case FdoSchemaElementState_Modified:
            DeleteNoMeta();
            AddNoMeta();
            break;
	    }
    }
}

FdoSchemaExceptionP FdoSmLpSpatialContext::Errors2Exception(FdoSchemaException* pFirstException ) const
{
    ((FdoSmLpSpatialContext*) this)->Finalize();

	// Need to completely load the object to discover all errors.
	((FdoSmLpSpatialContext *) this)->Finalize();

	// Tack on errors for this element
	FdoSchemaExceptionP pException = FdoSmSchemaElement::Errors2Exception(pFirstException);

	return pException;
}

FdoSmPhSpatialContextWriterP FdoSmLpSpatialContext::GetPhysicalScAddWriter()
{
    FdoSmPhSpatialContextWriterP pWriter = mPhysicalSchema->GetSpatialContextWriter();

    pWriter->SetName( GetName() );
    pWriter->SetDescription( GetDescription() );
    pWriter->SetGroupId( mScgId );

    return( pWriter );
}

FdoSmPhSpatialContextWriterP FdoSmLpSpatialContext::GetPhysicalScModifyWriter()
{
    FdoSmPhSpatialContextWriterP pWriter = mPhysicalSchema->GetSpatialContextWriter();

    // Description is the only modifiable thing.
	pWriter->SetDescription( GetDescription() );

    return( pWriter );
}


FdoSmPhSpatialContextGroupWriterP FdoSmLpSpatialContext::GetPhysicalScgAddWriter()
{
    FdoSmPhSpatialContextGroupWriterP pWriter = mPhysicalSchema->GetSpatialContextGroupWriter();

	pWriter->SetCrsName( mCoordSysName );
    pWriter->SetCrsWkt( mCoordSysWkt );
	pWriter->SetSrid( mSrid );
	
    pWriter->SetXTolerance( mXYTolerance );
    pWriter->SetZTolerance( mZTolerance );

    if (mExtentType == FdoSpatialContextExtentType_Dynamic)
        pWriter->SetExtentType( L"D" );
    else
        pWriter->SetExtentType( L"S" );

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIGeometry>   geom = gf->CreateGeometryFromFgf( mExtent );
    FdoPtr<FdoIEnvelope>   env = geom->GetEnvelope();

    pWriter->SetXMin(env->GetMinX());
    pWriter->SetYMin(env->GetMinY());
    pWriter->SetXMax(env->GetMaxX());
    pWriter->SetYMax(env->GetMaxY());

    // TODO: Check consequences of unset Z extent.
    pWriter->SetZMin(env->GetMinZ());
    pWriter->SetZMax(env->GetMaxZ());

    return( pWriter );
}

FdoInt64  FdoSmLpSpatialContext::GetMatchingScgid()
{
	FdoInt64	scgid = -1;

    FdoSmPhSpatialContextGroupReaderP scgReader = mPhysicalSchema->CreateSpatialContextGroupReader();

	FdoStringP extentType = (mExtentType == FdoSpatialContextExtentType_Dynamic)? L"D" : L"S";
 
	while ( scgReader->ReadNext() && ( scgid == -1 )) {

		if ( ( scgReader->GetCrsName() != mCoordSysName ) ||
			 ( wcscmp( scgReader->GetCrsWkt(), mCoordSysWkt ) != 0 ) ||
			 ( scgReader->GetExtentType() != extentType ) ||
			 ( scgReader->GetXTolerance() != mXYTolerance ) ||
			 ( scgReader->GetZTolerance() != mZTolerance ) )
			continue;
		
		FdoPtr<FdoFgfGeometryFactory>	gf = FdoFgfGeometryFactory::GetInstance();
		FdoPtr<FdoIGeometry>			geom = gf->CreateGeometryFromFgf( mExtent );
		FdoPtr<FdoIEnvelope>			env = geom->GetEnvelope();

		if ( ( scgReader->GetXMin() == env->GetMinX() ) &&
			 ( scgReader->GetYMin() == env->GetMinY() ) &&
			 ( scgReader->GetXMax() == env->GetMaxX() ) &&
			 ( scgReader->GetYMax() == env->GetMaxY() ) ) {

			scgid = scgReader->GetId();
		}
	}
	return scgid;
}

FdoSmPhSpatialContextGroupWriterP FdoSmLpSpatialContext::GetPhysicalScgModifyWriter()
{
    FdoSmPhSpatialContextGroupWriterP pWriter = mPhysicalSchema->GetSpatialContextGroupWriter();

    // Only extent and tolerance are modifiable.
    // TODO: in some providers, drop and re-create spatial index.

    pWriter->SetXTolerance( mXYTolerance );
    pWriter->SetZTolerance( mZTolerance );

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIGeometry>   geom = gf->CreateGeometryFromFgf( mExtent );
    FdoPtr<FdoIEnvelope>   env = geom->GetEnvelope();

    pWriter->SetXMin(env->GetMinX());
    pWriter->SetYMin(env->GetMinY());
    pWriter->SetXMax(env->GetMaxX());
    pWriter->SetYMax(env->GetMaxY());

    // TODO: Check consequences of unset Z extent.
    pWriter->SetZMin(env->GetMinZ());
    pWriter->SetZMax(env->GetMaxZ());

    return( pWriter );
}

FdoSmPhMgrP FdoSmLpSpatialContext::GetPhysicalSchema()
{
    return mPhysicalSchema;
}

FdoString* FdoSmLpSpatialContext::Get_CoordinateSystemWkt()
{
    return mCoordSysWkt;
}

void FdoSmLpSpatialContext::SetId(FdoInt64 id)
{
	mId = id;
}

void FdoSmLpSpatialContext::SetCoordinateSystemName( FdoString* name )
{
    mCoordSysName = name;
}

void FdoSmLpSpatialContext::SetCoordinateSystemWkt( FdoString* wkt )
{
    mCoordSysWkt = wkt;
}

FdoInt64 FdoSmLpSpatialContext::GetSrid()
{
	return mSrid;
}

void FdoSmLpSpatialContext::SetSrid( FdoInt64 srid )
{
	mSrid = srid;
}

void FdoSmLpSpatialContext::SetExtent( FdoByteArray* ext )
{
    if( ext ) // make a copy of the byte array as the one obtained from the factory is not thread safe.
        mExtent = FdoByteArray::Create( ext->GetData(), ext->GetCount() );
}

void FdoSmLpSpatialContext::AddNoMeta()
{
    FdoSmPhOwnerP owner = mPhysicalSchema->FindOwner();

    if ( owner ) 
    {
        // When datastore has no MetaSchema, a spatial context cannot exist without
        // a referencing column. The following adds this column. These columns are 
        // kept in a special ScInfo table.

        // First find the special table.
        FdoSmPhDbObjectP dbObject = owner->FindDbObject(FdoSmPhMgr::ScInfoNoMetaTable);

        if ( !dbObject ) 
        {
            // Special table doesn't yet exist, create it.
            dbObject = owner->CreateTable(FdoSmPhMgr::ScInfoNoMetaTable);
            FdoSmPhColumnP column = dbObject->CreateColumnInt64(FdoSmPhMgr::ScInfoNoMetaPKey, false);
            dbObject->AddPkeyCol(column->GetName());
        }

        // Add a column that is associated with this Spatial Context.
        FdoSmPhScInfoP scinfo = FdoSmPhScInfo::Create();
        scinfo->mSrid = GetSrid();
        scinfo->mCoordSysName = GetCoordinateSystem();
        scinfo->mExtent = GetExtent();
        scinfo->mXYTolerance = GetXYTolerance();
        scinfo->mZTolerance = GetZTolerance();

        dbObject->CreateColumnGeom( GetName(), scinfo );
        dbObject->Commit();
    }
}

void FdoSmLpSpatialContext::DeleteNoMeta()
{
    FdoSmPhOwnerP owner = mPhysicalSchema->FindOwner();

    if ( owner ) 
    {
        FdoSmPhDbObjectP dbObject = owner->FindDbObject(mPhysicalSchema->GetRealDbObjectName(FdoSmPhMgr::ScInfoNoMetaTable));

        if ( dbObject ) 
        {
            // Remove this spatial context's column from the special table.
            FdoSmPhColumnP column = dbObject->GetColumns()->FindItem(GetName());

            if ( column  ) {
                FdoSmPhColumnGeomP columnGeom = column->SmartCast<FdoSmPhColumnGeom>();

                if ( columnGeom ) {
                    column->SetElementState( FdoSchemaElementState_Deleted );
                    dbObject->Commit();
                }
            }
        }
    }
}


void FdoSmLpSpatialContext::Finalize()
{
	// Finalize is not re-entrant.
	if ( GetState() == FdoSmObjectState_Finalizing)
    {
		// Don't worry about errors for elements that are going away.
		if ( GetElementState() != FdoSchemaElementState_Deleted ) 
			AddFinalizeLoopError();
		return;
	}

    // Not really much to do here (yet).
	if ( GetState() == FdoSmObjectState_Initial )
    {
		SetState( FdoSmObjectState_Final);

        PreFinalize();

        if ( GetElementState() == FdoSchemaElementState_Added ) {
            FdoSmPhOwnerP owner = mPhysicalSchema->FindOwner();
        
            if ( owner ) 
            {
                if ( !owner->GetHasMetaSchema() )
                {
                    // When datastore has no MetaSchema a referencing column is 
                    // created for this spatial context (see AddNoMeta()).
                    // The column is named after the spatial context.
                    //
                    // Log errors if the spatial context name is not a valid column
                    // and or is too long. We cannot mangle the column name because that
                    // would effectively change the spatial context name.

                    if ( owner->GetManager()->CensorDbObjectName(GetName()) != GetName() ) 
            			AddNoMetaNameChangeError( owner );
                
                    if ( wcslen(GetName()) > owner->GetManager()->ColNameMaxLen() ) 
            			AddNoMetaNameLengthError( owner, owner->GetManager()->ColNameMaxLen() );
                }

                // Match Spatial Context coordinate system to coordinate system in datastore.

                FdoSmPhCoordinateSystemP csys;

                FdoSmPhMgr::CoordinateSystemMatchLevel matchLevel = mPhysicalSchema->GetCoordinateSystemMatchLevel(); 
                bool nameMatched = false;
                bool matchError = false;

                // Try match by name first.
                if ( mCoordSysName != L"" ) 
                {
                    csys = owner->FindCoordinateSystem( mCoordSysName );

                    if ( csys )
                    {
                        nameMatched = true;

                        if ( mCoordSysWkt == L"" ) 
                        {
                            // WKT not specified so set it from matched coordinate system.
                            SetCoordinateSystemWkt( csys->GetWkt() );
                        }
                        else if ( mCoordSysWkt != csys->GetWkt() )
                        {
                            // WKT mismatch so this is not the right coordinate system.
                            // This is an error when strict matching is enforced by the provider.
                            // Otherwise, try to match by WKT.
                            nameMatched = false;
                            if ( matchLevel == FdoSmPhMgr::CoordinateSystemMatchLevel_Strict )
                            {
                                AddMismatchedWktError();
                                matchError = true;
                            }
                        }

                        if ( nameMatched ) 
                            SetSrid( csys->GetSrid() );
                    }
                    else 
                    {
                        if ( matchLevel == FdoSmPhMgr::CoordinateSystemMatchLevel_Strict ) 
                        {
                            // When strict matching is enforced by the provider,
                            // log error if name specified but doesn't match a 
                            // datastore coordinate system.
                            // Otherwise, try to match by WKT.
                            AddNoCsysError();
                            matchError = true;
                        }
                    }
                }

                if ( !nameMatched && !matchError ) 
                {
                    if ( mCoordSysWkt != L"" )
                    {
                        csys = owner->FindCoordinateSystemByWkt( mCoordSysWkt );
                        if ( csys )
                        {
                            SetSrid( csys->GetSrid() );
                            SetCoordinateSystemName( csys->GetName() );
                        }
                        else 
                        {
                            // WKT match failure is an error unless the provider doesn't mind.
                            if ( (matchLevel == FdoSmPhMgr::CoordinateSystemMatchLevel_Strict) ||
                                 (matchLevel == FdoSmPhMgr::CoordinateSystemMatchLevel_Wkt)
                            ) 
                                AddNoWktError();
                        }
                    }
                    else
                    {
                        // Name match failed and no wkt specified. This is an error unless provider doesn't mind.
                        if ( !nameMatched && (mCoordSysName != L"") && (matchLevel != FdoSmPhMgr::CoordinateSystemMatchLevel_Lax) ) 
                            AddNoCsysError();
                    }
                }
            }
        }

        PostFinalize();
	}
}

void FdoSmLpSpatialContext::PreFinalize()
{
}

void FdoSmLpSpatialContext::PostFinalize()
{
}

void FdoSmLpSpatialContext::AddNoMetaNameChangeError( FdoSmPhOwnerP owner)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				SM_NLSID(0x000008C4L, "Cannot create spatial context '%1$ls' in datastore '%2$ls'; datastore has no FDO metadata tables so spatial context name must be a valid column name"),
				(FdoString*) GetName(),
                owner ? owner->GetName() : L""
			)
		)
	);
}

void FdoSmLpSpatialContext::AddNoMetaNameLengthError( FdoSmPhOwnerP owner, FdoSize maxLen)
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				SM_NLSID(0x000008C5L, "Cannot create spatial context '%1$ls' in datastore '%2$ls'; datastore has no FDO metadata tables amd spatial context name exceeds %3$d characters"),
				(FdoString*) GetName(),
                owner ? owner->GetName() : L"",
                maxLen
			)
		)
	);
}

void FdoSmLpSpatialContext::AddNoCsysError()
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				SM_NLSID(0x000008C6L, "Error creating spatial context %1$ls, coordinate system %2$ls is not in current datastore."),
                GetName(), 
                (FdoString*) mCoordSysName
			)
		)
	);
}

void FdoSmLpSpatialContext::AddNoWktError()
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				SM_NLSID(0x000008C7L, "Error creating spatial context %1$ls, coordinate system catalog does not contain entry for WKT '%2$ls'"),
                GetName(), 
                (FdoString*) mCoordSysWkt
			)
		)
	);
}

void FdoSmLpSpatialContext::AddMismatchedWktError()
{
	GetErrors()->Add( FdoSmErrorType_Other, 
        FdoSchemaException::Create(
            FdoSmError::NLSGetMessage(
				SM_NLSID(0x000008C8L, "Error creating spatial context %1$ls (SRID=%2$ld), the WKT provided does not match the catalog."),
                GetName(), 
                GetSrid()
			)
		)
	);
}
