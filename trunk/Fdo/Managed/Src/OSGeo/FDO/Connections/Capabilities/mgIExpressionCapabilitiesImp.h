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

#include "FDO\Connections\Capabilities\mgIExpressionCapabilities.h"

class FdoIExpressionCapabilities;

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

/// \brief
/// The FdoIExpressionCapbilities interface declares the feature provider's level
/// of support for FdoExpression classes.
private __gc class IExpressionCapabilitiesImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilities
{
public:
    /// \brief
    /// Returns an array of FdoExpressionType objects the feature provider supports.
    /// 
    /// \param length 
    /// Input the number of expression types
    /// 
    /// \return
    /// Returns the list of expression types
    /// 
	__property NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionType get_ExpressionTypes() [];

    /// \brief
    /// Returns a collection of FdoFunctionDefinition objects the feature provider supports within expressions.
    /// 
    /// \return
    /// Returns the collection of function definitions
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection* get_Functions();

protected:
	System::Void ReleaseUnmanagedObject();

public private:
	IExpressionCapabilitiesImp(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoIExpressionCapabilities* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES


