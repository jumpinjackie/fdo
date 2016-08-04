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

#include "FDO\Expression\mgExpressionType.h"

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
enum class ExpressionType;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

ref class FunctionDefinitionCollection;

/// \ingroup (OSGeoFDOConnectionsCapabilities)
/// \interface OSGeo::FDO::Connections::Capabilities::IExpressionCapabilities
/// \brief
/// The IExpressionCapbilities interface declares the feature provider's level
/// of support for Expression classes.
public interface class IExpressionCapabilities : public System::IDisposable
{
public:
    /// \brief
    /// Returns an array of ExpressionType objects the feature provider supports.
    /// 
    /// \return
    /// Returns the list of expression types
    /// 
    property array<NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionType>^ ExpressionTypes
    {
        array<NAMESPACE_OSGEO_FDO_EXPRESSION::ExpressionType>^ get();
    }

    /// \brief
    /// Returns a collection of FunctionDefinition objects the feature provider supports within expressions.
    /// 
    /// \return
    /// Returns the collection of function definitions
    /// 
    property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection^ Functions
    {
        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionDefinitionCollection^ get();
    }
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES


