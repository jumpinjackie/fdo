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

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS
public __gc __interface IConnection;
END_NAMESPACE_OSGEO_FDO_CONNECTIONS

BEGIN_NAMESPACE_OSGEO_FDO

public __gc __interface IConnectionManager : public System::IDisposable
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
    /// Returns an instance of an FdoIConnection object. Throws an instance of FdoException * if an error occurs.
    /// 
	NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnection* CreateConnection(String* providerName);

    /// \brief
    /// Frees a connection library reference given the provider name.
    /// 
    /// \param providerName 
    /// The unique name of the feature provider. This name should be of the form 
    /// [Company].[Provider].[Version].
    /// 
    /// \return
    /// Returns nothing. Throws an instance of FdoException * if an error occurs.
    /// 
	System::Void FreeLibrary(String* providerName);
};

END_NAMESPACE_OSGEO_FDO


