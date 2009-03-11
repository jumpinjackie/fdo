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

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The BinaryOperations enumeration lists the binary expression operations available.
[System::FlagsAttribute]
public enum class BinaryOperations
{
    /// Add the two expressions or concatenate two string expressions
	BinaryOperations_Add = FdoBinaryOperations_Add,

    /// Subtract the two expressions
	BinaryOperations_Subtract = FdoBinaryOperations_Subtract,

    /// Multiply the two expressions
	BinaryOperations_Multiply = FdoBinaryOperations_Multiply,

    /// Divide the two expressions
	BinaryOperations_Divide = FdoBinaryOperations_Divide
};

END_NAMESPACE_OSGEO_FDO_EXPRESSION


