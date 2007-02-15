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

/// \ingroup (enums)
/// \brief
/// The ComparisonOperations enumeration lists the comparison operations
/// available.
/// \remarks
/// For the Like Operation, wildcard characters supported in the pattern are:
/// \li \c %    Any string of zero or more characters;
/// \li \c _    Any single character;
/// \li \c []   Any single character within the specified range ([a-f]) or set 
/// ([abcdef]);
/// \li \c [^]  Any single character not within the specified range ([^a-f]) or 
/// set ([^abcdef]).
/// 
[System::FlagsAttribute]
public __value enum ComparisonOperations
{
    /// Test to see if the left and right expressions are equal.
	ComparisonOperations_EqualTo = FdoComparisonOperations_EqualTo,

    /// Test to see if the left and right expressions are not equal.
	ComparisonOperations_NotEqualTo = FdoComparisonOperations_NotEqualTo,

    /// Test to see if the left expression is greater than the right expression.
	ComparisonOperations_GreaterThan = FdoComparisonOperations_GreaterThan,

    /// Test to see if the left expression is greater than or equal to the right expression.
	ComparisonOperations_GreaterThanOrEqualTo = FdoComparisonOperations_GreaterThanOrEqualTo,

    /// Test to see if the left expression is less than the right expression.
	ComparisonOperations_LessThan = FdoComparisonOperations_LessThan,

    /// Test to see if the left expression is less than or equal to the right expression.
	ComparisonOperations_LessThanOrEqualTo = FdoComparisonOperations_LessThanOrEqualTo,

    /// Test to see if the left expression matches the pattern specified by the
    /// right expression.
	ComparisonOperations_Like = FdoComparisonOperations_Like
};
END_NAMESPACE_OSGEO_FDO_FILTER


