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
#include <Fdo\Expression\StringValue.h>
#include <Fdo\Schema\DataType.h>

#include "FDO\Expression\mgStringValue.h"
#include "FDO\Schema\mgDataType.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::StringValue::StringValue(IntPtr unmanaged, Boolean autoDelete) : DataValue(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_EXPRESSION::StringValue::StringValue() : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoStringValue::Create()), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::StringValue::StringValue(System::String^ value) : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoStringValue::Create(StringToUni(value))), true))
}

FdoStringValue* NAMESPACE_OSGEO_FDO_EXPRESSION::StringValue::GetImpObj()
{
	return static_cast<FdoStringValue*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_EXPRESSION::StringValue::operator System::String^( NAMESPACE_OSGEO_FDO_EXPRESSION::StringValue^ value )
{
    FdoString* result = (value->GetImpObj())->operator FdoCharacter*();
	return CHECK_STRING(result);
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_EXPRESSION::StringValue::DataType::get()
{
	FdoDataType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unobj);
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::StringValue::String::get()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->GetString())
	return CHECK_STRING(unstr);
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::StringValue::String::set(System::String^ value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetString(StringToUni(value)))
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::StringValue::SetNull()
{
	EXCEPTION_HANDLER(GetImpObj()->SetNull())
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::StringValue::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp^>(processor))->GetImpObj()))
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::StringValue::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return CHECK_STRING(unstr);
}
