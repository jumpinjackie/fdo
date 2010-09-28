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
#ifndef FDOPOSTGIS_TOPOLOGYCAPABILITIES_H_INCLUDED
#define FDOPOSTGIS_TOPOLOGYCAPABILITIES_H_INCLUDED


namespace fdo { namespace postgis {


/// Implementation of ...
///
class TopologyCapabilities : public FdoITopologyCapabilities
{
public:

    /// Default constructor.
    TopologyCapabilities();
    
    //
    // FdoITopologyCapabilities interface
    //

    /// Determine if topology features are supported by the PostGIS provider.
    /// If topology is not supported, any attempt to create topology-specific
    /// schema will fail.
    ///
    /// \return true if topology is supported; false otherwise.
    ///
    bool SupportsTopology();
 	
    /// Determine if hierarchical dependencies topology properties are supported.
    ///
    /// If the capability is not supported, any attempt of setting a non-NULL for
    /// FdoTopoGeometryPropertyDefinition's "DependsOnTopoGeometry" property will fail.
    ///
    /// \return true if the provider supports TopoGeometry properties that
    /// depend on each other in a hierarchy; false otherwise.
    bool SupportsTopologicalHierarchy();
 	
    /// Determine if 
    ///
    /// \return true if the provider automatically (as a result of geometry assignment)
    /// breaks curves and inserts nodes wherever curves cross or touch,
    /// or where a node touches a curve; false otherwise.
    ///
    bool BreaksCurveCrossingsAutomatically();
 	
    /// Determine if 
    /// This capability, if supported, may involve the use of pessimistic locking.
    ///
    /// \return true if the provider supports the activation of a topology
    /// using an area (surface) restriction; false otherwise.
    ///
    bool ActivatesTopologyByArea();

    /// Determine if 
    ///
    /// \return true if the provider constrains edits of TopoGeometry properties
    /// (by geometry value) to those that do not change topological relationships;
    /// false otherwise.
    ///
    bool ConstrainsFeatureMovements();

protected:

    /// Destructor.
    virtual ~TopologyCapabilities();

    //
    // FdoIDisposable interface
    //

    /// Dispose this object.
    void Dispose();

private:
	
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_TOPOLOGYCAPABILITIES_H_INCLUDED
