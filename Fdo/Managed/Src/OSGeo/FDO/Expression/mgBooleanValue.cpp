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
#include <Fdo\Expression\BooleanValue.h>
#include <Fdo\Schema\DataType.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Expression\mgBooleanValue.h"
#include "FDO\Schema\mgDataType.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"
#include "FDO\mgObjectFactory.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::BooleanValue::BooleanValue(System::IntPtr unmanaged, System::Boolean autoDelete) : DataValue(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_EXPRESSION::BooleanValue::BooleanValue() : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoBooleanValue::Create()), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::BooleanValue::BooleanValue(System::Boolean value) : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoBooleanValue::Create(value)), true))
}

FdoBooleanValue* NAMESPACE_OSGEO_FDO_EXPRESSION::BooleanValue::GetImpObj()
{
	return static_cast<FdoBooleanValue*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_EXPRESSION::BooleanValue::operator System::Boolean ( NAMESPACE_OSGEO_FDO_EXPRESSION::BooleanValue^ value )
{
	return (value->GetImpObj())->operator FdoBoolean();
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_EXPRESSION::BooleanValue::DataType::get()
{
	FdoDataType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unobj);
}

System::Boolean NAMESPACE_OSGEO_FDO_EXPRESSION::BooleanValue::Boolean::get()
{
	System::Boolean b;
	EXCEPTION_HANDLER(b = !!GetImpObj()->GetBoolean())
	return b;
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::BooleanValue::Boolean::set(System::Boolean value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetBoolean(value))
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::BooleanValue::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp^>(processor))->GetImpObj()))
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::BooleanValue::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return CHECK_STRING(unstr);
}