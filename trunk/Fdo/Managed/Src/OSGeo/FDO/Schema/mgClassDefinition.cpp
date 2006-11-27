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
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Schema\ClassType.h>
#include <Fdo\Commands\Locking\LockType.h>
#include <Fdo\Commands\CommandType.h>
#include <Fdo\Schema\DataType.h>
#include <Fdo\Schema\PropertyType.h>
#include <Fdo\Schema\UniqueConstraintCollection.h>
#include <Fdo\Schema\ClassDefinition.h>


#include "FDO\Schema\mgClassDefinition.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgPropertyDefinitionCollection.h"
#include "FDO\Schema\mgReadOnlyPropertyDefinitionCollection.h"
#include "FDO\Schema\mgDataPropertyDefinitionCollection.h"
#include "FDO\Schema\mgReadOnlyDataPropertyDefinitionCollection.h"
#include "FDO\Schema\mgClassCapabilities.h"

FdoClassDefinition* NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition::GetImpObj()
{
	return static_cast<FdoClassDefinition*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassType NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition::get_ClassType()
{
	FdoClassType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClassType())

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::ClassType>(result);
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition::get_BaseClass()
{
	FdoClassDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetBaseClass())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateClassDefinition(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition::set_BaseClass(NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetBaseClass((value == NULL ? NULL : value->GetImpObj())))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition::get_IsAbstract()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetIsAbstract())

	return result;	
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition::set_IsAbstract(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetIsAbstract(value))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition::SetBaseProperties(NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetBaseProperties((value == NULL ? NULL : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyPropertyDefinitionCollection* NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition::GetBaseProperties()
{
	FdoReadOnlyPropertyDefinitionCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetBaseProperties())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateReadOnlyPropertyDefinitionCollection(result, true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection* NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition::get_IdentityProperties()
{
	FdoDataPropertyDefinitionCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetIdentityProperties())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataPropertyDefinitionCollection(result, true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyDataPropertyDefinitionCollection* NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition::get_BaseIdentityProperties()
{
	FdoReadOnlyDataPropertyDefinitionCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetBaseIdentityProperties())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateReadOnlyDataPropertyDefinitionCollection(result, true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection* NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition::get_Properties()
{
	FdoPropertyDefinitionCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetProperties())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyDefinitionCollection(result, true);
}

System::String* NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition::get_QualifiedName()
{
	FdoStringP result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetQualifiedName())

	return (FdoString*)result;
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition::get_IsComputed()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetIsComputed())

	return result;	
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition::set_IsComputed(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetIsComputed(value))
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities* NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition::get_Capabilities()
{
	FdoClassCapabilities* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetCapabilities())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateClassCapabilities(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition::set_Capabilities(NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetCapabilities((value == NULL ? NULL : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection* NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition::get_UniqueConstraints()
{
	FdoUniqueConstraintCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetUniqueConstraints())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateUniqueConstraintCollection(result, true);
}
