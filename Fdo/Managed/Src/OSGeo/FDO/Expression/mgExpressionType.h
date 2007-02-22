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
BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
/// \endcond

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The ExpressionType enumeration defines the expression types available. This
/// enumeration is used in the capabilities metadata.
public __value enum ExpressionType
{
    /// Basic arithmetic expressions e.g.: +, -, *, /, negate, and string concatenation.
	ExpressionType_Basic = FdoExpressionType_Basic,

    /// Function evaluations
	ExpressionType_Function = FdoExpressionType_Function,

    /// Parameter substitutions
	ExpressionType_Parameter = FdoExpressionType_Parameter

};

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_FDO_EXPRESSION
/// \endcond
