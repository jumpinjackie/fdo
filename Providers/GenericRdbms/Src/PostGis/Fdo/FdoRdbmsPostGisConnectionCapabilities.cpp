//
// Copyright (C) 2006 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include "FdoRdbmsPostGisConnectionCapabilities.h"

FdoRdbmsPostGisConnectionCapabilities::FdoRdbmsPostGisConnectionCapabilities(void)
{
}

FdoRdbmsPostGisConnectionCapabilities::~FdoRdbmsPostGisConnectionCapabilities(void)
{
}


FdoLockType* FdoRdbmsPostGisConnectionCapabilities::GetLockTypes(FdoInt32& size)
{
    size = 0;
    return NULL;
}

bool FdoRdbmsPostGisConnectionCapabilities::SupportsLocking()
{
    return false;
}

bool FdoRdbmsPostGisConnectionCapabilities::SupportsLongTransactions()
{
    return false;
}

bool FdoRdbmsPostGisConnectionCapabilities::SupportsCSysWKTFromCSysName()
{
    // TODO - mloskot - review this return value
    return false;
}


void FdoRdbmsPostGisConnectionCapabilities::Dispose()
{
    delete this;
}
