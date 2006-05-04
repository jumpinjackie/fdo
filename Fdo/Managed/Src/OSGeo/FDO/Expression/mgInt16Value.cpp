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
#include <Fdo\Expression\Int16Value.h>
#include <Fdo\Schema\DataType.h>

#include "FDO\Expression\mgInt16Value.h"
#include "FDO\Schema\mgDataType.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::Int16Value::Int16Value(IntPtr unmanaged, Boolean autoDelete) : DataValue(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_EXPRESSION::Int16Value::Int16Value() : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoInt16Value::Create(), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Int16Value::Int16Value(System::Int16 value) : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoInt16Value::Create(value), true))
}

FdoInt16Value* NAMESPACE_OSGEO_FDO_EXPRESSION::Int16Value::GetImpObj()
{
	return static_cast<FdoInt16Value*>(__super::UnmanagedObject.ToPointer());
}

System::Int16 NAMESPACE_OSGEO_FDO_EXPRESSION::Int16Value::op_Explicit( NAMESPACE_OSGEO_FDO_EXPRESSION::Int16Value* value )
{
	return (value->GetImpObj())->operator FdoInt16();
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_EXPRESSION::Int16Value::get_DataType()
{
	FdoDataType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unobj);
}

System::Int16 NAMESPACE_OSGEO_FDO_EXPRESSION::Int16Value::get_Int16()
{
	FdoInt16 unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetInt16())
	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::Int16Value::set_Int16(System::Int16 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetInt16(value))
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::Int16Value::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor* processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp*>(processor))->GetImpObj()))
}

System::String* NAMESPACE_OSGEO_FDO_EXPRESSION::Int16Value::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return unstr;
}
