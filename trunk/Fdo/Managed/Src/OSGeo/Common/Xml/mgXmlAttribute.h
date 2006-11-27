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

class FdoXmlAttribute;

BEGIN_NAMESPACE_OSGEO_COMMON_XML

/// \brief
/// XmlAttribute contains the name and value of a single attribute
/// from an XML document.
public __gc __sealed class XmlAttribute : public  NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Constructs an XML Attribute object
    /// 
    /// \param name 
    /// Input unique attribute name. If the attribute name is namespace qualified
    /// the name is {uri}:{localName}. Otherwise, it is {localName}
    /// \param value 
    /// Input attribute value.
    /// \param localName 
    /// Input attribute name without namespace qualification.
    /// \param uri 
    /// Input uri for the attribute namespace. L"" if the attribute
    /// name is not namespace qualified.
    /// \param prefix 
    /// Input prefix for the attribute namespace. L"" if the attribute
    /// name is not namespace qualified.
    /// \param valueUri 
    /// Input uri for the attribute value's namespace. L"" if the attribute
    /// value is not namespace qualified.
    /// \param localValue 
    /// Input attribute value without namespace qualification.
    /// \param valuePrefix 
    /// Input prefix for the attribute value's namespace. L"" if the attribute
    /// value is not namespace qualified.
    /// 
    /// \return
    /// Returns FdoXmlAttribute
    /// 
	XmlAttribute(System::String* name, System::String* value, System::String* localName, System::String* attributeURI, System::String* prefix, System::String* valueURI, System::String* localValue, System::String* valuePrefix);
	XmlAttribute(System::String* name, System::String* value, System::String* localName, System::String* attributeURI, System::String* prefix, System::String* valueURI, System::String* localValue);
	XmlAttribute(System::String* name, System::String* value, System::String* localName, System::String* attributeURI, System::String* prefix, System::String* valueURI);
	XmlAttribute(System::String* name, System::String* value, System::String* localName, System::String* attributeURI, System::String* prefix);
	XmlAttribute(System::String* name, System::String* value, System::String* localName, System::String* attributeURI);
	XmlAttribute(System::String* name, System::String* value, System::String* localName);
	XmlAttribute(System::String* name, System::String* value);

	XmlAttribute(System::IntPtr unmanaged, System::Boolean autoDelete);

protected:
	virtual System::Void ReleaseUnmanagedObject();

public private:
	inline FdoXmlAttribute* GetImpObj();

private:
	System::Void InitConstruct(String* name, String* value, String* localName, String* uri, String* prefix, String* valueUri, String* localValue, String* valuePrefix);
};

END_NAMESPACE_OSGEO_COMMON_XML


