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
#include <Geometry.h>
#include <Std.h>
#include <Fdo/Xml/SpatialContextWriter.h>

FdoXmlSpatialContextWriter* FdoXmlSpatialContextWriter::Create(
	FdoXmlWriter*               writer,
   	FdoXmlSpatialContextFlags*  flags 
)
{
    if ( !writer ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"writer",
                L"(NULL)",
                L"FdoXmlSpatialContextWriter::Create"
            )
        );

    return new FdoXmlSpatialContextWriter( writer, flags );
}


FdoXmlSpatialContextWriter::FdoXmlSpatialContextWriter(
	FdoXmlWriter*               writer,
   	FdoXmlSpatialContextFlags*  flags
)
{
    mXmlWriter = FDO_SAFE_ADDREF(writer);

    if ( flags ) 
        mXmlFlags = FDO_SAFE_ADDREF(flags);
    else
        mXmlFlags = FdoXmlSpatialContextFlags::Create();

    mExtentType = FdoSpatialContextExtentType_Static;
    mExtent = NULL;

    mXYTolerance = 0;
    mZTolerance = 0;

    FDO_SAFE_RELEASE( mExtent );
}

FdoString* FdoXmlSpatialContextWriter::GetName()
{
    return mSCName;
}

void FdoXmlSpatialContextWriter::SetName(FdoString* value)
{
    mSCName = value;
}

FdoString* FdoXmlSpatialContextWriter::GetDescription()
{
    return mDescription;
}

void FdoXmlSpatialContextWriter::SetDescription(FdoString* value)
{
    mDescription = value;
}

FdoString* FdoXmlSpatialContextWriter::GetCoordinateSystem()
{
    return mCoordSysName;
}

void FdoXmlSpatialContextWriter::SetCoordinateSystem(FdoString* value)
{
    mCoordSysName = value;
}

FdoString* FdoXmlSpatialContextWriter::GetCoordinateSystemWkt()
{
    return mCoordSysWkt;
}

void FdoXmlSpatialContextWriter::SetCoordinateSystemWkt(FdoString* value)
{
    mCoordSysWkt = value;
}

FdoSpatialContextExtentType FdoXmlSpatialContextWriter::GetExtentType()
{
    return mExtentType;
}

void FdoXmlSpatialContextWriter::SetExtentType(FdoSpatialContextExtentType value)
{
    mExtentType = value;
}

FdoByteArray* FdoXmlSpatialContextWriter::GetExtent()
{
    return FDO_SAFE_ADDREF( mExtent );
}

void FdoXmlSpatialContextWriter::SetExtent(FdoByteArray* value)
{
    FDO_SAFE_RELEASE( mExtent );
    mExtent = FDO_SAFE_ADDREF( value );
}

const double FdoXmlSpatialContextWriter::GetXYTolerance()
{
    return mXYTolerance;
}

void FdoXmlSpatialContextWriter::SetXYTolerance(double value)
{
    mXYTolerance = value;
}

const double FdoXmlSpatialContextWriter::GetZTolerance()
{
    return mZTolerance;
}

void FdoXmlSpatialContextWriter::SetZTolerance(const double value)
{
    mZTolerance = value;
}

void FdoXmlSpatialContextWriter::WriteSpatialContext()
{
    // name is mandatory
    if ( mSCName.GetLength() < 1 )
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(SC_1_XMLNONAME)
            )
        );

    // Extents are mandatory
    if ( !mExtent ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(SC_3_XMLNOEXTENTS),
                (FdoString*) mSCName
            )
        );

    // Convert extents from byte array to 4 doubles.
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIPolygon>   geom = (FdoIPolygon *)gf->CreateGeometryFromFgf( mExtent );

    FdoPtr<FdoILinearRing> extPoly = geom->GetExteriorRing();
    FdoPtr<FdoIEnvelope> env = extPoly->GetEnvelope();

    double minX = env->GetMinX();
    double minY = env->GetMinY();
    double maxX = env->GetMaxX();
    double maxY = env->GetMaxY();
    
    // Spatial Context is represented by a DerivedCRS element
    mXmlWriter->WriteStartElement( L"gml:DerivedCRS" );
    // gml:id is mandatory, just set it to spatial context name.
    mXmlWriter->WriteAttribute( 
        L"gml:id", 
        mXmlFlags->GetNameAdjust() ? 
            ((FdoString*) mXmlWriter->EncodeName(mSCName)) : 
            (FdoString*) mSCName 
    );

    // Write out attributes that have no GML equivalent. These are kept in
    // the MetaData sub-property
    if ( (mExtentType == FdoSpatialContextExtentType_Dynamic) ||
         (mXYTolerance > 0) || 
         (mZTolerance > 0)
    ) {
        mXmlWriter->WriteStartElement( L"gml:metaDataProperty" );
        mXmlWriter->WriteStartElement( L"gml:GenericMetaData" );
    
        if ( mExtentType == FdoSpatialContextExtentType_Dynamic ) {
            mXmlWriter->WriteStartElement( L"fdo:SCExtentType" );
            mXmlWriter->WriteCharacters( L"dynamic" );
            mXmlWriter->WriteEndElement();
        }

        if ( mXYTolerance > 0 ) {
            mXmlWriter->WriteStartElement( L"fdo:XYTolerance" );
            mXmlWriter->WriteCharacters( (FdoString*) FdoStringP::Format(L"%lf", mXYTolerance) );
            mXmlWriter->WriteEndElement();
        }
        
        if ( mZTolerance > 0 ) {
            mXmlWriter->WriteStartElement( L"fdo:ZTolerance" );
            mXmlWriter->WriteCharacters( (FdoString*) FdoStringP::Format(L"%lf", mZTolerance) );
            mXmlWriter->WriteEndElement();
        }

        mXmlWriter->WriteEndElement();
        mXmlWriter->WriteEndElement();
    }

    // Write description as GML remarks
    if ( mDescription.GetLength() > 0 ) { 
        mXmlWriter->WriteStartElement( L"gml:remarks" );
        mXmlWriter->WriteCharacters( (FdoString*) mDescription );
        mXmlWriter->WriteEndElement();
    }

    // Write name as srsName
    mXmlWriter->WriteStartElement( L"gml:srsName" );
    mXmlWriter->WriteCharacters( (FdoString*) mSCName );
    mXmlWriter->WriteEndElement();

    // Encode extents in a GML bounding box.
    mXmlWriter->WriteStartElement( L"gml:validArea" );
    mXmlWriter->WriteStartElement( L"gml:boundingBox" );
    mXmlWriter->WriteStartElement( L"gml:pos" );
    mXmlWriter->WriteCharacters( (FdoString*) FdoStringP::Format(L"%lf %lf", minX, minY) );
    mXmlWriter->WriteEndElement();
    mXmlWriter->WriteStartElement( L"gml:pos" );
    mXmlWriter->WriteCharacters( (FdoString*) FdoStringP::Format(L"%lf %lf", maxX, maxY) );
    mXmlWriter->WriteEndElement();
    mXmlWriter->WriteEndElement();
    mXmlWriter->WriteEndElement();

    // Encode coordinate system name in a BaseCRS
    mXmlWriter->WriteStartElement( L"gml:baseCRS" );
    if ( mCoordSysName.GetLength() == 0 ) {
        // Use special URI for default FDO coordinate system.
        mXmlWriter->WriteAttribute( 
            L"xlink:href", 
            L"http://fdo.osgeo.org/crs#default_cartesian"
        );
    }
    else {
        if ( mCoordSysWkt.GetLength() == 0 ) {
            // Otherwise, when there is no WKT definition,
            // encode coordinate system in a URI reference. The URI has the
            // customer's URL as its root.
            mXmlWriter->WriteAttribute( 
                L"xlink:href", 
                (FdoString*) FdoStringP::Format( 
                    L"http://%ls/crs/#%ls",
                    mXmlFlags->GetUrl(),
                    (FdoString*) 
                        (mXmlFlags->GetNameAdjust() ?
                            mXmlWriter->EncodeName(mCoordSysName) : mCoordSysName) 
                )
            );
        }
        else {
            // There is a WKT definition so embed the coordinate system
            // in the XML document.
            mXmlWriter->WriteStartElement( L"fdo:WKTCRS" );
            mXmlWriter->WriteAttribute( 
                L"gml:id", 
                (FdoString*) (mXmlFlags->GetNameAdjust() ?
                    mXmlWriter->EncodeName(mCoordSysName) : 
                    mCoordSysName
                )
            );

            mXmlWriter->WriteStartElement( L"gml:srsName" );
            mXmlWriter->WriteCharacters( mCoordSysName );
            mXmlWriter->WriteEndElement();

            mXmlWriter->WriteStartElement( L"fdo:WKT" );
            mXmlWriter->WriteCharacters( mCoordSysWkt );
            mXmlWriter->WriteEndElement();

            mXmlWriter->WriteEndElement();
        }
    }
    mXmlWriter->WriteEndElement();

    // The following elements do not contain info used by FDO, but are required
    // by GML. Write them with specific well-know values.

    mXmlWriter->WriteStartElement( L"gml:definedByConversion" );
    mXmlWriter->WriteAttribute( 
        L"xlink:href",
        L"http://fdo.osgeo.org/coord_conversions#identity"
    );
    mXmlWriter->WriteEndElement();

    mXmlWriter->WriteStartElement( L"gml:derivedCRSType" );
    mXmlWriter->WriteAttribute( L"codeSpace", L"http://fdo.osgeo.org/crs_types" );
    mXmlWriter->WriteCharacters( L"geographic" );
    mXmlWriter->WriteEndElement();

    mXmlWriter->WriteStartElement( L"gml:usesCS" );
    mXmlWriter->WriteAttribute( 
        L"xlink:href", 
        L"http://fdo.osgeo.org/cs#default_cartesian"
    );
    mXmlWriter->WriteEndElement();

    // close off the DerivedCRS element.
    mXmlWriter->WriteEndElement();
}

void FdoXmlSpatialContextWriter::Dispose()
{
    delete this;
}




