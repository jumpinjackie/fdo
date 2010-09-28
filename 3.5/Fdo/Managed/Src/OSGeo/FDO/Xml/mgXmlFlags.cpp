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
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlFlags::Create()), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFlags::XmlFlags(System::String^ location) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlFlags::Create(StringToUni(location))), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFlags::XmlFlags(System::String^ location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlFlags::Create(StringToUni(location), static_cast<FdoXmlFlags::ErrorLevel>(errorLevel))), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFlags::XmlFlags(System::String^ location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel, System::Boolean nameAdjust) : NAMESPACE_OSGEO_RUNTIME::Disposable(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlFlags::Create(StringToUni(location), static_cast<FdoXmlFlags::ErrorLevel>(errorLevel), nameAdjust)), true))
}

FdoXmlFlags* NAMESPACE_OSGEO_FDO_XML::XmlFlags::GetImpObj()
{
	return static_cast<FdoXmlFlags*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_XML::XmlFlags::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFlags::Url::set(System::String^ location)
{
    EXCEPTION_HANDLER(GetImpObj()->SetUrl(StringToUni(location)))
}

System::String^ NAMESPACE_OSGEO_FDO_XML::XmlFlags::Url::get()
{
    FdoString* result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetUrl())

    return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFlags::Errorlevel::set(NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel value)
{
    EXCEPTION_HANDLER(GetImpObj()->SetErrorLevel(static_cast<FdoXmlFlags::ErrorLevel>(value)))
}

NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel NAMESPACE_OSGEO_FDO_XML::XmlFlags::Errorlevel::get()
{
    FdoXmlFlags::ErrorLevel result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetErrorLevel())

    return static_cast<NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel>(result);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFlags::NameAdjust::set(System::Boolean nameAdjust)
{
    EXCEPTION_HANDLER(GetImpObj()->SetNameAdjust(nameAdjust))
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFlags::NameAdjust::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetNameAdjust())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFlags::SchemaNameAsPrefix::set(System::Boolean schemaNameAsPrefix)
{
    EXCEPTION_HANDLER(GetImpObj()->SetSchemaNameAsPrefix(schemaNameAsPrefix))
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFlags::SchemaNameAsPrefix::get()
{
   	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetSchemaNameAsPrefix())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFlags::UseGmlId::set(System::Boolean id)
{
    EXCEPTION_HANDLER(GetImpObj()->SetUseGmlId(id))
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFlags::UseGmlId::get()
{
   	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetUseGmlId())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFlags::SchemaMappings::set(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection^ mappings)
{
    EXCEPTION_HANDLER(GetImpObj()->SetSchemaMappings((nullptr == mappings ? nullptr : mappings->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection^ NAMESPACE_OSGEO_FDO_XML::XmlFlags::SchemaMappings::get()
{
    FdoPhysicalSchemaMappingCollection* result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetSchemaMappings())

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePhysicalSchemaMappingCollection(IntPtr(result), true);
}
