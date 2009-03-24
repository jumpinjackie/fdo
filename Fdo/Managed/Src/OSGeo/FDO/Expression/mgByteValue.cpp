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
#include <Fdo\Expression\ByteValue.h>
#include <Fdo\Schema\DataType.h>

#include "FDO\Expression\mgByteValue.h"
#include "FDO\Schema\mgDataType.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::ByteValue::ByteValue(IntPtr unmanaged, Boolean autoDelete) : DataValue(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_EXPRESSION::ByteValue::ByteValue() : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoByteValue::Create()), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::ByteValue::ByteValue(System::Byte value) : DataValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoByteValue::Create(value)), true))
}

FdoByteValue* NAMESPACE_OSGEO_FDO_EXPRESSION::ByteValue::GetImpObj()
{
	return static_cast<FdoByteValue*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_EXPRESSION::ByteValue::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_EXPRESSION::ByteValue::operator System::Byte( NAMESPACE_OSGEO_FDO_EXPRESSION::ByteValue^ value )
{
	return (value->GetImpObj())->operator FdoByte();
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_EXPRESSION::ByteValue::DataType::get()
{
	FdoDataType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unobj);
}

System::Byte NAMESPACE_OSGEO_FDO_EXPRESSION::ByteValue::Byte::get()
{
	System::Byte b;
	EXCEPTION_HANDLER(b = GetImpObj()->GetByte())
	return b;
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::ByteValue::Byte::set(System::Byte value)
{
	EXCEPTION_HANDLER(GetImpObj()->SetByte(value))
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::ByteValue::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp^>(processor))->GetImpObj()))
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::ByteValue::ToString()
{
    FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return CHECK_STRING(unstr);
}
