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
#include "Common\Xml\mgXmlReader.h"
#include "Common\Xml\mgXmlSaxContext.h"

NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext::XmlSaxContext(XmlReader* reader) : Disposable(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlSaxContext::Create(reader->GetImpObj()), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext::XmlSaxContext(IntPtr unmanaged, Boolean autoDelete) : Disposable(unmanaged, autoDelete)
{
	
}

FdoXmlSaxContext* NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext::GetImpObj()
{
	return static_cast<FdoXmlSaxContext*>(__super::UnmanagedObject.ToPointer());
}

Void NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext::ReleaseUnmanagedObject()
{
	if (get_AutoDelete()) 
        EXCEPTION_HANDLER(GetImpObj()->Release())
	Detach();
}

NAMESPACE_OSGEO_COMMON_XML::XmlReader* NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext::get_Reader()
{ 
    XmlReader* reader;
	EXCEPTION_HANDLER(reader = ObjectFactory::CreateXmlReader(GetImpObj()->GetReader(), true))
	return reader;
}
