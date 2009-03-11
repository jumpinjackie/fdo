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
#include "Common\mgDictionary.h"
#include "Common\mgObjectFactory.h"
#include "Common\Xml\mgXmlWriter.h"
#include "Common\Xml\mgXmlCopyHandler.h"
#include "Common\Xml\mgXmlAttributeCollection.h"

NAMESPACE_OSGEO_COMMON_XML::XmlCopyHandler::XmlCopyHandler( XmlWriter^ writer ) : XmlSaxHandler(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlCopyHandler::Create(writer->GetImpObj())), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlCopyHandler::XmlCopyHandler(XmlWriter^ writer, String^ resourceLocation, String^ name,  String^ qualifiedName,  XmlAttributeCollection^ attributes, DictionaryElementCollection^ namespaces) : XmlSaxHandler(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlCopyHandler::Create(writer->GetImpObj(), StringToUni(resourceLocation), StringToUni(name), StringToUni(qualifiedName), attributes->GetImpObj(), namespaces->GetImpObj())), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlCopyHandler::XmlCopyHandler(IntPtr unmanaged, Boolean autoDelete) : XmlSaxHandler(unmanaged, autoDelete)
{

}

FdoXmlCopyHandler* NAMESPACE_OSGEO_COMMON_XML::XmlCopyHandler::GetImpObj()
{
	return static_cast<FdoXmlCopyHandler*>(UnmanagedObject.ToPointer());
}
