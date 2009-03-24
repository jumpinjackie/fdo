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
#include <Fdo.h>
#include <Fdo\Schema\DataType.h>

#include "FDO\Expression\mgDateTimeValue.h"
#include "FDO\Schema\mgDataType.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::DateTimeValue::DateTimeValue(IntPtr unmanaged, Boolean autoDelete) : DataValue(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_EXPRESSION::DateTimeValue::DateTimeValue() : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDateTimeValue::Create()), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DateTimeValue::DateTimeValue(System::DateTime value) : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoDateTimeValue::Create(SystemDateToFdoDateTime(value))), true))
}

FdoDateTimeValue* NAMESPACE_OSGEO_FDO_EXPRESSION::DateTimeValue::GetImpObj()
{
	return static_cast<FdoDateTimeValue*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_EXPRESSION::DateTimeValue::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DateTimeValue::operator System::DateTime( NAMESPACE_OSGEO_FDO_EXPRESSION::DateTimeValue^ value )
{
	return FdoDateTimeToDateTime((value->GetImpObj())->operator FdoDateTime());
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_EXPRESSION::DateTimeValue::DataType::get()
{
	FdoDataType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unobj);
}

System::DateTime NAMESPACE_OSGEO_FDO_EXPRESSION::DateTimeValue::DateTime::get()
{
	FdoDateTime unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDateTime())
	return FdoDateTimeToDateTime(unobj);
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::DateTimeValue::DateTime::set(System::DateTime value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetDateTime(SystemDateToFdoDateTime(value)))
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::DateTimeValue::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp^>(processor))->GetImpObj()))
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::DateTimeValue::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return CHECK_STRING(unstr);
}
