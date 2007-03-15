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
#include "CommandCapabilities.h"

namespace fdo { namespace postgis {

CommandCapabilities::CommandCapabilities()
{
    FDOLOG_WRITE("CommandCapabilities created");
}

CommandCapabilities::~CommandCapabilities()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void CommandCapabilities::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoICommandCapabilities interface
///////////////////////////////////////////////////////////////////////////////

FdoInt32* CommandCapabilities::GetCommands(FdoInt32& size)
{
    // TODO: Verify list of commands

    static FdoCommandType commands[] =
    {
        FdoCommandType_Select,
        //FdoCommandType_Insert,
        //FdoCommandType_Update,
        //FdoCommandType_Delete,
        FdoCommandType_SQLCommand,
        //FdoCommandType_ApplySchema
        //FdoCommandType_DestroySchema
        FdoCommandType_DescribeSchema,
        //FdoCommandType_DescribeSchemaMapping,
        FdoCommandType_CreateDataStore,
        FdoCommandType_DestroyDataStore,
        FdoCommandType_ListDataStores,
        FdoCommandType_GetSpatialContexts
    };

    size = (sizeof(commands) / sizeof(FdoCommandType));

    return (reinterpret_cast<FdoInt32*>(commands));
}

bool CommandCapabilities::SupportsParameters()
{
    return true;
}

bool CommandCapabilities::SupportsTimeout()
{
    return false;
}

bool CommandCapabilities::SupportsSelectExpressions()
{
    return true;
}

bool CommandCapabilities::SupportsSelectFunctions()
{
    return true;
}

bool CommandCapabilities::SupportsSelectDistinct()
{
    return true;
}

bool CommandCapabilities::SupportsSelectOrdering()
{
    return true;
}

bool CommandCapabilities::SupportsSelectGrouping()
{
    // TODO: Verify grouping
    return false;
}

}} // namespace fdo::postgis
