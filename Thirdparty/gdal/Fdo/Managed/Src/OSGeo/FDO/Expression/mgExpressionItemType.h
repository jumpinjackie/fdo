/*
* Copyright (C) 2004-2011  Autodesk, Inc.
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

#include <Fdo/Expression/ExpressionItemType.h>

/// \cond DOXYGEN-IGNORE
BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
/// \endcond

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The ExpressionItemType enumeration defines the expression types available.
public enum class ExpressionItemType
{
    /// Identifier e.g. PROP
	ExpressionItemType_Identifier = FdoExpressionItemType_Identifier,

    /// Computer identifier e.g. PROP*5 AS EXP
	ExpressionItemType_ComputedIdentifier = FdoExpressionItemType_ComputedIdentifier,

    /// Sub select e.g. SELECT PROP FROM CLS
	ExpressionItemType_SubSelectExpression = FdoExpressionItemType_SubSelectExpression,

    /// Parameter e.g. :PROP
	ExpressionItemType_Parameter = FdoExpressionItemType_Parameter,

    /// Function e.g. sin()
	ExpressionItemType_Function = FdoExpressionItemType_Function,

    /// Binary expression e.g. A+B
	ExpressionItemType_BinaryExpression = FdoExpressionItemType_BinaryExpression,

    /// Unary expression e.g. -A
	ExpressionItemType_UnaryExpression = FdoExpressionItemType_UnaryExpression,

    /// Data value e.g. 12, 12.4, 'text'
	ExpressionItemType_DataValue = FdoExpressionItemType_DataValue,

    /// Geometry e.g. GeomFromText('POLYGON XYZ..')
	ExpressionItemType_GeometryValue = FdoExpressionItemType_GeometryValue
};

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_FDO_EXPRESSION
/// \endcond
