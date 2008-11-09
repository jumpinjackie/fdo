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
#include <Fdo\Expression\Int32Value.h>
#include <Fdo\Schema\DataType.h>

#include "FDO\Expression\mgInt32Value.h"
#include "FDO\Schema\mgDataType.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::Int32Value::Int32Value(IntPtr unmanaged, Boolean autoDelete) : DataValue(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_EXPRESSION::Int32Value::Int32Value() : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoInt32Value::Create(), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::Int32Value::Int32Value(System::Int32 value) : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoInt32Value::Create(value), true))
}

FdoInt32Value* NAMESPACE_OSGEO_FDO_EXPRESSION::Int32Value::GetImpObj()
{
	return static_cast<FdoInt32Value*>(__super::UnmanagedObject.ToPointer());
}

System::Int32 NAMESPACE_OSGEO_FDO_EXPRESSION::Int32Value::op_Explicit( NAMESPACE_OSGEO_FDO_EXPRESSION::Int32Value* value )
{
	return (value->GetImpObj())->operator FdoInt32();
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_EXPRESSION::Int32Value::get_DataType()
{
	FdoDataType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unobj);
}

System::Int32 NAMESPACE_OSGEO_FDO_EXPRESSION::Int32Value::get_Int32()
{
	FdoInt32 unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetInt32())
	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::Int32Value::set_Int32(System::Int32 value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetInt32(value))
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::Int32Value::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor* processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp*>(processor))->GetImpObj()))
}

System::String* NAMESPACE_OSGEO_FDO_EXPRESSION::Int32Value::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return unstr;
}
