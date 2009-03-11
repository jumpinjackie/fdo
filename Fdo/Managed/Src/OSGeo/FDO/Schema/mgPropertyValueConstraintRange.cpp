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
#include <Fdo\Schema\ClassType.h>
#include <Fdo\Schema\SchemaElementState.h>
#include <Fdo\Schema\DataType.h>
#include <Fdo\Schema\PropertyType.h>
#include <Fdo\Commands\CommandType.h>
#include <Fdo\Schema\PropertyValueConstraintType.h>
#include <Fdo\Schema\PropertyValueConstraintRange.h>
#include <Fdo\Expression\DataValue.h>

#include "FDO\Schema\mgPropertyValueConstraint.h"
#include <Fdo\Expression\mgDataValue.h>
#include "FDO\Schema\mgPropertyValueConstraintRange.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintRange::PropertyValueConstraintRange(IntPtr unmanaged, Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraint(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintRange::PropertyValueConstraintRange() : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraint(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoPropertyValueConstraintRange::Create()), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintRange::PropertyValueConstraintRange(NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue^ minValue, NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue^ maxValue )  : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraint(System::IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoPropertyValueConstraintRange::Create(static_cast<FdoDataValue*>(minValue->UnmanagedObject.ToPointer()), static_cast<FdoDataValue*>(maxValue->UnmanagedObject.ToPointer()))), true))
}

FdoPropertyValueConstraintRange* NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintRange::GetImpObj()
{
    return static_cast<FdoPropertyValueConstraintRange*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue^ NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintRange::MinValue::get()
{
	FdoDataValue* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetMinValue())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataValue(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintRange::MinValue::set(NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetMinValue(static_cast<FdoDataValue*>(value->UnmanagedObject.ToPointer())))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintRange::MinInclusive::get()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetMinInclusive())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintRange::MinInclusive::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetMinInclusive(value))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue^ NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintRange::MaxValue::get()
{
	FdoDataValue* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetMaxValue())

	return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateDataValue(IntPtr(result), true);
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintRange::MaxValue::set(NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetMaxValue(static_cast<FdoDataValue*>(value->UnmanagedObject.ToPointer())))
}

System::Boolean NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintRange::MaxInclusive::get()
{
	bool result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetMaxInclusive())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraintRange::MaxInclusive::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetMaxInclusive(value))
}
