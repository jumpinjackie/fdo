/*
* 
* Copyright (C) 2004-2007  Autodesk, Inc.
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
#include <Fdo\Expression\GeometryValue.h>

#include "FDO\Expression\mgGeometryValue.h"
#include "FDO\Expression\mgIExpressionProcessorImp.h"

NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::GeometryValue(IntPtr unmanaged, Boolean autoDelete) : LiteralValue(unmanaged, autoDelete)
{

}

NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::GeometryValue() : LiteralValue(IntPtr::Zero, false)
{
	EXCEPTION_HANDLER(Attach(IntPtr(FdoGeometryValue::Create()), true))
}

FdoGeometryValue* NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::GetImpObj()
{
	return static_cast<FdoGeometryValue*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::GeometryValue(array<System::Byte>^ geometry) : LiteralValue(IntPtr::Zero, false)
{	
    FdoByteArray* arr = nullptr;
    try
    {
        arr = ByteArrayToFdoByteArray(geometry);
	    EXCEPTION_HANDLER(Attach(IntPtr(FdoGeometryValue::Create(arr)), true))
    }
    finally
    {
        if (arr != nullptr)
            arr->Release();
    }
}

System::Boolean NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::IsNull()
{
	System::Boolean unobj;
	EXCEPTION_HANDLER(unobj = !!GetImpObj()->IsNull())
	return unobj;
}

array<System::Byte>^ NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::Geometry::get()
{
    FdoByteArray* arr = nullptr;
    array<System::Byte>^ result;
    try
    {
	    EXCEPTION_HANDLER(arr = GetImpObj()->GetGeometry())
	    result = FdoByteArrayToByteArray(arr->GetData(), arr->GetCount());
    }
    finally
    {
        if (arr != nullptr)
            arr->Release();
    }
	return result;
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::Geometry::set(array<System::Byte>^ value)
{
    FdoByteArray* arr = nullptr;
    try
    {
        arr = ByteArrayToFdoByteArray(value);
    	EXCEPTION_HANDLER(GetImpObj()->SetGeometry(arr))
    }
    finally
    {
        if (arr != nullptr)
            arr->Release();
    }
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::SetNull()
{
	EXCEPTION_HANDLER(GetImpObj()->SetNullValue())
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp^>(processor))->GetImpObj()))
}

System::String^ NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return CHECK_STRING(unstr);
}

NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValueType NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::LiteralValueType::get()
{
	FdoLiteralValueType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetLiteralValueType())
	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValueType>(unobj);
}
