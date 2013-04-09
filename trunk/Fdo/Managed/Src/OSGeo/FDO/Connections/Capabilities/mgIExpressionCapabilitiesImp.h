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

/// \ingroup (OSGeoFDOConnectionsCapabilities)
/// \brief
/// The IExpressionCapbilities interface declares the feature provider's level
/// of support for Expression classes.
private ref class IExpressionCapabilitiesImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, 
                                                public NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::IExpressionCapabilities
{
public:
    /// \brief
    /// Returns an array of ExpressionType objects the feature provider supports.
    /// 
    /// \return
    /// Returns the list of expression types
    /// 
    virtual property array<NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionType>^ ExpressionTypes
    {
        array<NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionType>^ get();
    }

    /// \brief
    /// Returns a collection of FunctionDefinition objects the feature provider supports within expressions.
    /// 
    /// \return
    /// Returns the collection of function definitions
    /// 
    virtual property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection^ Functions
    {
        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection^ get();
    }

internal:
	IExpressionCapabilitiesImp(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoIExpressionCapabilities* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES


