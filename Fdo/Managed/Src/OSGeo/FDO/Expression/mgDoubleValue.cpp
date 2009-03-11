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
#include <Fdo\Expression\DoubleValue.h>
#include <Fdo\Schema\DataType.h>

#include "FDO\Expression\mgDoubleValue.h"
#include "FDO\Schema\mgDataType.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::DoubleValue::DoubleValue(IntPtr unmanaged, Boolean autoDelete) : DataValue(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_EXPRESSION::DoubleValue::DoubleValue() : DataValue(IntPtr::Zero, false)
{
    EXCEPTION_HANDLER(Attach(IntPtr(FdoDoubleValue::Create()), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DoubleValue::DoubleValue(System::Double value) : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDoubleValue::Create(value)), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DoubleValue::operator System::Double( NAMESPACE_OSGEO_FDO_EXPRESSION::DoubleValue^ value )
{
	return (value->GetImpObj())->operator FdoDouble();
}

FdoDoubleValue* NAMESPACE_OSGEO_FDO_EXPRESSION::DoubleValue::GetImpObj()
{
	return static_cast<FdoDoubleValue*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_EXPRESSION::DoubleValue::DataType::get()
{
	FdoDataType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unobj);
}

System::Double NAMESPACE_OSGEO_FDO_EXPRESSION::DoubleValue::Double::get()
{
	double unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDouble())
	return unobj;
}

 System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::DoubleValue::Double::set(System::Double value)
 {
	 EXCEPTION_HANDLER(GetImpObj()->SetDouble(value))
 }

 System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::DoubleValue::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor)
 {
	 EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp^>(processor))->GetImpObj()))
 }

 System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::DoubleValue::ToString()
 {
	 FdoString* unstr;
	 EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return CHECK_STRING(unstr);
 }
