//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "stdafx.h"
#include "PostGisProvider.h"
#include "PgGeometry.h"
// std
#include <cassert>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
// boost
#include <boost/cstdint.hpp>

namespace fdo { namespace postgis { namespace ewkb {

void hex_to_bytes(std::string const& hexstr, std::vector<boost::uint8_t>& bytes)
{
    typedef std::string::size_type size_type;
    size_type const bytesSize = hexstr.size() / 2;
    size_type const charsPerByte = 2;

    // Shrink vector and reserve enough memory to avoid further reallocations.
    std::vector<boost::uint8_t>().swap(bytes);
    bytes.reserve(bytesSize);

    for (size_type i = 0; i < bytesSize; ++i)
    {                   
        std::istringstream iss(hexstr.substr(i * charsPerByte, charsPerByte));
        boost::uint32_t n;
        iss >> std::hex >> n;
        bytes.push_back(static_cast<boost::uint8_t>(n));
    }

    assert(bytes.size() == bytesSize);
}

void bytes_to_hex(std::vector<boost::uint8_t> const& bytes, std::string& hexstr)
{
    typedef std::vector<boost::uint8_t>::size_type size_type;
    size_type const bytesSize = bytes.size();

    const char hexChars[] = "0123456789ABCDEF";
    char hexByte[3] = { 0 };

    std::ostringstream oss;
    for (size_type i = 0; i < bytesSize; ++i)
    {
        boost::uint8_t byte = static_cast<boost::uint8_t>(bytes[i]);
        hexByte[0] = hexChars[(byte >> 4) & 0xf]; 
        hexByte[1] = hexChars[byte & 0xf];
        hexByte[2] = '\0';

        oss << std::setw(2) << hexByte;
    }
    hexstr = oss.str();

    assert(hexstr.size() == bytesSize * 2);
}

FdoGeometryType FdoGeometryTypeFromPgType(std::string const& pgType)
{
    FdoGeometryType fdoType = FdoGeometryType_None;

    try
    {
        if (0 == pgType.compare(0, 5, "POINT"))
        {
            fdoType = FdoGeometryType_Point;
        }
        else if (0 == pgType.compare(0, 10, "LINESTRING"))
        {
            fdoType = FdoGeometryType_LineString;
        }
        else if (0 == pgType.compare(0, 7, "POLYGON"))
        {
            fdoType = FdoGeometryType_Polygon;
        }
        else if (0 == pgType.compare(0, 10, "MULTIPOINT"))
        {
            fdoType = FdoGeometryType_MultiPoint;
        }
        else if (0 == pgType.compare(0, 15, "MULTILINESTRING"))
        {
            fdoType = FdoGeometryType_MultiLineString;
        }
        else if (0 == pgType.compare(0, 12, "MULTIPOLYGON"))
        {
            fdoType = FdoGeometryType_MultiPolygon;
        }
        else if (0 == pgType.compare(0, 18, "GEOMETRYCOLLECTION"))
        {
            fdoType = FdoGeometryType_MultiGeometry;
        }
        
        // TODO: Add conversion of CURVE types
        
        //else if (0 == pgType.compare(0, 0, ""))
        //{
        //    fdoType = FdoGeometryType_CurveString;
        //}
        //else if (0 == pgType.compare(0, 0, ""))
        //{
        //    fdoType = FdoGeometryType_CurvePolygon
        //}
        //else if (0 == pgType.compare(0, 0, ""))
        //{
        //    fdoType = FdoGeometryType_MultiCurveString;
        //}
        //else if (0 == pgType.compare(0, 0, ""))
        //{
        //    fdoType = FdoGeometryType_MultiCurvePolygon;
        //}
    }
    catch (std::out_of_range& e)
    {
        // TODO: Consider replacing it with an exception
        FDOLOG_WRITE("FdoGeometryTypeFromPgType failure: %s", e.what());
        assert(!"INVALID INPUT GEOMETRY NAME");
    }

    assert(FdoGeometryType_None != fdoType);
    return fdoType;
}

FdoInt32 FdoDimensionTypeFromPgType(FdoInt32 const& pgDim, std::string const& pgType)
{
    FdoInt32 fdoDim = 0;

    switch (pgDim)
    {
    case 2:
        {
            fdoDim = FdoDimensionality_XY;
        }
        break;
    case 3:
        {
            if (pgType.substr(pgType.size() - 1, 1) == "M")
                fdoDim = (FdoDimensionality_XY | FdoDimensionality_M);
            else
                fdoDim = (FdoDimensionality_XY | FdoDimensionality_Z);
        }
        break;
    case 4:
        {
            fdoDim = (FdoDimensionality_XY | FdoDimensionality_Z | FdoDimensionality_M);
        }
        break;
    default:
        assert(!"SHOULD NEVER GET HERE");
    }

    return fdoDim;
}

std::string PgGeometryTypeFromFdoType(FdoInt32 const& fdoType)
{
    std::string pgType;

    switch (fdoType)
    {
    case FdoGeometryType_Point:
        pgType = "POINT";
        break;
    case FdoGeometryType_LineString:
        pgType = "LINESTRING";
        break;
    case FdoGeometryType_Polygon:
        pgType = "POLYGON";
        break;
    case FdoGeometryType_MultiPoint:
        pgType = "MULTIPOINT";
        break;
    case FdoGeometryType_MultiLineString:
        pgType = "MULTILINESTRING";
        break;
    case FdoGeometryType_MultiPolygon:
        pgType = "MULTIPOLYGON";
        break;
    case FdoGeometryType_MultiGeometry:
        pgType = "GEOMETRYCOLLECTION";
        break;
    default:
        // Also handles FdoGeometryType_None
        pgType = "GEOMETRY";
    }

    return pgType;
}

boost::uint32_t GetOrdinatesFromDimension(boost::uint32_t const& dim)
{
    boost::uint32_t value = 0;

    if (dim == FdoDimensionality_XY)
        value = 2;
    else if (dim == (FdoDimensionality_XY | FdoDimensionality_Z))
        value = 3;
    else if (dim == (FdoDimensionality_XY | FdoDimensionality_M))
        value = 3;
    else if (dim == (FdoDimensionality_XY | FdoDimensionality_Z | FdoDimensionality_M))
        value = 4;
    else
        assert(!"SHOULD NEVER GET HERE");

    return value;
}

FdoIGeometry* CreateGeometryFromExtendedWkb(ewkb_t const& ewkb)
{
    using boost::uint8_t;
    using boost::uint32_t;

    assert(!ewkb.empty());
    assert(ewkb.size() >= (sizeof(uint8_t) + sizeof(uint32_t) + sizeof(uint32_t)));

    ewkb_t::size_type currentByte = 0;
    ewkb_t::size_type const ewkbSize = ewkb.size();

    //
    // Endian flag
    //
    //uint8_t const endianByte = read_data<uint8_t>::from(&ewkb[currentByte]);
    uint8_t const& endianByte = ewkb[currentByte];
    if (eWkbNDR != endianByte)
    {
        // TODO: Add support for Big-Endian
        throw FdoException::Create(L"Only Little-Endian byte order supported.");
    }
    currentByte++;

    //
    // Geometry type
    //
    assert(1 == currentByte);
    uint32_t const ewkbFlags = read_data<uint32_t>::from(&ewkb[currentByte]);
    uint32_t geomType = ewkbFlags & 0xff;    

    currentByte += sizeof(ewkbFlags);

    //
    // EWKB flags
    //

    // Dimensionality
    uint32_t geomDim = FdoDimensionality_XY;
    if (ewkbFlags & eWkbFlagZ)
        geomDim |= FdoDimensionality_Z;
    if (ewkbFlags & eWkbFlagM)
        geomDim |= FdoDimensionality_M;

    // SRID - PostGIS specific flag
    bool isSrid = (0 != (ewkbFlags & eWkbFlagSrid));

    // BBOX - PostGIS specific flag
    bool isBbox = (0 != (ewkbFlags & eWkbFlagBBox));

    //
    // Generate FGF stream from EWKB
    //
    assert(5 == currentByte);

    boost::uint32_t bytesReadSize = 0;
    FdoPtr<FdoByteArray> fgfBytes = NULL;
    fgfBytes = CreateFgfFromExtendedWkb(ewkb, currentByte,
                                        geomType, geomDim,
                                        isSrid, isBbox, bytesReadSize);
    assert(NULL != fgfBytes);
    assert(ewkbSize == bytesReadSize + currentByte);

    //
    // Create FDO geometry from FGF binary data
    //
    FdoPtr<FdoFgfGeometryFactory> factory(FdoFgfGeometryFactory::GetInstance());
    assert(NULL != factory);

    FdoPtr<FdoIGeometry> geometry = factory->CreateGeometryFromFgf(fgfBytes);
    assert(NULL != geometry);

    FDO_SAFE_ADDREF(geometry.p);
    return geometry.p;
}

FdoByteArray* CreateFgfFromExtendedWkb(ewkb_t const& ewkb,
                                       ewkb_t::size_type const& beginByte,
                                       boost::uint32_t const& geomType,
                                       boost::uint32_t const& geomDim,
                                       bool isSrid,
                                       bool isBbox,
                                       boost::uint32_t& bytesReadSize)
{
    // NOTE: Assume that 'beginByte' stores index of byte in 'ewkb' stream.
    //       immediately after the geometry type.
    assert(!ewkb.empty());
    assert(beginByte >= 5);
    assert(!isBbox); // Currently not supported, also by PostGIS!

    // Base types used in bytes reading.
    using boost::uint8_t;
    using boost::uint32_t;

    // Helper pointers definition used in necessary :-( casts below.
    typedef FdoByte* fdo_byte_ptr_t;
    typedef boost::uint8_t* byte_ptr_t;
    byte_ptr_t bytePtr = NULL;
    fdo_byte_ptr_t fdoBytePtr = NULL;

    // Index used to track current byte in EWKB stream
    ewkb_t::size_type currentByte = beginByte;

    // Tracks number of bytes not processed yet.
    uint32_t leftBytes = static_cast<uint32_t>(ewkb.size()) - beginByte;

    // Construct array for whole simple geometry or for sub-geometry of multi-geometries.
    // For multi-geometries, the array is appended in outermost call to final FGF data.
    uint32_t const fgfSize = sizeof(geomType) + sizeof(geomDim) + leftBytes;
    FdoByteArray* fgfBytes = FdoByteArray::Create(fgfSize);
    fgfBytes = FdoByteArray::Append(fgfBytes, sizeof(geomType), (FdoByte*)&geomType);

    // Processing:
    // - for simple geometries types, just copy the array
    // - for multi-geometries, process sub-geometries recursively
    //
    switch(geomType)
    {
    case FdoGeometryType_Point:
        {
            fgfBytes = FdoByteArray::Append(fgfBytes, sizeof(geomDim), (FdoByte*)&geomDim);

            if (isSrid)
            {
                currentByte += sizeof(uint32_t);
                leftBytes -= sizeof(uint32_t);
            }

            // Valid combinations: X,Y or X,Y,Z or X,Y,M or X,Y,Z,M
            uint32_t const ordinatesSize =
                sizeof(double) * GetOrdinatesFromDimension(geomDim);

            // NOTE: These dirty hacks below are required because
            // FdoByteArray do NOT respect const-correctness for input parameters.
            bytePtr = const_cast<byte_ptr_t>(&ewkb[currentByte]);
            fdoBytePtr = static_cast<fdo_byte_ptr_t>(bytePtr);

            fgfBytes = FdoByteArray::Append(fgfBytes, ordinatesSize, fdoBytePtr);
            currentByte += ordinatesSize;
            leftBytes -= ordinatesSize;

            //TODO: Add post-condition
        }
        break;
    case FdoGeometryType_LineString:
        {
            fgfBytes = FdoByteArray::Append(fgfBytes, sizeof(geomDim), (FdoByte*)&geomDim);

            if (isSrid)
            {
                currentByte += sizeof(uint32_t);
                leftBytes -= sizeof(uint32_t);
            }

            uint32_t const numCoords = read_data<uint32_t>::from(&ewkb[currentByte]);
            fgfBytes = FdoByteArray::Append(fgfBytes, sizeof(numCoords), (FdoByte*)&numCoords);
            currentByte += sizeof(numCoords);
            leftBytes -= sizeof(numCoords);

            uint32_t const ordinatesSize =
                numCoords * sizeof(double) * GetOrdinatesFromDimension(geomDim);
            bytePtr = const_cast<byte_ptr_t>(&ewkb[currentByte]);
            fdoBytePtr = static_cast<fdo_byte_ptr_t>(bytePtr);
            fgfBytes = FdoByteArray::Append(fgfBytes, ordinatesSize, fdoBytePtr);
            currentByte += ordinatesSize;
            leftBytes -= ordinatesSize;

            //TODO: Add post-condition
        }
        break;
    case FdoGeometryType_Polygon:
        {
            fgfBytes = FdoByteArray::Append(fgfBytes, sizeof(geomDim), (FdoByte*)&geomDim);

            if (isSrid)
            {
                currentByte += sizeof(uint32_t);
                leftBytes -= sizeof(uint32_t);
            }

            uint32_t const numRings = read_data<uint32_t>::from(&ewkb[currentByte]);
            fgfBytes = FdoByteArray::Append(fgfBytes, sizeof(numRings), (FdoByte*)&numRings);
            currentByte += sizeof(numRings);
            leftBytes -= sizeof(numRings);

            for (uint32_t i = 0;  i < numRings && leftBytes > 0;  i++)
            {
                uint32_t const numCoords = read_data<uint32_t>::from(&ewkb[currentByte]);
                fgfBytes = FdoByteArray::Append(fgfBytes, sizeof(numCoords), (FdoByte*)&numCoords);
                currentByte += sizeof(numCoords);
                leftBytes -= sizeof(numCoords);

                uint32_t const numOrdinates =
                    numCoords * sizeof(double) * GetOrdinatesFromDimension(geomDim);
                bytePtr = const_cast<byte_ptr_t>(&ewkb[currentByte]);
                fdoBytePtr = static_cast<fdo_byte_ptr_t>(bytePtr);
                fgfBytes = FdoByteArray::Append(fgfBytes, numOrdinates, fdoBytePtr);
                currentByte += numOrdinates;
                leftBytes -= numOrdinates;
            }

            //TODO: Add post-condition
        }
        break;
    case FdoGeometryType_MultiPoint:
    case FdoGeometryType_MultiGeometry:
    case FdoGeometryType_MultiLineString:
    case FdoGeometryType_MultiPolygon:
        {
            if (isSrid)
            {
                currentByte += sizeof(uint32_t);
                leftBytes -= sizeof(uint32_t);
            }

            uint32_t const numSubGeoms = read_data<uint32_t>::from(&ewkb[currentByte]);
            fgfBytes = FdoByteArray::Append(fgfBytes, sizeof(numSubGeoms), (FdoByte*)&numSubGeoms);
            currentByte += sizeof(numSubGeoms);
            leftBytes -= sizeof(numSubGeoms);

            for (uint32_t i = 0;  i < numSubGeoms && leftBytes > 0;  i++)
            {
                uint8_t const& endianByte = ewkb[currentByte];
                assert(eWkbNDR == endianByte);
                currentByte++;
                leftBytes--;

                uint32_t const ewkbFlags = read_data<uint32_t>::from(&ewkb[currentByte]);
                currentByte += sizeof(ewkbFlags);
                leftBytes -= sizeof(ewkbFlags);

                uint32_t subGeomType = ewkbFlags & 0xff;

                uint32_t subGeomDim = FdoDimensionality_XY;
                if (ewkbFlags & eWkbFlagZ)
                    subGeomDim |= FdoDimensionality_Z;
                if (ewkbFlags & eWkbFlagM)
                    subGeomDim |= FdoDimensionality_M;
                bool isSrid = (0 != (ewkbFlags & eWkbFlagSrid));
                bool isBbox = (0 != (ewkbFlags & eWkbFlagBBox));

                // Recurse on sub-geometry.
                uint32_t bytesReadSubGeom = 0;
                FdoPtr<FdoByteArray> fgfSubGeomBytes =
                    CreateFgfFromExtendedWkb(ewkb, currentByte, subGeomType,
                        subGeomDim, isSrid, isBbox, bytesReadSubGeom);

                currentByte += bytesReadSubGeom;
                leftBytes -= bytesReadSubGeom;
                fgfBytes = FdoByteArray::Append(fgfBytes,
                    fgfSubGeomBytes->GetCount(), fgfSubGeomBytes->GetData());
            }

            assert(ewkb.size() - currentByte == leftBytes);
        }
        break;
    default:
        throw FdoException::Create(L"Unknown geometry type.");
    }

    bytesReadSize = static_cast<uint32_t>(currentByte - beginByte);

    assert(NULL != fgfBytes);
    return fgfBytes;
}

}}} // namespace fdo::postgis::ewkb
