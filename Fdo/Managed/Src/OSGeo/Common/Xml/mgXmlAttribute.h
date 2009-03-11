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

/// \ingroup (OSGeoFDOCommonXml)
/// \brief
/// XmlAttribute contains the name and value of a single attribute
/// from an XML document.
public ref class XmlAttribute sealed : public  NAMESPACE_OSGEO_RUNTIME::Disposable
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
    /// \param attributeURI 
    /// Input uri for the attribute namespace. L"" if the attribute
    /// name is not namespace qualified.
    /// \param prefix 
    /// Input prefix for the attribute namespace. L"" if the attribute
    /// name is not namespace qualified.
    /// \param valueURI 
    /// Input uri for the attribute value's namespace. L"" if the attribute
    /// value is not namespace qualified.
    /// \param localValue 
    /// Input attribute value without namespace qualification.
    /// \param valuePrefix 
    /// Input prefix for the attribute value's namespace. L"" if the attribute
    /// value is not namespace qualified.
    /// 
	XmlAttribute(System::String^ name, System::String^ value, System::String^ localName, System::String^ attributeURI, System::String^ prefix, System::String^ valueURI, System::String^ localValue, System::String^ valuePrefix);

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
    /// \param attributeURI 
    /// Input uri for the attribute namespace. L"" if the attribute
    /// name is not namespace qualified.
    /// \param prefix 
    /// Input prefix for the attribute namespace. L"" if the attribute
    /// name is not namespace qualified.
    /// \param valueURI 
    /// Input uri for the attribute value's namespace. L"" if the attribute
    /// value is not namespace qualified.
    /// \param localValue 
    /// Input attribute value without namespace qualification.
    /// 
	XmlAttribute(System::String^ name, System::String^ value, System::String^ localName, System::String^ attributeURI, System::String^ prefix, System::String^ valueURI, System::String^ localValue);

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
    /// \param attributeURI 
    /// Input uri for the attribute namespace. L"" if the attribute
    /// name is not namespace qualified.
    /// \param prefix 
    /// Input prefix for the attribute namespace. L"" if the attribute
    /// name is not namespace qualified.
    /// \param valueURI 
    /// Input uri for the attribute value's namespace. L"" if the attribute
    /// value is not namespace qualified.
    /// 
	XmlAttribute(System::String^ name, System::String^ value, System::String^ localName, System::String^ attributeURI, System::String^ prefix, System::String^ valueURI);

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
    /// \param attributeURI 
    /// Input uri for the attribute namespace. L"" if the attribute
    /// name is not namespace qualified.
    /// \param prefix 
    /// Input prefix for the attribute namespace. L"" if the attribute
    /// name is not namespace qualified.
    /// 
	XmlAttribute(System::String^ name, System::String^ value, System::String^ localName, System::String^ attributeURI, System::String^ prefix);
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
    /// \param attributeURI 
    /// Input uri for the attribute namespace. L"" if the attribute
    /// name is not namespace qualified.
    /// 
	XmlAttribute(System::String^ name, System::String^ value, System::String^ localName, System::String^ attributeURI);
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
    /// 
	XmlAttribute(System::String^ name, System::String^ value, System::String^ localName);

    /// \brief
    /// Constructs an XML Attribute object
    /// 
    /// \param name 
    /// Input unique attribute name. If the attribute name is namespace qualified
    /// the name is {uri}:{localName}. Otherwise, it is {localName}
    /// \param value 
    /// Input attribute value.
    /// 
	XmlAttribute(System::String^ name, System::String^ value);

    /// \brief
    /// Constructs an XmlAttribute based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged attribute.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	XmlAttribute(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
internal:
	inline FdoXmlAttribute* GetImpObj();

private:
	System::Void InitConstruct(String^ name, String^ value, String^ localName, String^ uri, String^ prefix, String^ valueUri, String^ localValue, String^ valuePrefix);
/// \endcond
};

END_NAMESPACE_OSGEO_COMMON_XML


