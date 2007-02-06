//
// Copyright (C) 2007 Refractions Research, Inc. 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "stdafx.h"

#include "PostGisProvider.h"
#include "SQLDataReader.h"
#include "Connection.h"

#include <cassert>

namespace fdo { namespace postgis {

SQLDataReader::SQLDataReader(Connection* conn) : mConn(conn)
{
}

SQLDataReader::~SQLDataReader()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void SQLDataReader::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoISQLDataReader interface
///////////////////////////////////////////////////////////////////////////////

FdoInt32 SQLDataReader::GetColumnCount()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoString* SQLDataReader::GetColumnName(FdoInt32 index)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoDataType SQLDataReader::GetColumnType(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return FdoDataType();
}

FdoPropertyType SQLDataReader::GetPropertyType(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return FdoPropertyType_DataProperty; // dummy value
}

bool SQLDataReader::GetBoolean(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoByte SQLDataReader::GetByte(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoDateTime SQLDataReader::GetDateTime(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return FdoDateTime();
}

double SQLDataReader::GetDouble(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoInt16 SQLDataReader::GetInt16(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoInt32 SQLDataReader::GetInt32(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoInt64 SQLDataReader::GetInt64(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

float SQLDataReader::GetSingle(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoString* SQLDataReader::GetString(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoLOBValue* SQLDataReader::GetLOB(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoIStreamReader* SQLDataReader::GetLOBStreamReader(wchar_t const* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

bool SQLDataReader::IsNull(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoByteArray* SQLDataReader::GetGeometry(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

bool SQLDataReader::ReadNext()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

void SQLDataReader::Close()
{
    assert(!"NOT IMPLEMENTED");
}

}} // namespace fdo::postgis

