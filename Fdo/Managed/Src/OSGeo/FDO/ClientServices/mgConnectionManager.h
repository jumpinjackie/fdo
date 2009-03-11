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

#include "FDO\mgIConnectionManager.h"

class FdoConnectionManager;

BEGIN_NAMESPACE_OSGEO_FDO_CLIENTSERVICES

/// \ingroup (OSGeoFDOClientServices)
/// \brief
/// The ConnectionManager class supports dynamic creation of connection objects 
/// given a provider name. Derives from the IConnectionManager interface.
///
/// \remarks
/// In order for dynamic creation to work, each dynamic link library, shared library, 
/// or assembly that implements a provider must supply a well-defined entry point function that 
/// takes as input a provider name and returns an IConnection instance. The definition of the 
/// entry point function is language- and platform-dependent. For this release of FDO, the 
/// windows libraries supporting FDO providers will be expected to support the following well-known 
/// unmanaged entry point function, which will create and return an unitialized connection object:
///
///     FdoIConnection^ CreateConnection();
///
public ref class ConnectionManager sealed : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_FDO::IConnectionManager
{
public:
    /// \brief
    /// Creates an unitialized connection object given the provider name.
    /// 
    /// \param providerName 
    /// The unique name of the feature provider. This name should be of the form 
    /// [Company].[Provider].[Version].
    /// 
    /// \return
    /// Returns an instance of an IConnection object. Throws an instance of Exception if an error occurs.
    /// 
	virtual NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnection^ CreateConnection(String^ providerName);

    /// \brief
    /// Frees a connection library reference given the provider name.
    /// 
    /// \param providerName 
    /// The unique name of the feature provider. This name should be of the form 
    /// [Company].[Provider].[Version].
    /// 
    /// \return
    /// Returns nothing. Throws an instance of Exception if an error occurs.
    /// 
	virtual System::Void FreeLibrary(String^ providerName);

internal:
	ConnectionManager(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoConnectionManager* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_CLIENTSERVICES


