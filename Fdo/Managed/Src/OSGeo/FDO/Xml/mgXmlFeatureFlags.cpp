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
	EXCEPTION_HANDLER(Attach(FdoXmlFeatureFlags::Create(), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::XmlFeatureFlags(System::String* location) : NAMESPACE_OSGEO_FDO_XML::XmlFlags(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlFeatureFlags::Create(StringToUni(location)), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::XmlFeatureFlags(System::String* location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel) : NAMESPACE_OSGEO_FDO_XML::XmlFlags(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlFeatureFlags::Create(StringToUni(location), static_cast<FdoXmlFlags::ErrorLevel>(errorLevel)), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::XmlFeatureFlags(System::String* location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel, System::Boolean nameAdjust) : NAMESPACE_OSGEO_FDO_XML::XmlFlags(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlFeatureFlags::Create(StringToUni(location), static_cast<FdoXmlFlags::ErrorLevel>(errorLevel), nameAdjust), true))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::XmlFeatureFlags(System::String* location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel, System::Boolean nameAdjust, NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::ConflictOption conflictOption) : NAMESPACE_OSGEO_FDO_XML::XmlFlags(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoXmlFeatureFlags::Create(StringToUni(location), static_cast<FdoXmlFlags::ErrorLevel>(errorLevel), nameAdjust, static_cast<FdoXmlFeatureFlags::ConflictOption>(conflictOption)), true))
}

FdoXmlFeatureFlags* NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::GetImpObj()
{
    return static_cast<FdoXmlFeatureFlags*>(__super::UnmanagedObject.ToPointer());
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::Dispose(System::Boolean disposing)
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

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::set_Conflictoption(NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::ConflictOption value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetConflictOption(static_cast<FdoXmlFeatureFlags::ConflictOption>(value)))
}

NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::ConflictOption NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::get_Conflictoption()
{
	FdoXmlFeatureFlags::ConflictOption result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetConflictOption())

	return static_cast<NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::ConflictOption>(result);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::set_WriteCollection(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetWriteCollection(value))
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::get_WriteCollection()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetWriteCollection())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::set_WriteMember(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetWriteMember(value))
}

System::Boolean NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::get_WriteMember()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetWriteMember())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::set_CollectionUri(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetCollectionUri(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::get_CollectionUri()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCollectionUri())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::set_CollectionName(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetCollectionName(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::get_CollectionName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCollectionName())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::set_MemberUri(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetMemberUri(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::get_MemberUri()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetMemberUri())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::set_MemberName(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetMemberName(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::get_MemberName()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetMemberName())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::set_GmlIdPrefix(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetGmlIdPrefix(StringToUni(value)))
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::get_GmlIdPrefix()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetGmlIdPrefix())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::set_SchemaLocation(System::String* schemaNamespace, System::String* schemaLocation)
{
	EXCEPTION_HANDLER(GetImpObj()->SetSchemaLocation(StringToUni(schemaNamespace), StringToUni(schemaLocation)))
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::get_SchemaLocation(System::String* schemaNamespace)
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetSchemaLocation(StringToUni(schemaNamespace)))

	return result;
}

NAMESPACE_OSGEO_COMMON::StringCollection* NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::get_Namespaces()
{
	FdoStringCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetNamespaces())

	return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateStringCollection(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::set_DefaultNamespace(System::String* defaultNamespace)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDefaultNamespace(StringToUni(defaultNamespace)))
}

System::String* NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags::get_DefaultNamespace()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDefaultNamespace())

	return result;
}
