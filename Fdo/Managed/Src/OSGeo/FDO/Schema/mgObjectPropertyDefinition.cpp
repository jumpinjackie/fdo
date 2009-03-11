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
#include <Fdo\Commands\CommandType.h>
#include <Fdo\Schema\ObjectPropertyDefinition.h>

#include "FDO\Schema\mgObjectPropertyDefinition.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Schema\mgClassDefinition.h"
#include "FDO\Schema\mgDataPropertyDefinition.h"

NAMESPACE_OSGEO_FDO_SCHEMA::ObjectPropertyDefinition::ObjectPropertyDefinition() : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoObjectPropertyDefinition::Create()), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::ObjectPropertyDefinition::ObjectPropertyDefinition(System::String^ name, System::String^ description) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoObjectPropertyDefinition::Create(StringToUni(name), StringToUni(description))), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::ObjectPropertyDefinition::ObjectPropertyDefinition(System::String^ name, System::String^ description, System::Boolean system) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoObjectPropertyDefinition::Create(StringToUni(name), StringToUni(description), system)), true))
}

FdoObjectPropertyDefinition* NAMESPACE_OSGEO_FDO_SCHEMA::ObjectPropertyDefinition::GetImpObj()
{
	return static_cast<FdoObjectPropertyDefinition*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::ObjectPropertyDefinition::Class::get()
{
	FdoClassDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetClass())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateClassDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ObjectPropertyDefinition::Class::set(NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetClass((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition^ NAMESPACE_OSGEO_FDO_SCHEMA::ObjectPropertyDefinition::IdentityProperty::get()
{
	FdoDataPropertyDefinition* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetIdentityProperty())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataPropertyDefinition(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ObjectPropertyDefinition::IdentityProperty::set(NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetIdentityProperty((value == nullptr ? nullptr : value->GetImpObj())))
}

NAMESPACE_OSGEO_FDO_SCHEMA::ObjectType NAMESPACE_OSGEO_FDO_SCHEMA::ObjectPropertyDefinition::ObjectType::get()
{
	FdoObjectType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetObjectType())

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::ObjectType>(result);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ObjectPropertyDefinition::ObjectType::set(NAMESPACE_OSGEO_FDO_SCHEMA::ObjectType value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetObjectType(static_cast<FdoObjectType>(value)))
}

NAMESPACE_OSGEO_FDO_SCHEMA::OrderType NAMESPACE_OSGEO_FDO_SCHEMA::ObjectPropertyDefinition::OrderType::get()
{
	FdoOrderType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetOrderType())

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::OrderType>(result);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::ObjectPropertyDefinition::OrderType::set(NAMESPACE_OSGEO_FDO_SCHEMA::OrderType value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetOrderType(static_cast<FdoOrderType>(value)))
}
