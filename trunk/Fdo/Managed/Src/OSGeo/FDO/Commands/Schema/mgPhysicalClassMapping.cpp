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
#include <Fdo\Commands\Schema\PhysicalClassMapping.h>

#include "FDO\Commands\Schema\mgPhysicalClassMapping.h"
#include "FDO\Xml\mgXmlFlags.h"

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping::PhysicalClassMapping(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(unmanaged, autoDelete)
{

}

FdoPhysicalClassMapping* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping::GetImpObj()
{
    return static_cast<FdoPhysicalClassMapping*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping::InitFromXml(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* pContext, NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection* attrs)
{
	EXCEPTION_HANDLER(GetImpObj()->InitFromXml(static_cast<FdoXmlSaxContext*>(pContext->UnmanagedObject.ToPointer()), static_cast<FdoXmlAttributeCollection*>(attrs->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping::WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* xmlWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags)
{
	EXCEPTION_HANDLER(GetImpObj()->_writeXml(static_cast<FdoXmlWriter*>(xmlWriter->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

