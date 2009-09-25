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
    return GetDataType((FdoString*)propertyName);
}

FdoPropertyType FdoDefaultDataReader::GetPropertyType(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetPropertyType((FdoString*)propertyName);
}

FdoBoolean FdoDefaultDataReader::IsNull(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return IsNull((FdoString*)propertyName);
}

FdoBoolean FdoDefaultDataReader::GetBoolean(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetBoolean((FdoString*)propertyName);
}

FdoByte FdoDefaultDataReader::GetByte(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetByte((FdoString*)propertyName);
}

FdoDateTime FdoDefaultDataReader::GetDateTime(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetDateTime((FdoString*)propertyName);
}

FdoDouble FdoDefaultDataReader::GetDouble(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetDouble((FdoString*)propertyName);
}

FdoInt16 FdoDefaultDataReader::GetInt16(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetInt16((FdoString*)propertyName);
}

FdoInt32 FdoDefaultDataReader::GetInt32(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetInt32((FdoString*)propertyName);
}

FdoInt64 FdoDefaultDataReader::GetInt64(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetInt64((FdoString*)propertyName);
}

FdoFloat FdoDefaultDataReader::GetSingle(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetSingle((FdoString*)propertyName);
}

FdoString* FdoDefaultDataReader::GetString(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetString((FdoString*)propertyName);
}

FdoLOBValue* FdoDefaultDataReader::GetLOB(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetLOB((FdoString*)propertyName);
}

FdoIStreamReader* FdoDefaultDataReader::GetLOBStreamReader(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetLOBStreamReader((FdoString*)propertyName);
}

FdoByteArray* FdoDefaultDataReader::GetGeometry(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetGeometry((FdoString*)propertyName);
}

FdoIRaster* FdoDefaultDataReader::GetRaster(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetRaster((FdoString*)propertyName);
}
