#ifndef _ITEMEXPRESSIONTYPE_H_
#define _ITEMEXPRESSIONTYPE_H_
// 

//
// Copyright (C) 2004-2007  Autodesk, Inc.
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


/// \ingroup (enums)
/// \brief
/// The FdoExpressionType enumeration defines the expression types available. This
/// enumeration is used in the capabilities metadata.
enum FdoExpressionItemType
{
   /// Identifier e.g. PROP
   FdoExpressionItemType_Identifier,
   /// Identifier e.g. PROP*5 AS EXP
   FdoExpressionItemType_ComputedIdentifier,
   /// Identifier e.g. SELECT PROP FROM CLS
   FdoExpressionItemType_SubSelectExpression,
   /// Parameter e.g. :PROP
   FdoExpressionItemType_Parameter,
   /// Function e.g. sin()
   FdoExpressionItemType_Function,
   /// Expression e.g. A+B
   FdoExpressionItemType_BinaryExpression,
   /// Expression e.g. -A
   FdoExpressionItemType_UnaryExpression,
   /// Data value e.g. 12, 12.4, 'text'
   FdoExpressionItemType_DataValue,
   /// Geometry e.g. GeomFromText('POLYGON XYZ..')
   FdoExpressionItemType_GeometryValue
};
#endif
