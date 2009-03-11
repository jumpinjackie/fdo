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

/// \cond DOXYGEN-IGNORE
BEGIN_NAMESPACE_OSGEO_FDO_FILTER
/// \endcond

/// \ingroup (OSGeoFDOFilter)
/// \brief
/// The DistanceOperations enumeration lists the distance conditions available.
[System::FlagsAttribute]
public enum class DistanceOperations
{
    /// Test to see if the geometric property value lies beyond a specified
    /// distance of a literal geometric value.
	DistanceOperations_Beyond = FdoDistanceOperations_Beyond,

    /// Test to see if the geometric property value lies within a specified
    /// distance of a literal geometric value.
	DistanceOperations_Within = FdoDistanceOperations_Within
};

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_FDO_FILTER
/// \endcond
