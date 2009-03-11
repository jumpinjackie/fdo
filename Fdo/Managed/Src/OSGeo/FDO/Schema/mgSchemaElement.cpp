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
#include <Fdo\Schema\SchemaElement.h>
#include <Fdo\Commands\CommandType.h>
#include <Fdo\Schema\SchemaElementState.h>

#include "FDO\Schema\mgSchemaElement.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgFeatureSchema.h"
#include "FDO\Schema\mgSchemaAttributeDictionary.h"

FdoSchemaElement* NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement::GetImpObj()
{
	return static_cast<FdoSchemaElement*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement^ NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement::Parent::get()
{
	FdoSchemaElement* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetParent())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateSchemaElement(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^ NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement::FeatureSchema::get()
{
	FdoFeatureSchema* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetFeatureSchema())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateFeatureSchema(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElementState NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement::ElementState::get()
{
	FdoSchemaElementState result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetElementState())

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElementState>(result);
}

NAMESPACE_OSGEO_FDO_SCHEMA::SchemaAttributeDictionary^ NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement::Attributes::get()
{
	FdoSchemaAttributeDictionary* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetAttributes())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateSchemaAttributeDictionary(IntPtr(result), true);
}

System::String^ NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement::Name::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetName())

	return CHECK_STRING(result);
}

System::String^ NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement::QualifiedName::get()
{
	FdoStringP result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetQualifiedName())

	return CHECK_STRING((FdoString*)result);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement::Name::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetName(StringToUni(value)))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement::CanSetName::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->CanSetName())

	return result;	
}

System::String^ NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement::Description::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDescription())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement::Description::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDescription(StringToUni(value)))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement::Delete()
{
	EXCEPTION_HANDLER(GetImpObj()->Delete())
}

