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
// The following header files sequence is very important
#include <Fdo\Schema\PropertyType.h>
#include <Fdo\Expression\LOBValue.h>
#include <Fdo\Commands\SQL\ISQLDataReader.h>
#include <Fdo\Commands\CommandType.h>

#include "FDO\Commands\SQL\mgISQLDataReaderImp.h"
#include "FDO\mgObjectFactory.h"
#include "FDO\Expression\mgLOBValue.h"

inline FdoISQLDataReader* NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetImpObj()
{
	return static_cast<FdoISQLDataReader*>(UnmanagedObject.ToPointer());
}

IntPtr NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetDisposableObject()
{
    return IntPtr(static_cast<FdoIDisposable*>(GetImpObj()));
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetColumnCount()
{
	FdoInt32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetColumnCount())

	return result;
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetColumnName(System::Int32 index)
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetColumnName(index))

	return CHECK_STRING(result);
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetColumnIndex(System::String^ name)
{
    System::Int32 result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetColumnIndex(StringToUni(name)))

    return result;
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetColumnType(System::String^ name)
{
	FdoDataType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetColumnType(StringToUni(name)))

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(result);
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetPropertyType(System::String^ name)
{
	FdoPropertyType result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyType(StringToUni(name)))

	return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType>(result);
}
 
System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetBoolean(System::String^ name)
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->GetBoolean(StringToUni(name)))

	return result;
}

System::Byte NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetByte(System::String^ name)
{
	System::Byte result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetByte(StringToUni(name)))

	return result;
}

System::DateTime NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetDateTime(System::String^ name)
{
	FdoDateTime result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDateTime(StringToUni(name)))

	return FdoDateTimeToDateTime(result);
}

System::Double NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetDouble(System::String^ name)
{
	System::Double result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetDouble(StringToUni(name)))

	return result;
}

System::Int16 NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetInt16(System::String^ name)
{
	System::Int16 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetInt16(StringToUni(name)))

	return result;
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetInt32(System::String^ name)
{
	System::Int32 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetInt32(StringToUni(name)))

	return result;
}

System::Int64 NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetInt64(System::String^ name)
{
	System::Int64 result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetInt64(StringToUni(name)))

	return result;
}

System::Single NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetSingle(System::String^ name)
{
	System::Single result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetSingle(StringToUni(name)))

	return result;
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetString(System::String^ name)
{
	FdoString* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetString(StringToUni(name)))

	return CHECK_STRING(result);
}
 
NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue^ NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetLOB(System::String^ name)
{
	FdoLOBValue* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLOB(StringToUni(name)))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateLOBValue(IntPtr(result), true);
}

NAMESPACE_OSGEO_COMMON::IStreamReader^ NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetLOBStreamReader(System::String^ name)
{
	FdoIStreamReader* result;

	EXCEPTION_HANDLER(result = GetImpObj()->GetLOBStreamReader(StringToUni(name)))
	
    return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateIStreamReader(IntPtr(result), true);
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::IsNull(System::String^ name)
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->IsNull(StringToUni(name)))

	return result;
}

array<System::Byte>^ NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetGeometry(System::String^ name)
{
    array<System::Byte>^ result;
	FdoByteArray* arr;
    try
    {
	    EXCEPTION_HANDLER(arr = GetImpObj()->GetGeometry(StringToUni(name)))
	    result = FdoByteArrayToByteArray(arr->GetData(), arr->GetCount());
    }
    finally
    {
        if (arr != nullptr)
	        arr->Release();
    }
	return result;
}

NAMESPACE_OSGEO_FDO_SCHEMA::DataType NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetColumnType(System::Int32 index)
{
    FdoDataType result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetColumnType(index))

    return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::DataType>(result);
}

NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetPropertyType(System::Int32 index)
{
    FdoPropertyType result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetPropertyType(index))

    return static_cast<NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType>(result);
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetBoolean(System::Int32 index)
{
    System::Boolean result;

    EXCEPTION_HANDLER(result = !!GetImpObj()->GetBoolean(index))

    return result;
}

System::Byte NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetByte(System::Int32 index)
{
    System::Byte result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetByte(index))

    return result;
}

System::DateTime NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetDateTime(System::Int32 index)
{
    FdoDateTime result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetDateTime(index))

    return FdoDateTimeToDateTime(result);
}

System::Double NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetDouble(System::Int32 index)
{
    System::Double result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetDouble(index))

    return result;
}

System::Int16 NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetInt16(System::Int32 index)
{
    System::Int16 result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetInt16(index))

    return result;
}

System::Int32 NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetInt32(System::Int32 index)
{
    System::Int32 result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetInt32(index))

    return result;
}

System::Int64 NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetInt64(System::Int32 index)
{
    System::Int64 result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetInt64(index))

    return result;
}

System::Single NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetSingle(System::Int32 index)
{
    System::Single result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetSingle(index))

    return result;
}

System::String^ NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetString(System::Int32 index)
{
    FdoString* result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetString(index))

    return CHECK_STRING(result);
}

NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue^ NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetLOB(System::Int32 index)
{
    FdoLOBValue* result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetLOB(index))

    return NAMESPACE_OSGEO_FDO::ObjectFactory::CreateLOBValue(IntPtr(result), true);
}

NAMESPACE_OSGEO_COMMON::IStreamReader^ NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetLOBStreamReader(System::Int32 index)
{
    FdoIStreamReader* result;

    EXCEPTION_HANDLER(result = GetImpObj()->GetLOBStreamReader(index))

    return NAMESPACE_OSGEO_COMMON::ObjectFactory::CreateIStreamReader(IntPtr(result), true);
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::IsNull(System::Int32 index)
{
    System::Boolean result;

    EXCEPTION_HANDLER(result = !!GetImpObj()->IsNull(index))

    return result;
}

array<System::Byte>^ NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::GetGeometry(System::Int32 index)
{
    array<System::Byte>^ result;
    FdoByteArray* arr;
    try
    {
        EXCEPTION_HANDLER(arr = GetImpObj()->GetGeometry(index))
            result = FdoByteArrayToByteArray(arr->GetData(), arr->GetCount());
    }
    finally
    {
        if (arr != nullptr)
            arr->Release();
    }
    return result;
}

System::Boolean NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::ReadNext()
{
	System::Boolean result;

	EXCEPTION_HANDLER(result = !!GetImpObj()->ReadNext())

	return result;
}

System::Void NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReaderImp::Close()
{
	EXCEPTION_HANDLER(GetImpObj()->Close())
}
