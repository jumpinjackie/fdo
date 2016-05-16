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

#pragma  once

/// \cond DOXYGEN-IGNORE
BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT
/// \endcond

/// \ingroup (OSGeoFDOCommandsSpatialContext)
/// \brief
/// The SpatialContextExtentType enumeration defines how the extent of
/// a context should be handled by the provider.
public enum class SpatialContextExtentType
{
    /// The spatial extent of the context is static and must be specified
    /// when the context is created. 
	SpatialContextExtentType_Static = FdoSpatialContextExtentType_Static,

    /// The spatial extent of the context is dynamic and changes as data is
    /// added and removed from the context. 
	SpatialContextExtentType_Dynamic = FdoSpatialContextExtentType_Dynamic
};

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_FDO_COMMANDS_SPATIALCONTEXT
/// \endcond
