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
#include <Rdbms/Override/PostGis/PostGisOv.h>
#include "FdoRdbmsPostGisCommandCapabilities.h"
#include <Rdbms/FdoRdbmsCommandType.h>

FdoRdbmsPostGisCommandCapabilities::FdoRdbmsPostGisCommandCapabilities(void)
{
    // idle
}

FdoRdbmsPostGisCommandCapabilities::~FdoRdbmsPostGisCommandCapabilities(void)
{
    // idle
}

int* FdoRdbmsPostGisCommandCapabilities::GetCommands(int& size)
{
    // TODO: update to new CommandTypes not included here
    static const int commands[] =
    {
        FdoCommandType_Select,
        FdoCommandType_SelectAggregates,
        FdoCommandType_Insert,
        FdoCommandType_Delete,
        FdoCommandType_Update,
        FdoCommandType_DescribeSchema,
        FdoCommandType_ApplySchema,
        FdoCommandType_DestroySchema,
        FdoCommandType_CreateSpatialContext,
        FdoCommandType_DestroySpatialContext,
        FdoCommandType_GetSpatialContexts,
		FdoCommandType_CreateDataStore,
		FdoCommandType_DestroyDataStore,
		FdoCommandType_ListDataStores,
        FdoCommandType_DescribeSchemaMapping,
        FdoCommandType_SQLCommand,
        FdoCommandType_GetSchemaNames,
        FdoCommandType_GetClassNames,
        // Custom commands
        FdoRdbmsCommandType_CreateSpatialIndex,
        FdoRdbmsCommandType_DestroySpatialIndex,
        FdoRdbmsCommandType_GetSpatialIndexes
    };

    size = sizeof(commands) / sizeof(int);
    return (int*) commands;
}
