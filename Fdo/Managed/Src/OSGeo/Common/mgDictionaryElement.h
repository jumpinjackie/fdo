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

#include "Runtime\Disposable.h"

class FdoDictionaryElement;

BEGIN_NAMESPACE_OSGEO_COMMON

/// \ingroup (OSGeoFDOCommon)
/// \brief
///     DictionaryElement is an element in a DictionaryElementCollection. It consists of a
///     name-value pair.
public ref class DictionaryElement sealed : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    ///     Constructs a new empty dictionary element
    /// 
    /// \param name 
    ///     The name of the element.
    /// 
    /// \param value 
    ///     The value of the element.
    /// 
	DictionaryElement(System::String^ name, System::String^ value);

    /// \brief
    ///     Constructs an elemnet based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    ///     Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    ///     Input Indicates if the constructed object should be automatically deleted 
    ///     once it no longer referenced.
    /// 
	DictionaryElement(System::IntPtr unmanaged, System::Boolean autoDelete);

    /// \brief
    ///     Gets the name of this dictionary element
    /// 
    /// \return
    ///     Returns the dictionary element name
    /// 
    property System::String^ Name
    {
        System::String^ get();
    }

    /// \brief
    ///     Indicates whether the name of the DictionaryElement can be changed once
    ///     it has been created.
    /// 
    /// \return
    ///     Returns true if the DictionaryElement name can be changed; otherwise false.
    /// 
    property System::Boolean CanSetName
    {
        System::Boolean get();
    }

    /// \brief
    ///     Gets the value of this dictionary element.
    /// 
    /// \return
    ///     Returns the element value
    /// 
    /// \brief
    ///     Sets the value of this dictionary element
    /// 
    /// \param value 
    ///     Input the value of the dictionary element
    /// 
    property System::String^ Value
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

/// \cond DOXYGEN-IGNORE
internal:
	inline FdoDictionaryElement* GetImpObj();
/// \endcond
};

END_NAMESPACE_OSGEO_COMMON


