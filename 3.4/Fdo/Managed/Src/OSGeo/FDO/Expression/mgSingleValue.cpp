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
#include <Fdo\Expression\SingleValue.h>
#include <Fdo\Schema\DataType.h>

#include "FDO\Expression\mgSingleValue.h"
#include "FDO\Schema\mgDataType.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::SingleValue::SingleValue(IntPtr unmanaged, Boolean autoDelete) : DataValue(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_EXPRESSION::SingleValue::SingleValue() : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoSingleValue::Create(), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::SingleValue::SingleValue(System::Single value) : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoSingleValue::Create(value), true))
}

FdoSingleValue* NAMESPACE_OSGEO_FDO_EXPRESSION::SingleValue::GetImpObj()
{
	return static_cast<FdoSingleValue*>(__super::UnmanagedObject.ToPointer());
}

System::Single NAMESPACE_OSGEO_FDO_EXPRESSION::SingleValue::op_Explicit( NAMESPACE_OSGEO_FDO_EXPRESSION::SingleValue* value )
{
	return (value->GetImpObj())->operator FdoFloat();
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_EXPRESSION::SingleValue::get_DataType()
{
	FdoDataType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unobj);
}

System::Single NAMESPACE_OSGEO_FDO_EXPRESSION::SingleValue::get_Single()
{
	FdoFloat unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetSingle())
	return unobj;
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::SingleValue::set_Single(System::Single value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetSingle(value))
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::SingleValue::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor* processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp*>(processor))->GetImpObj()))
}

System::String* NAMESPACE_OSGEO_FDO_EXPRESSION::SingleValue::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return unstr;
}
