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
#include "Common\Xml\VirtualSaxHandler.h"
#include "Common\Xml\mgXmlSaxHandler.h"
#include "Common\Xml\mgXmlSaxContext.h"
#include "Common\Xml\mgXmlAttributeCollection.h"
#include "Common\mgObjectFactory.h"

FdoXmlSaxHandler* VirtualSaxHandler::XmlStartDocument(FdoXmlSaxContext* saxContext)
{
	if (!WrapperCallWrapper::IsCalling(wrapperCallBits, XmlStartDocumentBit))
	{
		WrapperCallWrapper ctx(wrapperCallBits, XmlStartDocumentBit);
		XmlSaxContext* mgsaxcontext = NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlSaxContext(saxContext, true);
		XmlSaxHandler* mgreturn = static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler*>(GetWrapper()->XmlStartDocument(mgsaxcontext));
		return (FdoXmlSaxHandler*)mgreturn->GetImpObj();
	}
	else
	{
		return FdoXmlSaxHandler::XmlStartDocument(saxContext);
	}
}

System::Void VirtualSaxHandler::XmlEndDocument(FdoXmlSaxContext* saxContext)
{
	if (!WrapperCallWrapper::IsCalling(wrapperCallBits, XmlEndDocumentBit))
	{
		WrapperCallWrapper ctx(wrapperCallBits, XmlEndDocumentBit);
		XmlSaxContext* mgsaxcontext = NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlSaxContext(saxContext, true);
		GetWrapper()->XmlEndDocument(mgsaxcontext);
	}
	else
	{
		FdoXmlSaxHandler::XmlEndDocument(saxContext);
	}
}

FdoXmlSaxHandler* VirtualSaxHandler::XmlStartElement(FdoXmlSaxContext* saxcontext, FdoString* uri, FdoString* name, FdoString* qname, FdoXmlAttributeCollection* atts)
{
	if (!WrapperCallWrapper::IsCalling(wrapperCallBits, XmlStartElementBit))
	{
		WrapperCallWrapper ctx(wrapperCallBits, XmlStartElementBit);
		XmlSaxContext* mgsaxcontext = NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlSaxContext(saxcontext, true);
		XmlAttributeCollection* mgatts = NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlAttributeCollection(atts, true);

		XmlSaxHandler* mgreturn = static_cast<NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler*>(GetWrapper()->XmlStartElement(mgsaxcontext, uri, name, qname, mgatts));
		return (FdoXmlSaxHandler*)mgreturn->GetImpObj();
	}
	else
	{
		return FdoXmlSaxHandler::XmlStartElement(saxcontext, uri, name, qname, atts);
	}
}

FdoBoolean VirtualSaxHandler::XmlEndElement(FdoXmlSaxContext* saxContext, FdoString* uri, FdoString* name, FdoString* qname)
{
	if (!WrapperCallWrapper::IsCalling(wrapperCallBits, XmlEndElementBit))
	{
		WrapperCallWrapper ctx(wrapperCallBits, XmlEndElementBit);
		XmlSaxContext* mgsaxcontext = NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlSaxContext(saxContext, true);
		return GetWrapper()->XmlEndElement(mgsaxcontext, uri,  name, qname);
	}
	else
	{
		return FdoXmlSaxHandler::XmlEndElement(saxContext, uri, name, qname);
	}
}

System::Void VirtualSaxHandler::XmlCharacters(FdoXmlSaxContext* saxContext, FdoString* chars)
{
	if (!WrapperCallWrapper::IsCalling(wrapperCallBits, XmlCharactersBit))
	{
		WrapperCallWrapper ctx(wrapperCallBits, XmlCharactersBit);
		XmlSaxContext* mgsaxcontext = NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateXmlSaxContext(saxContext, true);
		GetWrapper()->XmlCharacters(mgsaxcontext, chars);
	}
	else
	{
		FdoXmlSaxHandler::XmlCharacters(saxContext, chars);
	}
}

