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

#include "FDO\Connections\mgProviderDatastoreType.h"

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS
interface class IConnectionPropertyDictionary;

/// \ingroup (OSGeoFDOConnections)
/// \interface OSGeo::FDO::Connections::IConnectionInfo
/// \brief
/// The IConnectionInfo interface exposes information about the feature provider
/// and provides access to the IConnectionPropertyDictionary interface.
public interface class IConnectionInfo : public System::IDisposable
{
public:
    /// \brief
    /// Gets the name of the feature provider.
    /// 
    /// \return
    /// Returns the provider name
    /// 
    property System::String^ ProviderName
    {
        System::String^ get();
    }

    /// \brief
    /// Gets the display name of the feature provider.
    /// 
    /// \return
    /// Returns the provider's display name
    /// 
    property System::String^ ProviderDisplayName
    {
        System::String^ get();
    }

    /// \brief
    /// Gets the description of the feature provider.
    /// 
    /// \return
    /// Returns the provider description
    /// 
    property System::String^ ProviderDescription
    {
        System::String^ get();
    }

    /// \brief
    /// Gets the version of the feature provider.
    /// 
    /// \return
    /// Returns provider version
    /// 
    property System::String^ ProviderVersion
    {
        System::String^ get();
    }

    /// \brief
    /// Gets the version of the Feature Data Objects specification to which this provider conforms.
    /// 
    /// \return
    /// Returns FDO version supported.
    /// 
    property System::String^ FeatureDataObjectsVersion
    {
        System::String^ get();
    }

    /// \brief
    /// Gets the IConnectionPropertyDictionary interface that can be used to dynamically query and set the properties required to establish a connection.
    /// 
    /// \return
    /// Returns the property dictionary
    /// 
    property NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionary^ ConnectionProperties
    {
        NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionary^ get();
    }

    /// \brief
    /// Returns the provider type. A provider can be a file-based, database-based or
    /// web-based provider. The valid values the function may return are defined in
    /// the enumeration ProviderDatastoreType. The enumeration includes the following
    /// values: ProviderDatastoreType_Unknown, ProviderDatastoreType_File,
    /// ProviderDatastoreType_DatabaseServer, ProviderDatastoreType_WebServer.
    /// 
    /// \return
    /// Returns the provider data store type.
    /// 
    property NAMESPACE_OSGEO_FDO_CONNECTIONS::ProviderDatastoreType ProviderDatastoreType
    {
        NAMESPACE_OSGEO_FDO_CONNECTIONS::ProviderDatastoreType get();
    }

    /// \brief
    /// File-based providers depend on a various files. This function returns a list
    /// of fully qualified dependend file names. The return parameter will be nullptr if
    /// the provider is not a file-based provider.
    /// 
    /// \return
    /// Returns the list of fully-qualified dependend file names if the provider is a
    /// file-based provider, nullptr otherwise.
    /// 
    property NAMESPACE_OSGEO_COMMON::StringCollection^ DependentFileNames
    {
        NAMESPACE_OSGEO_COMMON::StringCollection^ get();
    }
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS


