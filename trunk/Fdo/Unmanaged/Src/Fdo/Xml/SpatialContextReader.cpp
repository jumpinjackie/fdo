//
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include <FdoGeometry.h>
#include <FdoStd.h>
#include <Fdo/Xml/SpatialContextReader.h>
#include "SCReadHandler.h"

FdoXmlSpatialContextReader* FdoXmlSpatialContextReader::Create(
	FdoXmlReader*               reader,
   	FdoXmlSpatialContextFlags*  flags
)
{
    if ( !reader ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"reader",
                L"(NULL)",
                L"FdoXmlSpatialContextReader::Create"
            )
        );

    return new FdoXmlSpatialContextReader( reader, flags );
}

FdoXmlSpatialContextReader::FdoXmlSpatialContextReader(
		FdoXmlReader*               reader,
    	FdoXmlSpatialContextFlags*  flags
)
{
    mXmlReader = FDO_SAFE_ADDREF(reader);

    if ( flags ) 
        mXmlFlags = FDO_SAFE_ADDREF(flags);
    else
        mXmlFlags = FdoXmlSpatialContextFlags::Create();

    mExtent = NULL;

    // Initialize the reader handler
    mSCHandler = FdoXmlSCReadHandler::Create();
    mSCHandler->mFirst = false;
    mSCHandler->Setup( mXmlFlags );
}
    
FdoXmlSpatialContextReader::~FdoXmlSpatialContextReader()
{
    if ( mExtent != NULL )
        mExtent->Release(); 

    FDO_SAFE_RELEASE(mSCHandler);
}

FdoXmlReader* FdoXmlSpatialContextReader::GetXmlReader()
{
    FdoXmlReader* pReader = mXmlReader;
    FDO_SAFE_ADDREF( pReader );

    return pReader;
}

FdoString* FdoXmlSpatialContextReader::GetName()
{
    ThrowOnNotRead( L"FdoXmlSpatialContextReader::GetName()" );

    return mSCHandler->mSCName;
}

FdoString* FdoXmlSpatialContextReader::GetDescription()
{
    ThrowOnNotRead( L"FdoXmlSpatialContextReader::GetDescription()" );

    return mSCHandler->mDescription;
}

FdoString* FdoXmlSpatialContextReader::GetCoordinateSystem()
{
    ThrowOnNotRead( L"FdoXmlSpatialContextReader::GetCoordinateSystem()" );

    return mSCHandler->mCoordSysName;
}

FdoString* FdoXmlSpatialContextReader::GetCoordinateSystemWkt()
{
    return mSCHandler->mCoordSysWkt;
}

FdoSpatialContextExtentType FdoXmlSpatialContextReader::GetExtentType()
{
    ThrowOnNotRead( L"FdoXmlSpatialContextReader::GetExtentType()" );

    return mSCHandler->mExtentType;
}

FdoByteArray* FdoXmlSpatialContextReader::GetExtent()
{
    ThrowOnNotRead( L"FdoXmlSpatialContextReader::GetExtent()" );

    if ( mExtent ) 
        mExtent->AddRef();
    return mExtent;
}

const double FdoXmlSpatialContextReader::GetXYTolerance()
{
    ThrowOnNotRead( L"" );

    return mSCHandler->mXYTolerance;
}

const double FdoXmlSpatialContextReader::GetZTolerance()
{
    ThrowOnNotRead( L"FdoXmlSpatialContextReader::GetZTolerance()" );

    return  mSCHandler->mZTolerance;
}

const bool FdoXmlSpatialContextReader::IsActive()
{
    ThrowOnNotRead( L"FdoXmlSpatialContextReader::IsActive()" );

    return  mIsActiveSC;
}

bool FdoXmlSpatialContextReader::ReadNext()
{
    FdoXmlSaxContextP context = FdoXmlSaxContext::Create( mXmlReader );

    // Clear out extents for previously read spatial context
    if ( mExtent ) {
        mExtent->Release();
        mExtent = NULL;
    }

    // Re-initialze the read handler so the next spatial context can be read.
    mSCHandler->Setup( mXmlFlags );

    // Read the next spatial context from XML.
    mXmlReader->Parse( this, context, true );

    context->ThrowErrors();

    if ( mSCHandler->mSCName.GetLength() > 0 ) {
        // A spatial context was found. Convert extents to byte array.
 	    FdoPtr<FdoFgfGeometryFactory>  gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIEnvelope>            env = gf->CreateEnvelopeXY(
                                                mSCHandler->mMinX, 
                                                mSCHandler->mMinY, 
                                                mSCHandler->mMaxX, 
                                                mSCHandler->mMaxY 
                                              ); 

        FdoPtr<FdoIGeometry>            geom = gf->CreateGeometry(env); 

        mExtent =  gf->GetFgf(geom);
        mSCHandler->mFirst = false;

        // Decode the coordinate system name.
        if ( mXmlFlags->GetNameAdjust() ) {
            mSCHandler->mCoordSysName = mXmlReader->DecodeName( mSCHandler->mCoordSysName );
        }
        return true;
    }
    else {
        // No spatial context found. We're done reading.
        return false;
    }
}

FdoXmlSaxHandler* FdoXmlSpatialContextReader::XmlStartDocument(FdoXmlSaxContext*)
{
    mSCHandler->mFirst = true;

    return NULL;
}

FdoXmlSaxHandler* FdoXmlSpatialContextReader::XmlStartElement( 
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts
)
{
    return mSCHandler->doTransition( true, context, uri, name, atts );
}

FdoBoolean FdoXmlSpatialContextReader::XmlEndElement( 
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname
)
{
    mSCHandler->doTransition( false, context, uri, name, FdoXmlAttributesP(FdoXmlAttributeCollection::Create()) );
    return(mSCHandler->mStopParse);
}


void FdoXmlSpatialContextReader::Dispose()
{
    delete this;
}

void FdoXmlSpatialContextReader::ThrowOnNotRead( FdoString* function )
{
    if ( mSCHandler->mSCName.GetLength() == 0 )
        // No currently read spatial context, so throw an error.
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(SC_4_XMLNOREAD),
                function
            )
        );
}


