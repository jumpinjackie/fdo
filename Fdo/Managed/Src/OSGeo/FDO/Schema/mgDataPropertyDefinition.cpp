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
#include <Fdo\Schema\DataPropertyDefinition.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Schema\DataType.h>
#include <Fdo\Schema\PropertyType.h>
#include <Fdo\Schema\PropertyValueConstraint.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Schema\mgPropertyValueConstraint.h"
#include "FDO\Schema\mgDataPropertyDefinition.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::DataPropertyDefinition() : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDataPropertyDefinition::Create(), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::DataPropertyDefinition(System::String* name, System::String* description) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDataPropertyDefinition::Create(StringToUni(name), StringToUni(description)), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::DataPropertyDefinition(System::String* name, System::String* description, System::Boolean system) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDataPropertyDefinition::Create(StringToUni(name), StringToUni(description), system), true))
}

FdoDataPropertyDefinition* NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::GetImpObj()
{
	return static_cast<FdoDataPropertyDefinition*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::get_PropertyType()
{
	FdoPropertyType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyType())

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType>(result);
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::get_DataType()
{
	FdoDataType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDataType())

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(result);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::set_DataType(NAMESPACE_OSGEO_FDO_SCHEMA::DataType value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDataType(static_cast<FdoDataType>(value)))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::get_ReadOnly()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetReadOnly())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::set_ReadOnly(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetReadOnly(value))
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::get_Length()
{
	FdoInt32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLength())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::set_Length(System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetLength(value))
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::get_Precision()
{
	FdoInt32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetPrecision())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::set_Precision(System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetPrecision(value))
}

System::Int32 NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::get_Scale()
{
	FdoInt32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetScale())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::set_Scale(System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetScale(value))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::get_Nullable()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetNullable())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::set_Nullable(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetNullable(value))
}

System::String* NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::get_DefaultValue()
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDefaultValue())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::set_DefaultValue(System::String* value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDefaultValue(StringToUni(value)))
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::set_IsAutoGenerated(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetIsAutoGenerated(value))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::get_IsAutoGenerated()
{
	FdoBoolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetIsAutoGenerated())

	return result;
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraint* NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::get_ValueConstraint()
{
	FdoPropertyValueConstraint* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetValueConstraint())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreatePropertyValueConstraint(result, true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition::set_ValueConstraint(NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraint* value )
{
	EXCEPTION_HANDLER(GetImpObj()->SetValueConstraint(value->GetImpObj()))
}
