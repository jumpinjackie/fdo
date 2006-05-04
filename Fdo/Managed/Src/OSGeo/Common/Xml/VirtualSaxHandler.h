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

#include <Common.h>
#include "Runtime\ProtocolExt.h"
#include "Common\Xml\mgXmlSaxHandler.h"

template <typename WrapperType, typename Base>
class VirtualObject;

class FdoXmlSaxHandler;
class FdoXmlSaxContext;

BEGIN_NAMESPACE_OSGEO_COMMON_XML
public __gc class XmlSaxHandler;
public __gc class XmlSaxContext;
public __gc class XmlAttributeCollection;
END_NAMESPACE_OSGEO_COMMON_XML

using namespace NAMESPACE_OSGEO_COMMON_XML;

class VirtualSaxHandler : public VirtualObject <NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler, FdoXmlSaxHandler>
{
public:
	FdoXmlSaxHandler* XmlStartDocument(FdoXmlSaxContext* saxContext);

	System::Void XmlEndDocument(FdoXmlSaxContext* saxContext);

	FdoXmlSaxHandler* XmlStartElement(FdoXmlSaxContext* saxcontext, FdoString* uri, FdoString* name, FdoString* qname, FdoXmlAttributeCollection* atts);

	FdoBoolean XmlEndElement(FdoXmlSaxContext* saxContext, FdoString* uri, FdoString* name, FdoString* qname);

	System::Void XmlCharacters(FdoXmlSaxContext* saxContext, FdoString* chars);

private:
	enum WrapperCallBits
	{
		XmlStartDocumentBit				= 0x01,
		XmlEndDocumentBit			        = 0x01 << 1,
		XmlStartElementBit					= 0x01 << 2,
		XmlEndElementBit					= 0x01 << 3,
		XmlCharactersBit						= 0x01 << 4
	};

	mutable FdoInt32 wrapperCallBits;
};


