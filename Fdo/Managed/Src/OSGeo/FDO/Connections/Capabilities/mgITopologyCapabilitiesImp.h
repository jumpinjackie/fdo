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

#pragma once

#include "FDO\Connections\Capabilities\mgITopologyCapabilities.h"

class FdoITopologyCapabilities;

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

/// \ingroup (OSGeoFDOConnectionsCapabilities)
/// \brief
/// FDO supports properties that inform the client code about capabilities that
/// may vary between providers. 
/// <p><b>Note:</b> <span class="red_text">This Help topic is provided for informational use only. There is
/// no interface or support provided. OSGeo reserves the right to change
/// the software related to the content herein.</span>
private ref class ITopologyCapabilitiesImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, 
                                              public NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ITopologyCapabilities
{
public:
    /// True if the provider supports topology; if this is false, attempts to
    /// create topology-specific schema will fail.
	virtual System::Boolean SupportsTopology();

    /// True if the provider supports TopoGeometry properties that depend on
    /// each other in a hierarchy; if this is false, attempting to set a
    /// non-nullptr for TopoGeometryPropertyDefinition's "DependsOnTopoGeometry"
    /// property will fail.
	virtual System::Boolean SupportsTopologicalHierarchy();

    /// True if the provider automatically (as a result of geometry assignment)
    /// breaks curves and inserts nodes wherever curves cross or touch, or where
    /// a node touches a curve.
	virtual System::Boolean BreaksCurveCrossingsAutomatically();

    /// True if the provider supports the activation of a topology using an area
    /// (surface) restriction; this may involve the use of pessimistic locking.
	virtual System::Boolean ActivatesTopologyByArea();

    /// True if the provider constrains edits of TopoGeometry properties (by
    /// geometry value) to those that do not change topological relationships.
	virtual System::Boolean ConstrainsFeatureMovements();

internal:
	ITopologyCapabilitiesImp(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoITopologyCapabilities* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES


