//
// Copyright (C) 2004-2006  Autodesk, Inc.
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

#pragma  once

/// \cond DOXYGEN-IGNORE
BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS
/// \endcond

/// \ingroup (OSGeoFDOConnections)
/// \brief
/// The ProviderDatastoreType enumeration lists all of the concrete provider
/// types supported by FDO.
public enum class ProviderDatastoreType
{

    /// Indicates that the provider is a database-based provider.
    ProviderDatastoreType_DatabaseServer = FdoProviderDatastoreType_DatabaseServer,

    /// Indicates that the provider is a file-based provider.
    ProviderDatastoreType_File = FdoProviderDatastoreType_File,

    /// Indicates that the provider is a web-based provider.
    ProviderDatastoreType_WebServer = FdoProviderDatastoreType_WebServer,

    /// Indicates that the provider is of an unknown provider type.
    ProviderDatastoreType_Unknown = FdoProviderDatastoreType_Unknown

};  

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_FDO_CONNECTIONS
/// \endcond
