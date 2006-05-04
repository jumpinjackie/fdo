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
#include "Common\mgObjectFactory.h"
#include "Common\Xml\mgXmlSaxHandler.h"
#include "Common\Xml\mgXmlSaxContext.h"
#include "Common\Xml\mgXmlAttributeCollection.h"
#include "Common\Xml\VirtualSaxHandler.h"

NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler::XmlSaxHandler(System::IntPtr unmanaged, System::Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler::XmlSaxHandler() : Disposable(new VirtualSaxHandler(), true)
{
	static_cast<VirtualSaxHandler*>(GetImpObj())->SetWrapper(this);
}

FdoXmlSaxHandler* NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler::GetImpObj()
{
	return static_cast<FdoXmlSaxHandler*>(__super::UnmanagedObject.ToPointer());
}

Void NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler::Dispose(System::Boolean disposing)
{
	if (disposing)
	{
		// Add your own code here
	}

	if (!Disposed)
	{
		delete GetImpObj();
		Detach();
	}
}

NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler* NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler::XmlStartDocument(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context)
{
	FdoXmlSaxHandler* result;

	EXCEPTION_HANDLER(result= GetImpObj()->XmlStartDocument(context->GetImpObj()))

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateIXmlSaxHandler(result, true);
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler::XmlEndDocument(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context)
{
	EXCEPTION_HANDLER(GetImpObj()->XmlEndDocument(context->GetImpObj()))
}

NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler* NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler::XmlStartElement(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context, System::String* resourceLocation, System::String* name, System::String* qualifiedName, NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection* attributes)
{
	FdoXmlSaxHandler* result;

	EXCEPTION_HANDLER(result = GetImpObj()->XmlStartElement(context->GetImpObj(), StringToUni(resourceLocation), StringToUni(name), StringToUni(qualifiedName), attributes->GetImpObj()))

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateIXmlSaxHandler(result, true);
}

System::Boolean NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler::XmlEndElement(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context, System::String* resourceLocation, System::String* name, System::String* qualifiedName)
{
	FdoBoolean rv;

	EXCEPTION_HANDLER(rv = !!GetImpObj()->XmlEndElement(context == NULL ? NULL : context->GetImpObj(), StringToUni(resourceLocation), StringToUni(name), StringToUni(qualifiedName)))

	return rv;
}

System::Void NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler::XmlCharacters(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context, System::String* characters)
{
	EXCEPTION_HANDLER(GetImpObj()->XmlCharacters(context->GetImpObj(), StringToUni(characters)))
}
