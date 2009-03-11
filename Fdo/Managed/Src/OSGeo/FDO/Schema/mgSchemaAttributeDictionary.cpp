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
#include "FDO\Schema\mgSchemaAttributeDictionary.h"

#include <Fdo\Schema\SchemaAttributeDictionary.h>

FdoSchemaAttributeDictionary* NAMESPACE_OSGEO_FDO_SCHEMA::SchemaAttributeDictionary::GetImpObj()
{
	return static_cast<FdoSchemaAttributeDictionary*>(__super::UnmanagedObject.ToPointer());
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::SchemaAttributeDictionary::Count::get()
{
	System::Int32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCount())

	return result;
}

array<System::String^>^ NAMESPACE_OSGEO_FDO_SCHEMA::SchemaAttributeDictionary::AttributeNames::get()
{
	// NEED_TEST
	FdoString** result;
	FdoInt32 len;

	EXCEPTION_HANDLER(result = GetImpObj()->GetAttributeNames(len))

	return FdoStringArrayToStringArray(result, len);
}

System::String^ NAMESPACE_OSGEO_FDO_SCHEMA::SchemaAttributeDictionary::GetAttributeValue(System::String^ name)
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetAttributeValue(StringToUni(name)))

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::SchemaAttributeDictionary::SetAttributeValue(System::String^ name, System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetAttributeValue(StringToUni(name), StringToUni(value)))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::SchemaAttributeDictionary::ContainsAttribute(System::String^ name)
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->ContainsAttribute(StringToUni(name)))

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::SchemaAttributeDictionary::Add(System::String^ name, System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->Add(StringToUni(name), StringToUni(value)))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::SchemaAttributeDictionary::Remove(System::String^ name)
{
	EXCEPTION_HANDLER(GetImpObj()->Remove(StringToUni(name)))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::SchemaAttributeDictionary::Clear()
{
	EXCEPTION_HANDLER(GetImpObj()->Clear())
}
