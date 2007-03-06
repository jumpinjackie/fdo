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
#include "PgGeometryColumn.h"
// std
#include <cassert>
#include <string>

namespace fdo { namespace postgis {

PgGeometryColumn::PgGeometryColumn(FdoString* name, FdoDimensionality dim,
                                   FdoGeometryType type, FdoInt32 srid)
    : mName(name), mDim(dim), mType(type), mSRID(srid)
{
}

PgGeometryColumn::~PgGeometryColumn()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void PgGeometryColumn::Dispose()
{
    FDOLOG_MARKER("PgGeometryColumn::#Dispose");

    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// PgGeometryColumn interface
///////////////////////////////////////////////////////////////////////////////

FdoStringP PgGeometryColumn::GetName() const
{
    return mName;
}

FdoDimensionality PgGeometryColumn::GetDimensionality() const
{
    return mDim;
}

FdoGeometryType PgGeometryColumn::GetGeometryType() const
{
    return mType;
}

FdoInt32 PgGeometryColumn::GetSRID() const
{
    return mSRID;
}

}} // namespace fdo::postgis
