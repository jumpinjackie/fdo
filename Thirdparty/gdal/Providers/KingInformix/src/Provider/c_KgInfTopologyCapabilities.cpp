/*
* Copyright (C) 2007  Haris Kurtagic
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
*/

#include "stdafx.h"

c_KgInfTopologyCapabilities::c_KgInfTopologyCapabilities ()
{
}

c_KgInfTopologyCapabilities::~c_KgInfTopologyCapabilities ()
{
}

void c_KgInfTopologyCapabilities::Dispose()
{
    delete this;
}

// True if the provider supports topology; if this is false, attempts to
// create topology-specific schema will fail.
bool c_KgInfTopologyCapabilities::SupportsTopology ()
{
    return (false);
}

// True if the provider supports TopoGeometry properties that depend on
// each other in a hierarchy; if this is false, attempting to set a
// non-NULL for FdoTopoGeometryPropertyDefinition's "DependsOnTopoGeometry"
// property will fail.
bool c_KgInfTopologyCapabilities::SupportsTopologicalHierarchy ()
{
    return (false);
}

// True if the provider automatically (as a result of geometry assignment)
// breaks curves and inserts nodes wherever curves cross or touch, or where
// a node touches a curve.
bool c_KgInfTopologyCapabilities::BreaksCurveCrossingsAutomatically ()
{
    return (false);
}

// True if the provider supports the activation of a topology using an area
// (surface) restriction; this may involve the use of pessimistic locking.
bool c_KgInfTopologyCapabilities::ActivatesTopologyByArea ()
{
    return (false);
}

// True if the provider constrains edits of TopoGeometry properties (by
// geometry value) to those that do not change topological relationships.
bool c_KgInfTopologyCapabilities::ConstrainsFeatureMovements ()
{
    return (false);
}


