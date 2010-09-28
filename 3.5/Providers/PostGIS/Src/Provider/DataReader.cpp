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
#include "DataReader.h"
#include "Connection.h"
#include "PgCursor.h"

#include <cassert>

namespace fdo { namespace postgis {

DataReader::DataReader() : Base()
{
	assert(false);
}

DataReader::DataReader(Connection* conn, PgCursor* cursor) : Base(conn, cursor)
{
    FDOLOG_MARKER("DataReader::DataReader");
}

DataReader::~DataReader()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void DataReader::Dispose()
{
    delete this;
}


///////////////////////////////////////////////////////////////////////////////
// FdoIDataReader interface
///////////////////////////////////////////////////////////////////////////////

FdoInt32 DataReader::GetPropertyCount()
{
    return Base::GetPropertyCount();
}

FdoString* DataReader::GetPropertyName(FdoInt32 index)
{
    return Base::GetPropertyName(index);
}

FdoInt32 DataReader::GetPropertyIndex(FdoString* propertyName)
{
    return Base::GetPropertyIndex(propertyName);
}

FdoDataType DataReader::GetDataType(FdoString* propertyName)
{
    return Base::GetDataType(propertyName);
}

FdoPropertyType DataReader::GetPropertyType(FdoString* propertyName)
{
    return Base::GetPropertyType(propertyName);
}

}} // namespace fdo::postgis

