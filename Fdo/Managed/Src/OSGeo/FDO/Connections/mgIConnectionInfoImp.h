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

#include "FDO\Connections\mgIConnectionInfo.h"

class FdoIConnectionInfo;

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS

/// \brief
/// The FdoIConnectionInfo interface exposes information about the feature provider
/// and provides access to the FdoIConnectionPropertyDictionary interface.
private __gc class IConnectionInfoImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionInfo
{
public:
    /// \brief
    /// Gets the name of the feature provider.
    /// 
    /// \return
    /// Returns the provider name
    /// 
	__property System::String* get_ProviderName();

    /// \brief
    /// Gets the display name of the feature provider.
    /// 
    /// \return
    /// Returns the provider's display name
    /// 
	__property System::String* get_ProviderDisplayName();

    /// \brief
    /// Gets the description of the feature provider.
    /// 
    /// \return
    /// Returns the provider description
    /// 
	__property System::String* get_ProviderDescription();

    /// \brief
    /// Gets the version of the feature provider.
    /// 
    /// \return
    /// Returns provider version
    /// 
	__property System::String* get_ProviderVersion();

    /// \brief
    /// Gets the version of the Feature Data Objects specification to which this provider conforms.
    /// 
    /// \return
    /// Returns FDO version supported.
    /// 
	__property System::String* get_FeatureDataObjectsVersion();

    /// \brief
    /// Gets the FdoIConnectionPropertyDictionary interface that can be used to dynamically query and set the properties required to establish a connection.
    /// 
    /// \return
    /// Returns the property dictionary
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionary* get_ConnectionProperties();

public private:
	IConnectionInfoImp(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoIConnectionInfo* GetImpObj();

protected:
	System::Void Dispose(System::Boolean disposing);
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS


