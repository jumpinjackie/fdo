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

PgGeometryColumn::PgGeometryColumn(FdoString* name, FdoGeometryType type, FdoInt32 dim,
                                   FdoInt32 srid) :
    mName(name), mType(type), mDim(dim), mSRID(srid)
{
    FDOLOG_WRITE("PgGeometryColumn: %s", static_cast<char const*>(mName));
}

PgGeometryColumn::PgGeometryColumn(FdoString* name, FdoGeometryType type, FdoInt32 dim,
                                   FdoInt32 srid, FdoPtr<FdoEnvelopeImpl> bbox) :
    mName(name), mType(type), mDim(dim), mSRID(srid), mEnvelope(bbox)
{
    FDOLOG_WRITE("PgGeometryColumn: %s", static_cast<char const*>(mName));

    FDO_SAFE_ADDREF(mEnvelope.p);
}

PgGeometryColumn::~PgGeometryColumn()
{
    // idle
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void PgGeometryColumn::Dispose()
{
    //FDOLOG_MARKER("PgGeometryColumn::#Dispose");
    
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// PgGeometryColumn interface
///////////////////////////////////////////////////////////////////////////////

FdoStringP PgGeometryColumn::GetName() const
{
    return mName;
}

FdoStringP PgGeometryColumn::GetDescription() const
{
    // TODO: to implement
    return L"";
}

FdoGeometryType PgGeometryColumn::GetGeometryType() const
{
    return mType;
}

FdoInt32 PgGeometryColumn::GetDimensionType() const
{
    return mDim;
}

FdoInt32 PgGeometryColumn::GetSRID() const
{
    return mSRID;
}

FdoPtr<FdoEnvelopeImpl> PgGeometryColumn::GetEnvelope() const
{
    FDO_SAFE_ADDREF(mEnvelope.p);
    return mEnvelope.p;
}


}} // namespace fdo::postgis
