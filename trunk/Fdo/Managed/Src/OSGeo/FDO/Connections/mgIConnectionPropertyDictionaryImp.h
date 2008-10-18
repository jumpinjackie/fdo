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

#include "FDO\Connections\mgIConnectionPropertyDictionary.h"

class FdoIConnectionPropertyDictionary;

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS

/// \ingroup (OSGeoFDOConnections)
/// \brief
/// The IConnectionPropertyDictionary exposes a dictionary style interface
/// to the connection properties. The properties required to establish a connection
/// can be determined and their values can be set through this interface.
/// Modifying connection properties through this
/// interface dynamically modifies the connection string held by the connection
/// object. When the connection is in an open state, calling SetProperty on this
/// interface will result in an error.
private __gc class IConnectionPropertyDictionaryImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, public NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnectionPropertyDictionary
{
public:
    /// \brief
    /// Gets the names of all the properties that can appear in a connection string
    /// for this feature provider as an array of Strings. The order of the property
    /// names in the resulting array dictate the order in which they need to be 
    /// specified. This is especially important for the success of the 
    /// EnumeratePropertyValues method because properties that occur earlier in the array
    /// may be required for successful enumeration of properties that appear later.
    /// 
    /// \return
    /// Returns the list of parameter names
    /// 
    __property System::String* get_PropertyNames() [];

    /// \brief
    /// Gets the value of the specified property.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the property value.
    /// 
    System::String* GetProperty(System::String* name);

    /// \brief
    /// Sets the value of the specified property. An exception is thrown if the connection is currently open.
    /// 
    /// \param name 
    /// Input the property name
    /// \param value 
    /// Input the property value
    /// 
    /// \return
    /// Returns nothing
    /// 
    System::Void SetProperty( System::String* name,  System::String* value);

    /// \brief
    /// Gets the default value for the specified property.
    /// 
    /// \param name 
    /// Input the property name
    /// 
    /// \return
    /// Returns the property default value
    /// 
    System::String* GetPropertyDefault( System::String* name);

    /// \brief
    /// Determines if the specified property is required.
    /// 
    /// \param name 
    /// Input the property name
    /// 
    /// \return
    /// Returns true if the specified property is required
    /// 
    System::Boolean IsPropertyRequired( System::String* name);

    /// \brief
    ///  Indicates if the property is a password or other protected field
    /// that should be kept secure.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns true if the property is a password or other protected field
    /// that should be kept secure.
    /// 
    System::Boolean IsPropertyProtected(System::String* name);

    /// \brief
    /// Determines if the possible values for the specified property can be enumerated via the EnumeratePropertyValues method.
    /// 
    /// \param name 
    /// Input the property name
    /// 
    /// \return
    /// Returns true if the possible values for the specified property can be enumerated.
    /// 
    System::Boolean IsPropertyEnumerable(System::String* name);

    /// \brief
    ///  Returns an array of possible values for the specified property.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the list of values for this property.
    /// 
    System::String* EnumeratePropertyValues(System::String* name) [];

    /// \brief
    ///  Gets a localized name for the property (for NLS purposes).
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the localized name for the property (for NLS purposes).
    /// 
    System::String* GetLocalizedName(System::String* name);

    /// \brief
    /// Determines if the specified property represents a file name.
    /// 
    /// \param name 
    /// Input the property name
    /// 
    /// \return
    /// Returns true if the specified property is a file name
    /// 
    System::Boolean IsPropertyFileName(System::String* name);

    /// \brief
    /// Determines if the specified property represents a path name.
    /// 
    /// \param name 
    /// Input the property name
    /// 
    /// \return
    /// Returns true if the specified property is a path name
    /// 
    System::Boolean IsPropertyFilePath(System::String* name);

/// \cond DOXYGEN-IGNORE
protected:
    System::Void ReleaseUnmanagedObject();
/// \endcond

public private:
    IConnectionPropertyDictionaryImp(System::IntPtr unmanaged, System::Boolean autoDelete);

    inline FdoIConnectionPropertyDictionary* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS


