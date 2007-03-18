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
#include "PgCursor.h"

#include <cassert>

namespace fdo { namespace postgis {

FeatureReader::FeatureReader(Connection* conn, PgCursor* cursor, FdoClassDefinition* classDef)
    : Base(conn, cursor), mClassDef(classDef)
{
    FDO_SAFE_ADDREF(mClassDef.p);
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
    FDO_SAFE_ADDREF(mClassDef.p);
    return mClassDef.p;
}

FdoInt32 FeatureReader::GetDepth()
{
    // TODO: Verify this value.
    return 0;
}

const FdoByte* FeatureReader::GetGeometry(FdoString* propertyName, FdoInt32* count)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoByteArray* FeatureReader::GetGeometry(FdoString* propertyName)
{
    return Base::GetGeometry(propertyName);
}

FdoIFeatureReader* FeatureReader::GetFeatureObject(FdoString* propertyName)
{
    // TODO: An Association Property not supported yet by PostGIS provider.
    return NULL;
}

}} // namespace fdo::postgis

