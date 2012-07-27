//
// Copyright (C) 2004-2006  Autodesk, Inc.
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//


#pragma  once

/// \cond DOXYGEN-IGNORE
BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
/// \endcond

/// \ingroup (OSGeoFDOSchema)
/// \brief
/// The FdoPolygonVertexOrderRule enumeration defines the vertex order rule in a 
/// polygon loop. FdoPolygonOrderVertexRule values are typically counterclockwise, 
/// clockwise and none. None value means the vertex order rule is unknown or undefined.
public enum class PolygonVertexOrderRule
{
    /// Represents counterclockwise vertex order.
    PolygonVertexOrderRule_CCW = FdoPolygonVertexOrderRule_CCW,

    /// Represents clockwise vertex order.
    PolygonVertexOrderRule_CW = FdoPolygonVertexOrderRule_CW,

    /// Represents vertex order rule is unknown or undefined.
    PolygonVertexOrderRule_None = FdoPolygonVertexOrderRule_None
};

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_FDO_SCHEMA
/// \endcond


