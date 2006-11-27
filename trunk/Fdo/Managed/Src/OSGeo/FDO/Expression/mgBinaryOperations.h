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

/// \brief
/// The FdoBinaryOperations enumeration lists the binary expression operations
/// available.
/// <param name="FdoBinaryOperations_Add">
/// Add the two expressions or concatenate two string expressions
/// </param>
/// <param name="FdoBinaryOperations_Subtract">
/// Subtract the two expressions
/// </param>
/// <param name="FdoBinaryOperations_Multiply">
/// Multiply the two expressions
/// </param>
/// <param name="FdoBinaryOperations_Divide">
/// Divide the two expressions
/// </param>
[System::FlagsAttribute]
public  __value enum BinaryOperations
{
	BinaryOperations_Add = FdoBinaryOperations_Add,

	BinaryOperations_Subtract = FdoBinaryOperations_Subtract,

	BinaryOperations_Multiply = FdoBinaryOperations_Multiply,

	BinaryOperations_Divide = FdoBinaryOperations_Divide
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


