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

#include "FDO\Expression\mgDataValue.h"
#include "FDO\Schema\mgDataType.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue::DataValue(IntPtr unmanaged, Boolean autoDelete) : LiteralValue(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue::DataValue(NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType) : LiteralValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDataValue::Create(static_cast<FdoDataType>(dataType)), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue::DataValue(System::Boolean value) : LiteralValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDataValue::Create(value), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue::DataValue(System::Byte value) : LiteralValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDataValue::Create(value), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue::DataValue(System::DateTime value) : LiteralValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDataValue::Create(SystemDateToFdoDateTime(value)), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue::DataValue(System::Double value, NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType) : LiteralValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDataValue::Create(value, static_cast<FdoDataType>(dataType)), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue::DataValue(System::Int16 value) : LiteralValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDataValue::Create(value), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue::DataValue(System::Int32 value) : LiteralValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDataValue::Create(value), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue::DataValue(System::Int64 value) : LiteralValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDataValue::Create(value), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue::DataValue(System::Single value) : LiteralValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDataValue::Create(value), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue::DataValue(System::String* value) : LiteralValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(FdoDataValue::Create(StringToUni(value)), true))
}

FdoDataValue* NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue::GetImpObj()
{
	return static_cast<FdoDataValue*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue::DataValue(System::Byte value [], NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType) : LiteralValue(IntPtr::Zero, false)
{
	// NEED_TEST
	FdoByte __pin* unbuffer = &value[0];
	EXCEPTION_HANDLER(Attach(FdoDataValue::Create(unbuffer, value->Length, static_cast<FdoDataType>(dataType)), true))
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue::get_DataType()
{
	FdoDataType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unobj);
}

System::Boolean NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue::IsNull()
{
	FdoBoolean b;
	EXCEPTION_HANDLER(b = !!GetImpObj()->IsNull())
	return b;
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue::SetNull()
{
	EXCEPTION_HANDLER(GetImpObj()->SetNull())
}
