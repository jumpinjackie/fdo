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

BEGIN_NAMESPACE_OSGEO_FDO_FILTER

/// \brief
/// The ConditionType enumeration defines the filter condition types available. 
/// This enumeration is used in the capabilities.
/// <param name="ConditionType_Comparison">
/// Tests if one expression is equal, not equal, greater than, less than, 
/// greater than or equal to, or less than or equal to another expression
/// </param>
/// <param name="ConditionType_Like">
/// Tests if the value of a specified data property matches a specified pattern
/// </param>
/// <param name="ConditionType_In">
/// Tests if the value of a specified data property is within a given set 
/// of literal values
/// </param>
/// <param name="ConditionType_Null">
/// Tests if the value of a specified data property is null
/// </param>
/// <param name="ConditionType_Spatial">
/// Tests whether the value of a geometric property and a literal geometric 
/// value satisfy the spatial relationship implied by the operation
/// </param>
/// <param name="ConditionType_Distance">
/// Tests whether the value of a geometric property is within or beyond a 
/// specified distance of a literal geometric value
/// </param>
public __value enum ConditionType
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
END_NAMESPACE_OSGEO_FDO_FILTER


