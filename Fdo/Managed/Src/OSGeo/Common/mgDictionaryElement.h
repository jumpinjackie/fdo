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

/// \brief
///     DictionaryElement is an element in a DictionaryElementCollection. It consists of a
///     name-value pair.
public __sealed __gc class DictionaryElement : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
	DictionaryElement(System::String* name, System::String* value);

	DictionaryElement(System::IntPtr unmanaged, System::Boolean autoDelete);

    /// \brief
    ///     Gets the name of this element
    /// 
    /// \return
    ///     Returns the element name
    /// 
    ///  
	__property String* get_Name();

    /// \brief
    ///     Indicates that this object does not allow its name
    ///     to change. Not allowing name change allows more efficient 
    ///     random access to FdoDictionary.
    /// 
    /// \return
    ///     Returns false
    /// 
    ///  
	__property System::Boolean get_CanSetName();

    /// \brief
    ///     Gets the value of this dictionary element.
    /// 
    /// \return
    ///     Returns the element value
    /// 
    ///  
	__property System::String* get_Value();

    /// \brief
    ///     Sets the value of this dictionary element
    /// 
    /// \param value 
    /// Input the element value
    /// 
	__property System::Void set_Value(String* value);

protected:
	System::Void ReleaseUnmanagedObject();

public private:
	inline FdoDictionaryElement* GetImpObj();
};

END_NAMESPACE_OSGEO_COMMON


