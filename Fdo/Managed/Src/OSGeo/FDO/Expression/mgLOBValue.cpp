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
	return static_cast<FdoLOBValue*>(__super::UnmanagedObject.ToPointer());
}

System::Byte NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue::op_Explicit( NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue* value ) []
{
	FdoByteArray* unobj = (value->GetImpObj())->operator FdoByteArray*();
	System::Byte buffer __gc[] = FdoByteArrayToByteArray(unobj->GetData(), unobj->GetCount());
	unobj->Release();
	return buffer;
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue::get_DataType()
{
	FdoDataType unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetDataType())
	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(unobj);
}

System::Byte NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue::get_Data() []
{
	FdoByteArray* unobj;
	EXCEPTION_HANDLER(unobj = GetImpObj()->GetData())
	System::Byte mgBuffer __gc[] = FdoByteArrayToByteArray(unobj->GetData(), unobj->GetCount());
	unobj->Release();
	return mgBuffer;
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue::set_Data(System::Byte value [])
{
	EXCEPTION_HANDLER(GetImpObj()->SetData(ByteArrayToFdoByteArray(value)))
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue::SetNull()
{
	EXCEPTION_HANDLER(GetImpObj()->SetNull())
}

System::Void NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue::Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor* processor)
{
	EXCEPTION_HANDLER(GetImpObj()->Process((static_cast<NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessorImp*>(processor))->GetImpObj()))
}

System::String* NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue::ToString()
{
	FdoString* unstr;
	EXCEPTION_HANDLER(unstr = GetImpObj()->ToString())
	return unstr;
}
