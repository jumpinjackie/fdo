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

class FdoProvider;

BEGIN_NAMESPACE_OSGEO_FDO_CLIENTSERVICES

/// \ingroup (OSGeoFDOClientServices)
/// \brief
/// The Provides class provides information about a feature provider, including name, 
/// description, library, and version information.
public __sealed __gc class Provider : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Gets the unique name of the feature provider. This name should be of the form 
    /// [Company].[Provider].[Version].
    /// 
    /// \return
    /// Returns the name of the Provider.
    /// Throws an instance of Exception if an error occurs.
    /// 
	__property System::String* get_Name();

    /// \brief
    /// Gets a user friendly display name of the feature provider.
    /// 
    /// \return
    /// Returns the display name of the Provider.
    /// Throws an instance of Exception if an error occurs.
    /// 
	__property System::String* get_DisplayName();
    
    /// \brief
    /// Gets a brief description of the feature provider.
    /// 
    /// \return
    /// Returns the description of the Provider.
    /// Throws an instance of Exception if an error occurs.
    /// 
	__property System::String* get_Description();
    
    /// \brief
    /// Gets the version of the feature provider. The version number string has the form 
    /// [VersionMajor].[VersionMinor].[BuildMajor].[BuildMinor].
    /// 
    /// \return
    /// Returns the version of the Provider.
    /// Throws an instance of Exception if an error occurs.
    /// 
	__property System::String* get_Version();

    /// \brief
    /// Gets the version of the feature data objects specification the feature provider conforms to. 
    /// The version number string has the form [VersionMajor].[VersionMinor].[BuildMajor].[BuildMinor].
    /// 
    /// \return
    /// Returns the Feature Data Objects version of the Provider.
    /// Throws an instance of Exception if an error occurs.
    /// 
	__property System::String* get_FeatureDataObjectsVersion();

    /// \brief
    /// Gets the FULL library path + library name of the provider. 
    /// 
    /// \return
    /// Returns the library path of the Provider.
    /// Throws an instance of Exception if an error occurs.
    /// 
	__property System::String* get_LibraryPath();

    /// \brief
    /// Gets a boolean flag indicating if the provider is a managed or unmanaged provider. 
    /// 
    /// \return
    /// Returns the managed state flag as a bool.
    /// Throws an instance of Exception if an error occurs.
    /// 
	__property System::Boolean get_IsManaged();

public private:
	Provider(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{
	}

	inline FdoProvider* GetImpObj();

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond
};

END_NAMESPACE_OSGEO_FDO_CLIENTSERVICES


