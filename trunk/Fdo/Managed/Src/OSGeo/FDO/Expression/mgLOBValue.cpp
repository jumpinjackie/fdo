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
#include <Fdo\Expression\LOBValue.h>
#include <Fdo\Schema\DataType.h>

#include "FDO\Expression\mgLOBValue.h"
#include "FDO\Schema\mgDataType.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue::LOBValue(IntPtr unmanaged, Boolean autoDelete) : DataValue(unmanaged, autoDelete)
{

}

FdoLOBValue* NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue::GetImpObj()
{
	return static_cast<FdoLOBValue*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue::operator array<System::Byte>^( NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue^ value )
{
	FdoByteArray* arr = (value->GetImpObj())->operator FdoByteArray*();
    array<System::Byte>^ result;
    try
    {
	    result = FdoByteArrayToByteArray(arr->GetData(), arr->GetCount());
    }
    finally
    {
        if (arr != nullptr)
            arr->Release();
    }
	return result;
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue::DataType::get()
{
	FdoDataType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unobj);
}

array<System::Byte>^ NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue::Data::get()
{
    FdoByteArray* arr = nullptr;
    array<System::Byte>^ result;
    try
    {
	    EXCEPTION_HANDLER(arr = GetImpObj()->GetData())
	    result = FdoByteArrayToByteArray(arr->GetData(), arr->GetCount());
    }
    finally
    {
        if (arr != nullptr)
            arr->Release();
    }
	return result;
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue::Data::set(array<System::Byte>^ value)
{
    FdoByteArray* arr = nullptr;
    try
    {
        arr = ByteArrayToFdoByteArray(value);
    	EXCEPTION_HANDLER(GetImpObj()->SetData(arr))
    }
    finally
    {
        if (arr != nullptr)
            arr->Release();
    }
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue::SetNull()
{
	EXCEPTION_HANDLER(GetImpObj()->SetNull())
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp^>(processor))->GetImpObj()))
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return CHECK_STRING(unstr);
}
