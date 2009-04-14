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
#include "Common\Xml\mgXmlCharDataHandler.h"

NAMESPACE_OSGEO_COMMON_XML::XmlCharDataHandler::XmlCharDataHandler() : XmlSaxHandler(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlCharDataHandler::Create(), true))
}

NAMESPACE_OSGEO_COMMON_XML::XmlCharDataHandler::XmlCharDataHandler(IntPtr unmanaged, Boolean autoDelete) : XmlSaxHandler(unmanaged, autoDelete)
{

}

FdoXmlCharDataHandler* NAMESPACE_OSGEO_COMMON_XML::XmlCharDataHandler::GetImpObj()
{
	return static_cast<FdoXmlCharDataHandler*>(__super::UnmanagedObject.ToPointer());
}

String* NAMESPACE_OSGEO_COMMON_XML::XmlCharDataHandler::get_RetString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->GetString())
	return unstr;
}
