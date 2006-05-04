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

#include "mgGeometryType.h"

BEGIN_NAMESPACE_OSGEO_RUNTIME
public __gc class Disposable;
END_NAMESPACE_OSGEO_RUNTIME

BEGIN_NAMESPACE_OSGEO_COMMON_IO
public __gc class IoStream;
public __gc class IoTextWriter;
public __gc class IoTextReader;
END_NAMESPACE_OSGEO_COMMON_IO

BEGIN_NAMESPACE_OSGEO_COMMON_XML
public __gc __interface IXmlSaxHandler;
public __gc class XmlReader;
public __gc class XmlWriter;
public __gc class XmlSaxContext;
public __gc class XmlAttributeCollection;
public __gc class XmlAttribute;
END_NAMESPACE_OSGEO_COMMON_XML

BEGIN_NAMESPACE_OSGEO_COMMON
public __gc __interface IStreamReader;
public __gc class StringElement;
public __gc class DictionaryElement;
public __gc class DictionaryElementCollection;
public __gc class StringCollection;

public __gc __sealed class ObjectFactory
{
public:
    static NAMESPACE_OSGEO_COMMON::DictionaryElementCollection* CreateDictionaryElementCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON::StringElement* CreateStringElement(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON::DictionaryElement* CreateDictionaryElement(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON::StringCollection* CreateStringCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON::IStreamReader* CreateIStreamReader(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_COMMON_XML::XmlReader* CreateXmlReader(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON_XML::XmlWriter* CreateXmlWriter(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* CreateXmlSaxContext(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection* CreateXmlAttributeCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON_XML::XmlAttribute* CreateXmlAttribute(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler* CreateIXmlSaxHandler(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* CreateIoTextWriter(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON_IO::IoTextReader* CreateIoTextReader(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_COMMON_IO::IoStream* CreateIoStream(System::IntPtr ptr, System::Boolean autoDispose);

private:
	ObjectFactory(){}
};

END_NAMESPACE_OSGEO_COMMON


