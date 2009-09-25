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
#include <FdoDefaultSqlDataReader.h>

FdoDefaultSqlDataReader::FdoDefaultSqlDataReader()
{
}

FdoDefaultSqlDataReader::~FdoDefaultSqlDataReader()
{
}

FdoBoolean FdoDefaultSqlDataReader::IsNull(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return IsNull(columnName);
}

FdoDataType FdoDefaultSqlDataReader::GetColumnType(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetColumnType(columnName);
}

FdoPropertyType FdoDefaultSqlDataReader::GetPropertyType(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetPropertyType(columnName);
}

FdoBoolean FdoDefaultSqlDataReader::GetBoolean(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetBoolean(columnName);
}

FdoByte FdoDefaultSqlDataReader::GetByte(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetByte(columnName);
}

FdoDateTime FdoDefaultSqlDataReader::GetDateTime(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetDateTime(columnName);
}

FdoDouble FdoDefaultSqlDataReader::GetDouble(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetDouble(columnName);
}

FdoInt16 FdoDefaultSqlDataReader::GetInt16(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetInt16(columnName);
}

FdoInt32 FdoDefaultSqlDataReader::GetInt32(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetInt32(columnName);
}

FdoInt64 FdoDefaultSqlDataReader::GetInt64(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetInt64(columnName);
}

FdoFloat FdoDefaultSqlDataReader::GetSingle(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetSingle(columnName);
}

FdoString* FdoDefaultSqlDataReader::GetString(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetString(columnName);
}

FdoLOBValue* FdoDefaultSqlDataReader::GetLOB(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetLOB(columnName);
}

FdoIStreamReader* FdoDefaultSqlDataReader::GetLOBStreamReader(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetLOBStreamReader(columnName);
}

FdoByteArray* FdoDefaultSqlDataReader::GetGeometry(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetGeometry(columnName);
}
