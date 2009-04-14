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
#ifndef FDOPOSTGIS_PGGEOMETRY_H_INCLUDED
#define FDOPOSTGIS_PGGEOMETRY_H_INCLUDED

#include <cstring>
#include <vector>
#include <boost/cstdint.hpp>
#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>

namespace fdo { namespace postgis { namespace ewkb {

/// Type used as array of bytes storing EWKB data.
typedef std::vector<boost::uint8_t> ewkb_t;

/// Enumeration of possible byte orders.
enum ByteOrder
{
    eWkbXDR = 0, // Big Endian
    eWkbNDR = 1  // Little Endian
};

/// Enumeration of various PostGIS-specific flags stored in EWKB data.
enum GeometryTypeFlags
{
    eWkbFlagZ    = 0x80000000,
    eWkbFlagM    = 0x40000000,
    eWkbFlagSrid = 0x20000000,
    eWkbFlagBBox = 0x10000000
};

/// Template class of converter reading bytes as given Target type.
/// It class is used to read subsequent data elements from EWKB stream.
/// This template class provides implementation of reading bytes for integral types.
///
template<class Target>
struct read_data
{
    static Target from(boost::uint8_t const* bytes)
    {
        BOOST_STATIC_ASSERT(boost::is_integral<Target>::value);

        Target data = Target();
        for (size_t bn = sizeof(Target); bn > 0; --bn)
        {
            size_t shift = (bn - 1) << 3;
            data |= (Target)(bytes[bn - 1] & 0xff) << shift;

        }
        return data;
    }
};

/// Specialization of read_data class for double type.
///
template<>
struct read_data<double>
{
    static double from(boost::uint8_t const* bytes)
    {
        BOOST_STATIC_ASSERT(sizeof(boost::int64_t) == sizeof(double));

        boost::int64_t tmp = read_data<boost::int64_t>::from(bytes);
        double data = double();
        std::memcpy(&data, &tmp, sizeof(double));
        return data;
    }
};

/// Decode binary encoded in hex-string to raw binary array.
/// \param
/// hexstr [in] - string of hex form of binary data.
/// \param
/// bytes [out] - array storing raw binary data decoded from passed hex string.
///
void hex_to_bytes(std::string const& hexstr, std::vector<boost::uint8_t>& bytes);


/// Encode raw binary array to hex-string.
/// \param
/// bytes [in] - array storing raw binary data.
/// \param
/// hexstr [out] - string of hex form of binary data encoded from given binary data.
///
void bytes_to_hex(std::vector<boost::uint8_t> const& bytes, std::string& hexstr);

// Translate name of PostGIS geometry type to FDO geometry type enumerator.
FdoGeometryType FdoGeometryTypeFromPgType(std::string const& pgType);

/// Generate FDO coordinates dimension mask from PostGIS dimension value
/// and geometry type.
FdoInt32 FdoDimensionTypeFromPgType(FdoInt32 const& pgDim, std::string const& pgType);

// Translate FDO geometry type enumeration to name of PostGIS geometry type.
std::string PgGeometryTypeFromFdoType(FdoInt32 const& fdoType);

// Translate FDO geometry type enumeration to name of PostGIS geometry type.
// This version accepts flag indicating if XYM dimension is requested.
std::string PgGeometryTypeFromFdoType(FdoInt32 const& fdoType, bool isXYM);

// Translate FDO geometry type enumeration to name of PostGIS geometry type.
// This version accepts flag indicating if XYM dimension is requested.
// In order to restrict the spatial column to a single type, use one of:
// POINT, LINESTRING, POLYGON, MULTIPOINT, MULTILINESTRING, MULTIPOLYGON, GEOMETRYCOLLECTION 
// or corresponding XYM versions
// POINTM, LINESTRINGM, POLYGONM, MULTIPOINTM, MULTILINESTRINGM, MULTIPOLYGONM, GEOMETRYCOLLECTIONM.
// For heterogeneous (mixed-type) collections, you can use "GEOMETRY" as the type.
std::string PgGeometryTypeFromFdoType(FdoGeometryType* fdoTypes, FdoInt32 length, bool isXYM);

/// Calculate number of ordinates based on given dimensionality.
///
/// \param
/// dim [in] - numeric value representing dimensionality, based on FdoDimensionality enum.
/// \return
/// Number of ordinates stored in geometry of given dimensionality.
///
boost::uint32_t GetOrdinatesFromDimension(boost::uint32_t const& dim);

/// Create FDO geometry from PostGIS EWKB stream of bytes.
///
/// \param
/// ewkb [in] - array of bytes with EWKB data.
/// \return
/// Pointer to new instance of FDO geometry object created from EWKB data.
///
FdoIGeometry* CreateGeometryFromExtendedWkb(ewkb_t const& ewkb);

/// Create FDO FGF binary stream from PostGIS EWKB stream of bytes.
/// This is base of simple state-machine converting EWKB to FGF.
/// For multi-geometries, the function is called recursively, for every subsequent
/// simple geometry, and every call iterates through bytes of ewkb array.
///
/// \param
/// ewkb [in] - array of bytes with EWKB data.
/// \param
/// beginByte [in] - index of byte from which subsequent call starts processing.
/// \param
/// geomType [in] - numeric value representing geometry type, defined in OGC SFS.
/// \param
/// geomDim [in] - numeric value representing geometry dimensionality.
/// \param
/// isSrid [in] - flag indicating if SRID bytes are included in EWKB. PostGIS specific.
/// \param
/// isBbox [in] - flag indicating if BBOX bytes are included. PostGIS specific.
/// \param
/// bytesReadSize [out] - number of bytes read by subsequent call of the function.
/// \return
/// FDO byte array storing geometry encoded in FGF format.
///
FdoByteArray* CreateFgfFromExtendedWkb(ewkb_t const& ewkb,
                                       ewkb_t::size_type const& beginByte,
                                       boost::uint32_t const& geomType,
                                       boost::uint32_t const& geomDim,
                                       bool isSrid,
                                       bool isBbox,
                                       boost::uint32_t& bytesReadSize);

}}} // namespace fdo::postgis::ewkb

#endif // FDOPOSTGIS_PGGEOMETRY_H_INCLUDED
