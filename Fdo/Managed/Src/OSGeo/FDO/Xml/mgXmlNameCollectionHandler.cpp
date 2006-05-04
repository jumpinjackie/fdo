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
#include <Fdo\Xml\Context.h>
#include <Fdo\Xml\NameCollectionHandler.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Xml\mgXmlNameCollectionHandler.h"
#include "FDO\mgObjectFactory.h"

System::Void NAMESPACE_OSGEO_FDO_XML::XmlNameCollectionHandler::Dispose(System::Boolean disposing)
{
	if (disposing)
	{
		// Add your own code here
	}

	if (!Disposed)
	{
		EXCEPTION_HANDLER(GetImpObj()->Release())
		Detach();
	}
}

NAMESPACE_OSGEO_FDO_XML::XmlNameCollectionHandler::XmlNameCollectionHandler() : NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlNameCollectionHandler::Create(), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlNameCollectionHandler::XmlNameCollectionHandler(NAMESPACE_OSGEO_COMMON::StringCollection* names) : NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlNameCollectionHandler::Create(static_cast<FdoStringCollection*>(names->UnmanagedObject.ToPointer())), true))
}

FdoXmlNameCollectionHandler* NAMESPACE_OSGEO_FDO_XML::XmlNameCollectionHandler::GetImpObj()
{
    return static_cast<FdoXmlNameCollectionHandler*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_COMMON::StringCollection* NAMESPACE_OSGEO_FDO_XML::XmlNameCollectionHandler::GetNames()
{
	FdoStringCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetNames())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateStringCollection(result, true);
}
