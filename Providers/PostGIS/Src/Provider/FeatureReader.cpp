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
#include "FeatureReader.h"
#include "Connection.h"

#include <cassert>

namespace fdo { namespace postgis {

FeatureReader::FeatureReader(Connection* conn) : Base(conn)
{
    assert(!"TODO - Add missing parameters");
}

FeatureReader::~FeatureReader()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void FeatureReader::Dispose()
{
    delete this;
}


///////////////////////////////////////////////////////////////////////////////
// FdoIFeatureReader interface
///////////////////////////////////////////////////////////////////////////////

FdoClassDefinition* FeatureReader::GetClassDefinition()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoInt32 FeatureReader::GetDepth()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

const FdoByte* FeatureReader::GetGeometry(FdoString* propertyName, FdoInt32* count)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoByteArray* FeatureReader::GetGeometry(FdoString* propertyName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoIFeatureReader* FeatureReader::GetFeatureObject(FdoString* propertyName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

}} // namespace fdo::postgis

