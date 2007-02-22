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

#pragma  once


BEGIN_NAMESPACE_OSGEO_FDO
public __gc __interface IConnectionManager;
public __gc __interface IProviderRegistry;
END_NAMESPACE_OSGEO_FDO

BEGIN_NAMESPACE_OSGEO_FDO_CLIENTSERVICES

/// \ingroup (OSGeoFDOClientServices)
/// \brief
/// The FeatureAccessManager class manages the set of feature providers stored in the FDO provider registry
/// and provides support for dynamic discovery and binding to registered feature providers
public __sealed __gc class FeatureAccessManager 
{
public:
    /// \brief
    /// Static method that gets an object that implements IConnectionManager
    /// 
    /// \return
    /// Returns a static instance of an IConnectionManager object. 
    /// Throws an instance of Exception if an error occurs.
    /// 
	static NAMESPACE_OSGEO_FDO::IConnectionManager* GetConnectionManager();

    /// \brief
    /// Static method that gets an object that implements IProviderRegistry
    /// 
    /// \return
    /// Returns a static instance of an IProviderRegistry object. 
    /// Throws an instance of Exception if an error occurs.
    /// 
	static NAMESPACE_OSGEO_FDO::IProviderRegistry* GetProviderRegistry();

private:
	FeatureAccessManager(){}
};

END_NAMESPACE_OSGEO_FDO_CLIENTSERVICES


