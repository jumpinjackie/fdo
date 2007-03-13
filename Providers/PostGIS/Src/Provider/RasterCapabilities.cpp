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
#include "RasterCapabilities.h"

namespace fdo { namespace postgis {

RasterCapabilities::RasterCapabilities()
{
    FDOLOG_WRITE("RasterCapabilities created");
}

RasterCapabilities::~RasterCapabilities()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void RasterCapabilities::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIRasterCapabilities interface
///////////////////////////////////////////////////////////////////////////////

bool RasterCapabilities::SupportsRaster()
{
    return false;
}

bool RasterCapabilities::SupportsStitching()
{
    return false;
}

bool RasterCapabilities::SupportsSubsampling()
{
    return false;
}

bool RasterCapabilities::SupportsDataModel(FdoRasterDataModel *model)
{
    return false;
}

}} // namespace fdo::postgis
