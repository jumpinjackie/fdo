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
/// The ConditionType enumeration defines the filter condition types available. 
/// This enumeration is used in the capabilities.
public enum class ConditionType
{
    /// Tests if one expression is equal, not equal, greater than, less than, 
    /// greater than or equal to, or less than or equal to another expression.
	ConditionType_Comparison = FdoConditionType_Comparison,

    /// Tests if the value of a specified data property is matches a specified pattern.
	ConditionType_Like = FdoConditionType_Like,

    /// Tests if the value of a specified data property is within a given set 
    /// of literal values.
	ConditionType_In = FdoConditionType_In,

    /// Tests if the value of a specified data property is null.
	ConditionType_Null = FdoConditionType_Null,

    /// Tests whether the value of a geometric property and a literal geometric 
    /// value satisfy the spatial relationship implied by the operation.
	ConditionType_Spatial = FdoConditionType_Spatial,

    /// Tests whether the value of a geometric property is within or beyond a 
    /// specified distance of a literal geometric value.
	ConditionType_Distance = FdoConditionType_Distance
};

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_FDO_FILTER
/// \endcond
