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
#ifndef FDOPOSTGIS_FILTERCAPABILITIES_H_INCLUDED
#define FDOPOSTGIS_FILTERCAPABILITIES_H_INCLUDED


namespace fdo { namespace postgis {


/// Implementation of interface describing capabilities about
/// filter expression tree supported by PostGIS provider.
///
class FilterCapabilities : public FdoIFilterCapabilities
{
public:

    /// Default constructor.
    FilterCapabilities();

    //
    // FdoIFilterCapabilities interface
    //

    /// Return array of FdoConditionType objects the feature provider supports.
    FdoConditionType* GetConditionTypes(FdoInt32& size);

    /// Return array of FdoSpatialOperation objects the feature provider supports.
    FdoSpatialOperations* GetSpatialOperations(FdoInt32& size);

    /// Return array of FdoDistanceOperation objects the feature provider supports.
    FdoDistanceOperations* GetDistanceOperations(FdoInt32& size);

    /// Determine if the feature provider supports geodesic distance measurement.
    bool SupportsGeodesicDistance();

    /// Determine if spatial and distance operations can be applied between two
    /// geometric properties. 
    bool SupportsNonLiteralGeometricOperations();

protected:

    /// Destructor.
    virtual ~FilterCapabilities();

    //
    // FdoIDisposable interface
    //

    /// Dispose this object.
    void Dispose();

private:
	
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_FILTERCAPABILITIES_H_INCLUDED
