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
#include "FilterProcessor.h"

#include <cassert>

namespace fdo { namespace postgis {

DeleteCommand::DeleteCommand(Connection* conn) : FeatureCommand(conn)
{
}

DeleteCommand::~DeleteCommand()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDelete interface
///////////////////////////////////////////////////////////////////////////////

FdoInt32 DeleteCommand::Execute()
{
    ///////////////////////////////////////////////////
    // TODO: This is temporary test of filter processor
    //       It is not an impl. of Delete command.
    ///////////////////////////////////////////////////

    FilterProcessor::Ptr proc(new FilterProcessor());
    mFilter->Process(proc);

    return 0;
}

FdoILockConflictReader* DeleteCommand::GetLockConflicts()
{
    return 0;
}

}} // namespace fdo::postgis
