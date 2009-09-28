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
#include "ConnectionCapabilities.h"

namespace fdo { namespace postgis {

ConnectionCapabilities::ConnectionCapabilities()
{
}

ConnectionCapabilities::~ConnectionCapabilities()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void ConnectionCapabilities::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIConnectionCapabilities interface
///////////////////////////////////////////////////////////////////////////////

FdoThreadCapability ConnectionCapabilities::GetThreadCapability()
{
    // TODO: Verify threading model

    return FdoThreadCapability_PerConnectionThreaded;
    //return FdoThreadCapability_PerCommandThreaded;
}

FdoSpatialContextExtentType* ConnectionCapabilities::GetSpatialContextTypes(FdoInt32& size)
{
    static FdoSpatialContextExtentType types[] = {
        //FdoSpatialContextExtentType_Static
        FdoSpatialContextExtentType_Dynamic
    };

    size = (sizeof(types) / sizeof(FdoSpatialContextExtentType));

    return types;
}

FdoLockType* ConnectionCapabilities::GetLockTypes(FdoInt32& size)
{
    // TODO: Verify supported types of locks

    static FdoLockType types[] = {
        FdoLockType_Transaction

        //FdoLockType_Shared,
        //FdoLockType_Exclusive,
        //FdoLockType_AllLongTransactionExclusive,
        //FdoLockType_LongTransactionExclusive
    };

    size = (sizeof(types) / sizeof(FdoLockType));
    return types;
}

bool ConnectionCapabilities::SupportsLocking()
{
    return false;
}

bool ConnectionCapabilities::SupportsTimeout()
{
    return false;
}

bool ConnectionCapabilities::SupportsTransactions()
{
    // TODO: We should investigate on why transaction crashes Autodesk Map (#314)

    return false;
}

bool ConnectionCapabilities::SupportsLongTransactions()
{
    return false;
}

bool ConnectionCapabilities::SupportsSQL()
{
    return true;
}

bool ConnectionCapabilities::SupportsConfiguration()
{
    return false;
}

bool ConnectionCapabilities::SupportsMultipleSpatialContexts()
{
    return true;
}

bool ConnectionCapabilities::SupportsCSysWKTFromCSysName()
{
    return false;
}

bool ConnectionCapabilities::SupportsWrite()
{
    return true;
}

bool ConnectionCapabilities::SupportsMultiUserWrite()
{
    return false;
}

bool ConnectionCapabilities::SupportsFlush()
{
    return false;
}

}} // namespace fdo::postgis
