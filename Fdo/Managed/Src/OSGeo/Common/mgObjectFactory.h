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
ref class Disposable;
END_NAMESPACE_OSGEO_RUNTIME

BEGIN_NAMESPACE_OSGEO_COMMON_IO
ref class IoStream;
ref class IoTextWriter;
ref class IoTextReader;
END_NAMESPACE_OSGEO_COMMON_IO

BEGIN_NAMESPACE_OSGEO_COMMON_XML
interface class IXmlSaxHandler;
ref class XmlReader;
ref class XmlWriter;
ref class XmlSaxContext;
ref class XmlAttributeCollection;
ref class XmlAttribute;
END_NAMESPACE_OSGEO_COMMON_XML

BEGIN_NAMESPACE_OSGEO_COMMON
interface class IStreamReader;
ref class StringElement;
ref class DictionaryElement;
ref class DictionaryElementCollection;
ref class StringCollection;

/// \ingroup (OSGeoFDOCommon)
public ref class ObjectFactory sealed
{
public:
    static NAMESPACE_OSGEO_COMMON::DictionaryElementCollection^ CreateDictionaryElementCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON::StringElement^ CreateStringElement(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON::DictionaryElement^ CreateDictionaryElement(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON::StringCollection^ CreateStringCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON::IStreamReader^ CreateIStreamReader(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_COMMON_XML::XmlReader^ CreateXmlReader(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ CreateXmlWriter(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ CreateXmlSaxContext(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection^ CreateXmlAttributeCollection(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON_XML::XmlAttribute^ CreateXmlAttribute(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler^ CreateIXmlSaxHandler(System::IntPtr ptr, System::Boolean autoDispose);

    static NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ CreateIoTextWriter(System::IntPtr ptr, System::Boolean autoDispose);
    static NAMESPACE_OSGEO_COMMON_IO::IoTextReader^ CreateIoTextReader(System::IntPtr ptr, System::Boolean autoDispose);
	static NAMESPACE_OSGEO_COMMON_IO::IoStream^ CreateIoStream(System::IntPtr ptr, System::Boolean autoDispose);

private:
	ObjectFactory(){}
};

END_NAMESPACE_OSGEO_COMMON


