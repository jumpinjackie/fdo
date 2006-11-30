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
    if( ext ) // make a copy of the byte array as the one obtained from the factory is not thread safe.
        mExtent = FdoByteArray::Create( ext->GetData(), ext->GetCount() );

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
	}

}
