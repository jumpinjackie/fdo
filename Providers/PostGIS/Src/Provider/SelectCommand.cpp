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
#include "SelectCommand.h"
#include "Connection.h"

#include <cassert>

namespace fdo { namespace postgis {

SelectCommand::SelectCommand(Connection* conn) : Base(conn)
{
}

SelectCommand::~SelectCommand()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void SelectCommand::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIBaseSelect interface
///////////////////////////////////////////////////////////////////////////////

FdoIdentifierCollection* SelectCommand::GetPropertyNames()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}
 	
FdoIdentifierCollection* SelectCommand::GetOrdering()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

void SelectCommand::SetOrderingOption(FdoOrderingOption option)
{
    assert(!"NOT IMPLEMENTED");
}

FdoOrderingOption SelectCommand::GetOrderingOption()
{
    assert(!"NOT IMPLEMENTED");
    return FdoOrderingOption_Ascending;
}

///////////////////////////////////////////////////////////////////////////////
// FdoISelect interface
///////////////////////////////////////////////////////////////////////////////

FdoLockType SelectCommand::GetLockType()
{
    assert(!"NOT IMPLEMENTED");
    return FdoLockType_Unsupported;
}
    
void SelectCommand::SetLockType(FdoLockType value)
{
    assert(!"NOT IMPLEMENTED");
}
    
FdoLockStrategy SelectCommand::GetLockStrategy()
{
    assert(!"NOT IMPLEMENTED");
    return FdoLockStrategy_All;
}
    
void SelectCommand::SetLockStrategy(FdoLockStrategy value)
{
    assert(!"NOT IMPLEMENTED");
}
    
FdoIFeatureReader* SelectCommand::Execute()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}
    
FdoIFeatureReader* SelectCommand::ExecuteWithLock()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}
    
FdoILockConflictReader* SelectCommand::GetLockConflicts()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

}} // namespace fdo::postgis
