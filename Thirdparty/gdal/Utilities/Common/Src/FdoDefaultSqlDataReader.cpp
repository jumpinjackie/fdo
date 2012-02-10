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
    return IsNull((FdoString*)columnName);
}

FdoDataType FdoDefaultSqlDataReader::GetColumnType(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetColumnType((FdoString*)columnName);
}

FdoPropertyType FdoDefaultSqlDataReader::GetPropertyType(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetPropertyType((FdoString*)columnName);
}

FdoBoolean FdoDefaultSqlDataReader::GetBoolean(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetBoolean((FdoString*)columnName);
}

FdoByte FdoDefaultSqlDataReader::GetByte(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetByte((FdoString*)columnName);
}

FdoDateTime FdoDefaultSqlDataReader::GetDateTime(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetDateTime((FdoString*)columnName);
}

FdoDouble FdoDefaultSqlDataReader::GetDouble(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetDouble((FdoString*)columnName);
}

FdoInt16 FdoDefaultSqlDataReader::GetInt16(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetInt16((FdoString*)columnName);
}

FdoInt32 FdoDefaultSqlDataReader::GetInt32(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetInt32((FdoString*)columnName);
}

FdoInt64 FdoDefaultSqlDataReader::GetInt64(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetInt64((FdoString*)columnName);
}

FdoFloat FdoDefaultSqlDataReader::GetSingle(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetSingle((FdoString*)columnName);
}

FdoString* FdoDefaultSqlDataReader::GetString(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetString((FdoString*)columnName);
}

FdoLOBValue* FdoDefaultSqlDataReader::GetLOB(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetLOB((FdoString*)columnName);
}

FdoIStreamReader* FdoDefaultSqlDataReader::GetLOBStreamReader(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetLOBStreamReader((FdoString*)columnName);
}

FdoByteArray* FdoDefaultSqlDataReader::GetGeometry(FdoInt32 index)
{
    FdoStringP columnName = GetColumnName(index);
    return GetGeometry((FdoString*)columnName);
}
