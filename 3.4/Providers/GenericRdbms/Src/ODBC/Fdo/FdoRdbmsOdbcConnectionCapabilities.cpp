/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "stdafx.h"
#include "FdoRdbmsOdbcConnectionCapabilities.h"

FdoRdbmsOdbcConnectionCapabilities::FdoRdbmsOdbcConnectionCapabilities(void)
{
}

FdoRdbmsOdbcConnectionCapabilities::~FdoRdbmsOdbcConnectionCapabilities(void)
{
}

FdoThreadCapability FdoRdbmsOdbcConnectionCapabilities::GetThreadCapability()
{
    return FdoThreadCapability_PerConnectionThreaded;
}

FdoSpatialContextExtentType* FdoRdbmsOdbcConnectionCapabilities::GetSpatialContextTypes(int& length)
{
    static FdoSpatialContextExtentType types[] = {FdoSpatialContextExtentType_Static};
    length = sizeof(types)/sizeof(FdoSpatialContextExtentType);
    return types;
}

bool FdoRdbmsOdbcConnectionCapabilities::SupportsLocking()
{
    return false;
}

FdoLockType* FdoRdbmsOdbcConnectionCapabilities::GetLockTypes(FdoInt32& size)
{
    size = 0;
    return NULL;
}

bool FdoRdbmsOdbcConnectionCapabilities::SupportsTimeout()
{
    return false;
}

bool FdoRdbmsOdbcConnectionCapabilities::SupportsTransactions()
{
    return false;
}

bool FdoRdbmsOdbcConnectionCapabilities::SupportsLongTransactions()
{
    return false;
}

bool FdoRdbmsOdbcConnectionCapabilities::SupportsSQL()
{
    return true;
}

void FdoRdbmsOdbcConnectionCapabilities::Dispose()
{
    delete this;
}

bool FdoRdbmsOdbcConnectionCapabilities::SupportsConfiguration()
{
    return true;
}

bool FdoRdbmsOdbcConnectionCapabilities::SupportsMultipleSpatialContexts()
{
    return false;
}

bool FdoRdbmsOdbcConnectionCapabilities::SupportsCSysWKTFromCSysName()
{
    return false;
}

bool FdoRdbmsOdbcConnectionCapabilities::SupportsWrite()
{
    return true;
}

bool FdoRdbmsOdbcConnectionCapabilities::SupportsMultiUserWrite()
{
    return true;
}
