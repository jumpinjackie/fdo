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

#include <cassert>

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
    assert(!"NOT IMPLEMENTED");
    return FdoThreadCapability_SingleThreaded;
}

FdoSpatialContextExtentType* ConnectionCapabilities::GetpatialContextTypes(FdoInt32& size)
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoLockType* ConnectionCapabilities::GetLockTypes(FdoInt32& size)
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

bool ConnectionCapabilities::SupportsLocking()
{
    assert(!"NOT IMPLEMENTED");
    return false;
}

bool ConnectionCapabilities::SupportsTimeout()
{
    assert(!"NOT IMPLEMENTED");
    return false;
}

bool ConnectionCapabilities::SupportsTransactions()
{
    assert(!"NOT IMPLEMENTED");
    return false;
}

bool ConnectionCapabilities::SupportsLongTransactions()
{
    assert(!"NOT IMPLEMENTED");
    return false;
}

bool ConnectionCapabilities::SupportsSQL()
{
    assert(!"NOT IMPLEMENTED");
    return false;
}

bool ConnectionCapabilities::SupportsConfiguration()
{
    assert(!"NOT IMPLEMENTED");
    return false;
}

bool ConnectionCapabilities::SupportsMultipleSpatialContexts()
{
    assert(!"NOT IMPLEMENTED");
    return false;
}

bool ConnectionCapabilities::SupportsCSysWKTFromCSysName()
{
    assert(!"NOT IMPLEMENTED");
    return false;
}

bool ConnectionCapabilities::SupportsWrite()
{
    assert(!"NOT IMPLEMENTED");
    return false;
}

bool ConnectionCapabilities::SupportsMultiUserWrite()
{
    assert(!"NOT IMPLEMENTED");
    return false;
}

bool ConnectionCapabilities::SupportsFlush()
{
    assert(!"NOT IMPLEMENTED");
    return false;
}

}} // namespace fdo::postgis
