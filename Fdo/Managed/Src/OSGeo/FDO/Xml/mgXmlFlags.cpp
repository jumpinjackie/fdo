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
#include <Fdo\Xml\Flags.h>
#include <Fdo\Xml\FeatureFlags.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Xml\mgXmlFlags.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Commands\Schema\mgPhysicalSchemaMappingCollection.h"

NAMESPACE_OSGEO_FDO_XML::XmlFlags::XmlFlags() : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlFlags::Create(), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFlags::XmlFlags(System::String* location) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlFlags::Create(StringToUni(location)), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFlags::XmlFlags(System::String* location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlFlags::Create(StringToUni(location), static_cast<FdoXmlFlags::ErrorLevel>(errorLevel)), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFlags::XmlFlags(System::String* location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel, System::Boolean nameAdjust) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlFlags::Create(StringToUni(location), static_cast<FdoXmlFlags::ErrorLevel>(errorLevel), nameAdjust), true))
}

FdoXmlFlags* NAMESPACE_OSGEO_FDO_XML::XmlFlags::GetImpObj()
{
	return static_cast<FdoXmlFlags*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFlags::Dispose(System::Boolean disposing)
{
	if (disposing)
	{
		// Add your own code here
	}

	if (!Disposed)
	{
		EXCEPTION_HANDLER(GetImpObj()->Release());
		Detach();
	}
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFlags::set_Url(System::String* location)
{
    EXCEPTION_HANDLER(GetImpObj()->SetUrl(StringToUni(location)))
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlFlags::get_Url()
{
    FdoString* result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetUrl())

    return NULL == result ? static_cast<System::String*>(NULL) : result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFlags::set_Errorlevel(NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel value)
{
    EXCEPTION_HANDLER(GetImpObj()->SetErrorLevel(static_cast<FdoXmlFlags::ErrorLevel>(value)))
}

NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel NAMESPACE_OSGEO_FDO_XML::XmlFlags::get_Errorlevel()
{
    FdoXmlFlags::ErrorLevel result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetErrorLevel())

    return static_cast<NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel>(result);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFlags::set_NameAdjust(System::Boolean nameAdjust)
{
    EXCEPTION_HANDLER(GetImpObj()->SetNameAdjust(nameAdjust))
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFlags::get_NameAdjust()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetNameAdjust())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFlags::set_SchemaNameAsPrefix(System::Boolean schemaNameAsPrefix)
{
    EXCEPTION_HANDLER(GetImpObj()->SetSchemaNameAsPrefix(schemaNameAsPrefix))
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFlags::get_SchemaNameAsPrefix()
{
   	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetSchemaNameAsPrefix())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFlags::set_UseGmlId(System::Boolean id)
{
    EXCEPTION_HANDLER(GetImpObj()->SetUseGmlId(id))
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFlags::get_UseGmlId()
{
   	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetUseGmlId())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFlags::set_SchemaMappings(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection* mappings)
{
    EXCEPTION_HANDLER(GetImpObj()->SetSchemaMappings((NULL == mappings ? NULL : mappings->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection* NAMESPACE_OSGEO_FDO_XML::XmlFlags::get_SchemaMappings()
{
    FdoPhysicalSchemaMappingCollection* result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetSchemaMappings())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePhysicalSchemaMappingCollection(result, true);
}
