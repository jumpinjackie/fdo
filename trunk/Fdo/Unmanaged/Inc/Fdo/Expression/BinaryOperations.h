#ifndef _BINARYOPERATIONS_H_
#define _BINARYOPERATIONS_H_
// 

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

#ifdef _WIN32
#pragma once
#endif

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
enum FdoBinaryOperations
{
    FdoBinaryOperations_Add,

    FdoBinaryOperations_Subtract,

    FdoBinaryOperations_Multiply,

    FdoBinaryOperations_Divide
};
#endif


