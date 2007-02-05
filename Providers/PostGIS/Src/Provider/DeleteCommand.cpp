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
#include "DeleteCommand.h"
#include "FeatureCommand.h"
#include "Connection.h"

#include <cassert>

namespace fdo { namespace postgis {

DeleteCommand::DeleteCommand(Connection* conn)
    : FeatureCommand(conn)
{
}

DeleteCommand::~DeleteCommand()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

FdoInt32 DeleteCommand::Execute()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoILockConflictReader* DeleteCommand::GetLockConflicts()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

}} // namespace fdo::postgis
