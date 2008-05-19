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

class FdoStringElement;

BEGIN_NAMESPACE_OSGEO_COMMON

/// \ingroup (OSGeoFDOCommon)
/// \brief
///     StringElement is an element in a StringCollection.
public __sealed __gc class StringElement : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    ///     Constructs a new string element based on an input string
    /// 
    /// \param value 
    ///     The string value of the element.
    /// 
	StringElement(System::String* value);

    /// \brief
    ///     Constructs a string elemnet based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    ///     Input A Pointer to the unmanaged string element.
    /// 
    /// \param autoDelete 
    ///     Input Indicates if the constructed element should be automatically deleted 
    ///     once it no longer referenced.
    /// 
	StringElement(System::IntPtr unmanaged, System::Boolean autoDelete);

    /// \brief
    ///     Retrieves the string object contained in the string element
    /// 
    /// \param unmanaged 
    ///     Input A Pointer to the unmanaged string element.
    /// 
    /// \param autoDelete 
    ///     Input Indicates if the constructed element should be automatically deleted 
    ///     once it no longer referenced.
    /// 
    __property System::String* get_String ();

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();

public private:
	inline FdoStringElement* GetImpObj();
/// \endcond
};

END_NAMESPACE_OSGEO_COMMON


