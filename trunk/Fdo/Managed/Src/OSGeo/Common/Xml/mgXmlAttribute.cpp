/*
* 
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

#include "stdafx.h"
#include "Common\mgException.h"
#include "Common\Xml\mgXmlAttribute.h"

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlAttribute::InitConstruct(String^ name, String^ value, String^ localName, String^ attributeURI, String^ prefix, String^ valueURI, String^ localValue, String^ valuePrefix)
{	
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlAttribute::Create(StringToUni(name), StringToUni(value), StringToUni(localName), StringToUni(attributeURI), StringToUni(prefix), StringToUni(valueURI), StringToUni(localValue), StringToUni(valuePrefix))), true)) 
}

NAMESPACE_OSGEO_COMMON_XML::XmlAttribute::XmlAttribute(String^ name, String^ value, String^ localName, String^ attributeURI, String^ prefix, String^ valueURI, String^ localValue, String^ valuePrefix) : Disposable(IntPtr::Zero, false)
{
	InitConstruct(name, value, localName, attributeURI, prefix, valueURI, localValue, valuePrefix );
}

NAMESPACE_OSGEO_COMMON_XML::XmlAttribute::XmlAttribute(System::String^ name, System::String^ value, System::String^ localName, System::String^ attributeURI, System::String^ prefix, System::String^ valueURI, System::String^ localValue) : Disposable(IntPtr::Zero, false)
{
	InitConstruct(name, value, localName, attributeURI, prefix, valueURI, localValue, String::Empty );
}

NAMESPACE_OSGEO_COMMON_XML::XmlAttribute::XmlAttribute(System::String^ name, System::String^ value, System::String^ localName, System::String^ attributeURI, System::String^ prefix, System::String^ valueURI) : Disposable(IntPtr::Zero, false)
{
	InitConstruct(name, value, localName, attributeURI, prefix, valueURI, String::Empty, String::Empty );
}

NAMESPACE_OSGEO_COMMON_XML::XmlAttribute::XmlAttribute(System::String^ name, System::String^ value, System::String^ localName, System::String^ attributeURI, System::String^ prefix) : Disposable(IntPtr::Zero, false)
{
	InitConstruct(name, value, localName, attributeURI, prefix, String::Empty, String::Empty, String::Empty );
}

NAMESPACE_OSGEO_COMMON_XML::XmlAttribute::XmlAttribute(System::String^ name, System::String^ value, System::String^ localName, System::String^ attributeURI) : Disposable(IntPtr::Zero, false)
{
	InitConstruct(name, value, localName, attributeURI, String::Empty, String::Empty, String::Empty, String::Empty );
}

NAMESPACE_OSGEO_COMMON_XML::XmlAttribute::XmlAttribute(System::String^ name, System::String^ value, System::String^ localName) : Disposable(IntPtr::Zero, false)
{
	InitConstruct(name, value, localName, String::Empty, String::Empty, String::Empty, String::Empty, String::Empty );
}

NAMESPACE_OSGEO_COMMON_XML::XmlAttribute::XmlAttribute(String^ name, String^ value) : Disposable(IntPtr::Zero, false)
{
	InitConstruct(name, value, String::Empty, String::Empty, String::Empty, String::Empty, String::Empty, String::Empty );
}

NAMESPACE_OSGEO_COMMON_XML::XmlAttribute::XmlAttribute(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

FdoXmlAttribute* NAMESPACE_OSGEO_COMMON_XML::XmlAttribute::GetImpObj()
{
	return static_cast<FdoXmlAttribute*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_COMMON_XML::XmlAttribute::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}
