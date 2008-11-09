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
	EXCEPTION_HANDLER(Attach(FdoGeometryValue::Create(), true))
}

FdoGeometryValue* NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::GetImpObj()
{
	return static_cast<FdoGeometryValue*>(__super::UnmanagedObject.ToPointer());
}

NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::GeometryValue(System::Byte geometry []) : LiteralValue(IntPtr::Zero, false)
{	
	FdoByteArray* umBuffer = ByteArrayToFdoByteArray(geometry);

	EXCEPTION_HANDLER(Attach(FdoGeometryValue::Create(umBuffer), true))

	umBuffer->Release();
}

System::Boolean NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::IsNull()
{
	FdoBoolean unobj;
	EXCEPTION_HANDLER(unobj = !!GetImpObj()->IsNull())
	return unobj;
}

System::Byte NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::get_Geometry() []
{
	FdoByteArray* unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetGeometry())
	System::Byte mgBuffer __gc[] = FdoByteArrayToByteArray(unobj->GetData(), unobj->GetCount());
	unobj->Release();
	return mgBuffer;
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::set_Geometry(System::Byte value [])
{
	EXCEPTION_HANDLER(GetImpObj()->SetGeometry(ByteArrayToFdoByteArray(value)))
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::SetNull()
{
	EXCEPTION_HANDLER(GetImpObj()->SetNullValue())
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor* processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp*>(processor))->GetImpObj()))
}

System::String* NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return unstr;
}

NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValueType NAMESPACE_OSGEO_FDO_EXPRESSION::GeometryValue::get_LiteralValueType()
{
	FdoLiteralValueType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetLiteralValueType())
	return static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValueType>(unobj);
}
