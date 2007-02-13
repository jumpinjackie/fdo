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

BEGIN_NAMESPACE_OSGEO_FDO_CLIENTSERVICES
public __gc class ProviderCollection;
END_NAMESPACE_OSGEO_FDO_CLIENTSERVICES

BEGIN_NAMESPACE_OSGEO_FDO

/// \brief
/// The IProviderRegistry interface supports registering, un-registering, and enumerating 
/// registered feature providers. 
///
/// \note
/// This is not the Windows registry.
public __gc __interface IProviderRegistry : public System::IDisposable
{
public:
    /// \brief
    /// Gets a read only collection of information describing each of the registered feature providers. 
    /// 
    /// \return
    /// Returns an an instance of ProviderCollection. Throws an instance of Exception if an error occurs.
    /// 
	NAMESPACE_OSGEO_FDO_CLIENTSERVICES::ProviderCollection* GetProviders();

    /// \brief
    /// Registers a provider given the necessary information to register the provider. 
    /// If any of the parameters are missing or invalid, an exception is thrown. 
    /// 
    /// \param name 
    /// The unique name of the feature provider. This name should be of the form 
    /// [Company].[Provider].[Version].
    /// \param displayName 
    /// A user friendly display name of the feature provider.
    /// \param description 
    /// A brief description of the feature provider.
    /// \param version 
    /// The version of the feature provider. The version number string has the form 
    /// [VersionMajor].[VersionMinor].[BuildMajor].[BuildMinor].
    /// \param fdoVersion 
    /// The version of the feature data objects specification the feature provider conforms to. 
    /// The version number string has the form [VersionMajor].[VersionMinor].[BuildMajor].[BuildMinor].
    /// \param libraryPath 
    /// The FULL library path + library name of the provider.
    /// \param isManaged 
    /// A flag indicating if the provider is a managed or unmanaged .Net provider.
    /// 
    /// \return
    /// Returns nothing. Throws an instance of Exception if an error occurs.
    /// 
	System::Void RegisterProvider(String * name, 
		String * displayName, 
		String * description, 
		String * version, 
		String * fdoVersion, 
		String * libraryPath,
		System::Boolean isManaged);

    /// \brief
    /// Unregisters the provider with the specified name. 
    /// 
    /// \param name 
    /// The unique name of the feature provider. This name should be of the form 
    /// [Company].[Provider].[Version].
    /// 
    /// \return
    /// Returns nothing. Throws an instance of Exception if an error occurs.
    /// 
	System::Void UnregisterProvider(String * name);

};

END_NAMESPACE_OSGEO_FDO


