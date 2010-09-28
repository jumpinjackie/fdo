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
#ifndef FDOPOSTGIS_GEOMETRYCAPABILITIES_H_INCLUDED
#define FDOPOSTGIS_GEOMETRYCAPABILITIES_H_INCLUDED


namespace fdo { namespace postgis {


/// Implementation of interface describing geometry capabilities
/// supported by PostGIS provider.
///
class GeometryCapabilities : public FdoIGeometryCapabilities
{
public:

    /// Default constructor.
    GeometryCapabilities();

    //
    // FdoIGeometryCapabilities interface
    //

    /// Return list of supported geometry types.
    FdoGeometryType* GetGeometryTypes(FdoInt32& size);
 	
    /// Return the list of supported component types.
    FdoGeometryComponentType* GetGeometryComponentTypes(FdoInt32& size);
 	
    /// Return supported dimensionalities which are based on the bit masks defined
    /// in the FdoDimensionality enumeration.
    FdoInt32 GetDimensionalities();

protected:

    /// Destructor.
    virtual ~GeometryCapabilities();

    //
    // FdoIDisposable interface
    //

    /// Dispose this object.
    void Dispose();

private:
	
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_GEOMETRYCAPABILITIES_H_INCLUDED
