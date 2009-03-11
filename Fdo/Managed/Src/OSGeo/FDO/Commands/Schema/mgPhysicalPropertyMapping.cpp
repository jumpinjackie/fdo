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
#include <Fdo\Commands\Schema\PhysicalPropertyMapping.h>

#include "FDO\Commands\Schema\mgPhysicalPropertyMapping.h"
#include "FDO\Xml\mgXmlFlags.h"

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalPropertyMapping::PhysicalPropertyMapping(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(unmanaged, autoDelete)
{

}


FdoPhysicalPropertyMapping* NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalPropertyMapping::GetImpObj()
{
    return static_cast<FdoPhysicalPropertyMapping*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalPropertyMapping::InitFromXml(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection^ attributes)
{
	EXCEPTION_HANDLER(GetImpObj()->InitFromXml(static_cast<FdoXmlSaxContext*>(context->UnmanagedObject.ToPointer()), static_cast<FdoXmlAttributeCollection*>(attributes->UnmanagedObject.ToPointer())))
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalPropertyMapping::WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ xmlWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags)
{
	EXCEPTION_HANDLER(GetImpObj()->_writeXml(static_cast<FdoXmlWriter*>(xmlWriter->UnmanagedObject.ToPointer()), flags->GetImpObj()))
}

