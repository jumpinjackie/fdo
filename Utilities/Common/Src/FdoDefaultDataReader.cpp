// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "stdafx.h"
#include <FdoDefaultDataReader.h>

FdoDefaultDataReader::FdoDefaultDataReader()
{
}

FdoDefaultDataReader::~FdoDefaultDataReader()
{
}

FdoDataType FdoDefaultDataReader::GetDataType(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetDataType(propertyName);
}

FdoPropertyType FdoDefaultDataReader::GetPropertyType(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetPropertyType(propertyName);
}

FdoBoolean FdoDefaultDataReader::IsNull(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return IsNull(propertyName);
}

FdoBoolean FdoDefaultDataReader::GetBoolean(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetBoolean(propertyName);
}

FdoByte FdoDefaultDataReader::GetByte(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetByte(propertyName);
}

FdoDateTime FdoDefaultDataReader::GetDateTime(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetDateTime(propertyName);
}

FdoDouble FdoDefaultDataReader::GetDouble(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetDouble(propertyName);
}

FdoInt16 FdoDefaultDataReader::GetInt16(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetInt16(propertyName);
}

FdoInt32 FdoDefaultDataReader::GetInt32(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetInt32(propertyName);
}

FdoInt64 FdoDefaultDataReader::GetInt64(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetInt64(propertyName);
}

FdoFloat FdoDefaultDataReader::GetSingle(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetSingle(propertyName);
}

FdoString* FdoDefaultDataReader::GetString(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetString(propertyName);
}

FdoLOBValue* FdoDefaultDataReader::GetLOB(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetLOB(propertyName);
}

FdoIStreamReader* FdoDefaultDataReader::GetLOBStreamReader(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetLOBStreamReader(propertyName);
}

FdoByteArray* FdoDefaultDataReader::GetGeometry(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetGeometry(propertyName);
}

FdoIRaster* FdoDefaultDataReader::GetRaster(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetRaster(propertyName);
}
