/*
 * 
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
#include "FdoRdbmsConnectionCapabilities.h"

FdoRdbmsConnectionCapabilities::FdoRdbmsConnectionCapabilities(void)
{
}

FdoRdbmsConnectionCapabilities::~FdoRdbmsConnectionCapabilities(void)
{
}

FdoThreadCapability FdoRdbmsConnectionCapabilities::GetThreadCapability()
{
    return FdoThreadCapability_PerConnectionThreaded;
}


FdoSpatialContextExtentType* FdoRdbmsConnectionCapabilities::GetSpatialContextTypes(int& length)
{
    static FdoSpatialContextExtentType types[] = {FdoSpatialContextExtentType_Static};
    length = sizeof(types)/sizeof(FdoSpatialContextExtentType);
    return types;
}

FdoLockType* FdoRdbmsConnectionCapabilities::GetLockTypes(FdoInt32& size)
{
    static FdoLockType lockTypes[] = {
        FdoLockType_Shared,
        FdoLockType_Exclusive,
        FdoLockType_Transaction,
        FdoLockType_AllLongTransactionExclusive,
        FdoLockType_LongTransactionExclusive
    };

    size = sizeof(lockTypes)/sizeof(FdoLockType);
    return lockTypes;
}

bool FdoRdbmsConnectionCapabilities::SupportsLocking()
{
    return true;
}

bool FdoRdbmsConnectionCapabilities::SupportsTimeout()
{
    return false;
}

bool FdoRdbmsConnectionCapabilities::SupportsTransactions()
{
    return true;
}

bool FdoRdbmsConnectionCapabilities::SupportsLongTransactions()
{
    return true;
}

bool FdoRdbmsConnectionCapabilities::SupportsSQL()
{
    return true;
}

bool FdoRdbmsConnectionCapabilities::SupportsConfiguration()
{
    return false;
}

void FdoRdbmsConnectionCapabilities::Dispose()
{
    delete this;
}

bool FdoRdbmsConnectionCapabilities::SupportsMultipleSpatialContexts()
{
    return true;
}

bool FdoRdbmsConnectionCapabilities::SupportsCSysWKTFromCSysName()
{
    return true;
}

bool FdoRdbmsConnectionCapabilities::SupportsWrite()
{
    return true;
}

bool FdoRdbmsConnectionCapabilities::SupportsMultiUserWrite()
{
    return true;
}
