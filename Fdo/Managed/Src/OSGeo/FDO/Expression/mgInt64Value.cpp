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
#include <Fdo\Expression\Int64Value.h>
#include <Fdo\Schema\DataType.h>

#include "FDO\Expression\mgInt64Value.h"
#include "FDO\Schema\mgDataType.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::Int64Value::Int64Value(IntPtr unmanaged, Boolean autoDelete) : DataValue(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_EXPRESSION::Int64Value::Int64Value() : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoInt64Value::Create()), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Int64Value::Int64Value(System::Int64 value) : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoInt64Value::Create(value)), true))
}

FdoInt64Value* NAMESPACE_OSGEO_FDO_EXPRESSION::Int64Value::GetImpObj()
{
	return static_cast<FdoInt64Value*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_EXPRESSION::Int64Value::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Int64Value::operator System::Int64( NAMESPACE_OSGEO_FDO_EXPRESSION::Int64Value^ value )
{
	return (value->GetImpObj())->operator FdoInt64();
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_EXPRESSION::Int64Value::DataType::get()
{
	FdoDataType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unobj);
}

System::Int64 NAMESPACE_OSGEO_FDO_EXPRESSION::Int64Value::Int64::get()
{
	System::Int64 unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetInt64())
	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::Int64Value::Int64::set(System::Int64 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetInt64(value))
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::Int64Value::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp^>(processor))->GetImpObj()))
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::Int64Value::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return CHECK_STRING(unstr);
}
