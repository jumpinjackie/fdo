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
#include <Fdo\Xml\FeatureFlags.h>
#include <Common\StringCollection.h>

#include "FDO\Xml\mgXmlFeatureFlags.h"

NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::XmlFeatureFlags() : NAMESPACE_OSGEO_FDO_XML::XmlFlags(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlFeatureFlags::Create()), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::XmlFeatureFlags(System::String^ location) : NAMESPACE_OSGEO_FDO_XML::XmlFlags(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlFeatureFlags::Create(StringToUni(location))), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::XmlFeatureFlags(System::String^ location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel) : NAMESPACE_OSGEO_FDO_XML::XmlFlags(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlFeatureFlags::Create(StringToUni(location), static_cast<FdoXmlFlags::ErrorLevel>(errorLevel))), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::XmlFeatureFlags(System::String^ location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel, System::Boolean nameAdjust) : NAMESPACE_OSGEO_FDO_XML::XmlFlags(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlFeatureFlags::Create(StringToUni(location), static_cast<FdoXmlFlags::ErrorLevel>(errorLevel), nameAdjust)), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::XmlFeatureFlags(System::String^ location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel, System::Boolean nameAdjust, NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::ConflictOption conflictOption) : NAMESPACE_OSGEO_FDO_XML::XmlFlags(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoXmlFeatureFlags::Create(StringToUni(location), static_cast<FdoXmlFlags::ErrorLevel>(errorLevel), nameAdjust, static_cast<FdoXmlFeatureFlags::ConflictOption>(conflictOption))), true))
}

FdoXmlFeatureFlags* NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::GetImpObj()
{
    return static_cast<FdoXmlFeatureFlags*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::Conflictoption::set(NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::ConflictOption value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetConflictOption(static_cast<FdoXmlFeatureFlags::ConflictOption>(value)))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::ConflictOption NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::Conflictoption::get()
{
	FdoXmlFeatureFlags::ConflictOption result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetConflictOption())

	return static_cast<NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::ConflictOption>(result);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::WriteCollection::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetWriteCollection(value))
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::WriteCollection::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetWriteCollection())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::WriteMember::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetWriteMember(value))
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::WriteMember::get()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetWriteMember())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::CollectionUri::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetCollectionUri(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::CollectionUri::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCollectionUri())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::CollectionName::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetCollectionName(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::CollectionName::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCollectionName())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::MemberUri::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetMemberUri(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::MemberUri::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetMemberUri())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::MemberName::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetMemberName(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::MemberName::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetMemberName())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::GmlIdPrefix::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGmlIdPrefix(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::GmlIdPrefix::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetGmlIdPrefix())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::SchemaLocation::set(System::String^ schemaNamespace, System::String^ schemaLocation)
{
	EXCEPTION_HANDLER(GetImpObj()->SetSchemaLocation(StringToUni(schemaNamespace), StringToUni(schemaLocation)))
}

System::String^ NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::SchemaLocation::get(System::String^ schemaNamespace)
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetSchemaLocation(StringToUni(schemaNamespace)))

	return CHECK_STRING(result);
}

NAMESPACE_OSGEO_COMMON::StringCollection^ NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::Namespaces::get()
{
	FdoStringCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetNamespaces())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateStringCollection(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::DefaultNamespace::set(System::String^ defaultNamespace)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDefaultNamespace(StringToUni(defaultNamespace)))
}

System::String^ NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::DefaultNamespace::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDefaultNamespace())

	return CHECK_STRING(result);
}
