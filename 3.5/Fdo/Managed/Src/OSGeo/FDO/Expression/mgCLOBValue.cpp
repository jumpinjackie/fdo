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
#include <Fdo\Expression\CLOBValue.h>
#include <Fdo\Schema\DataType.h>

#include "FDO\Expression\mgCLOBValue.h"
#include "FDO\Schema\mgDataType.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"
#include <CommonUtilities.h>

NAMESPACE_OSGEO_FDO_EXPRESSION::CLOBValue::CLOBValue(IntPtr unmanaged, Boolean autoDelete) : LOBValue(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_EXPRESSION::CLOBValue::CLOBValue() : LOBValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoCLOBValue::Create()), true))
}

NAMESPACE_OSGEO_FDO_EXPRESSION::CLOBValue::CLOBValue(array<System::Byte>^ value) : LOBValue(IntPtr::Zero, false)
{
    FdoByteArray* arr = nullptr;
    try
    {
        arr = ByteArrayToFdoByteArray(value);
	    EXCEPTION_HANDLER(Attach(IntPtr(FdoCLOBValue::Create(arr)), true))
    }
    finally
    {
        if (arr != nullptr)
            arr->Release();
    }
}

FdoCLOBValue* NAMESPACE_OSGEO_FDO_EXPRESSION::CLOBValue::GetImpObj()
{
	return static_cast<FdoCLOBValue*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_EXPRESSION::CLOBValue::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_EXPRESSION::CLOBValue::DataType::get()
{
	FdoDataType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unobj);
}

array<System::Byte>^ NAMESPACE_OSGEO_FDO_EXPRESSION::CLOBValue::Data::get()
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

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::CLOBValue::Data::set(array<System::Byte>^ value)
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

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::CLOBValue::SetNull()
{
	EXCEPTION_HANDLER(GetImpObj()->SetNull())
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::CLOBValue::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp^>(processor))->GetImpObj()))
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::CLOBValue::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return CHECK_STRING(unstr);
}
