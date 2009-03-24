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
#include <Fdo\Schema\AssociationPropertyDefinition.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Schema\PropertyType.h>
#include <Fdo\Schema\DeleteRule.h>
#include <Fdo\Schema\ClassType.h>
#include <Fdo\Commands\CommandType.h>


#include "FDO\Schema\mgAssociationPropertyDefinition.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgClassDefinition.h"
#include "FDO\Schema\mgDataPropertyDefinitionCollection.h"

NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::AssociationPropertyDefinition() : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoAssociationPropertyDefinition::Create()), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::AssociationPropertyDefinition(System::String^ name, System::String^ description) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoAssociationPropertyDefinition::Create(StringToUni(name), StringToUni(description))), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::AssociationPropertyDefinition(System::String^ name, System::String^ description, System::Boolean system) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoAssociationPropertyDefinition::Create(StringToUni(name), StringToUni(description), system)), true))
}

FdoAssociationPropertyDefinition* NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::GetImpObj()
{
	return static_cast<FdoAssociationPropertyDefinition*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::PropertyType::get()
{
	FdoPropertyType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyType())

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType>(result);
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::AssociatedClass::get()
{
	FdoClassDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetAssociatedClass())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateClassDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::AssociatedClass::set(NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetAssociatedClass((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection^ NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::IdentityProperties::get()
{
	FdoDataPropertyDefinitionCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetIdentityProperties())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataPropertyDefinitionCollection(IntPtr(result), true);
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection^ NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::ReverseIdentityProperties::get()
{
	FdoDataPropertyDefinitionCollection* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetReverseIdentityProperties())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataPropertyDefinitionCollection(IntPtr(result), true);
}

System::String^ NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::ReverseName::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetReverseName())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::ReverseName::set(System::String^ name)
{
	EXCEPTION_HANDLER(GetImpObj()->SetReverseName(StringToUni(name)))
}

NAMESPACE_OSGEO_FDO_SCHEMA::DeleteRule NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::DeleteRule::get()
{
	FdoDeleteRule result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDeleteRule())

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DeleteRule>(result);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::DeleteRule::set(NAMESPACE_OSGEO_FDO_SCHEMA::DeleteRule value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDeleteRule(static_cast<FdoDeleteRule>(value)))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::LockCascade::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetLockCascade())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::LockCascade::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLockCascade(value))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::IsReadOnly::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetIsReadOnly())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::IsReadOnly::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetIsReadOnly(value))
}

System::String^ NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::Multiplicity::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetMultiplicity())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::Multiplicity::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetMultiplicity(StringToUni(value)))
}

System::String^ NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::ReverseMultiplicity::get()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetReverseMultiplicity())

	return CHECK_STRING(result);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition::ReverseMultiplicity::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetReverseMultiplicity(StringToUni(value)))
}
