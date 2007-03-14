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
#include "SpatialContextReader.h"

namespace fdo { namespace postgis {

SpatialContextReader::SpatialContextReader(SpatialContextCollection*  scc)
    :   mSpatialContexts(scc), mCurrentContext(NULL), mCurrentIndex(0)
{
    FDO_SAFE_ADDREF(mSpatialContexts.p);
}

SpatialContextReader::~SpatialContextReader()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void SpatialContextReader::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoISpatialContextReader interface
///////////////////////////////////////////////////////////////////////////////

FdoString* SpatialContextReader::GetName()
{
    return mCurrentContext->GetName();
}

FdoString* SpatialContextReader::GetDescription()
{
    return mCurrentContext->GetDescription();
}

FdoString* SpatialContextReader::GetCoordinateSystem()
{
    return mCurrentContext->GetCoordinateSystem();
}

FdoString* SpatialContextReader::GetCoordinateSystemWkt()
{
return mCurrentContext->GetCoordinateSystemWkt();
}

FdoSpatialContextExtentType SpatialContextReader::GetExtentType()
{
    return mCurrentContext->GetExtentType();
}

FdoByteArray* SpatialContextReader::GetExtent()
{
    return mCurrentContext->GetExtent();
}

const double SpatialContextReader::GetXYTolerance()
{
    return mCurrentContext->GetXYTolerance();
}

const double SpatialContextReader::GetZTolerance()
{
    return mCurrentContext->GetZTolerance();
}

const bool SpatialContextReader::IsActive()
{
    // TODO: It needs to be verified
    return true;
}

bool SpatialContextReader::ReadNext()
{
    bool eof = true;
    
    if (mCurrentIndex < mSpatialContexts->GetCount())
    {
        int const current = mCurrentIndex;
        mCurrentContext = mSpatialContexts->GetItem(current);
        mCurrentIndex++;
        eof = false;
    }
    
    return (!eof);
}

}} // namespace fdo::postgis
