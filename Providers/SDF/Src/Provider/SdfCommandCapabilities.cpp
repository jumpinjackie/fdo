// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "stdafx.h"
#include "SdfCommandCapabilities.h"
#include "SDF/SdfCommandType.h"

//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------


// constructs an SdfCommandCapabilities with the specified flag
SdfCommandCapabilities::SdfCommandCapabilities()
{
}


// default destructor
SdfCommandCapabilities::~SdfCommandCapabilities()
{
}


//-------------------------------------------------------
// FdoIDisposable implementation
//-------------------------------------------------------


// dispose this object
void SdfCommandCapabilities::Dispose()
{
    delete this;
}


//-------------------------------------------------------
// FdoICommandCapabilities implementation
//-------------------------------------------------------


// Returns an array of the CommandTypes supported by the feature provider.
// The size parameter gives the number of commands in the array.
FdoInt32* SdfCommandCapabilities::GetCommands(FdoInt32& size)
{
    // read-write commands
    static const FdoInt32 commandsReadWrite[] =
    {
        FdoCommandType_Select,
        FdoCommandType_Insert,
        FdoCommandType_Delete,
        FdoCommandType_Update,
        FdoCommandType_DescribeSchema,
        FdoCommandType_ApplySchema,
//      FdoCommandType_DestroySchema,
//      FdoCommandType_ActivateSpatialContext,
        FdoCommandType_CreateSpatialContext,
//      FdoCommandType_DestroySpatialContext,
        FdoCommandType_GetSpatialContexts,
//      FdoCommandType_CreateMeasureUnit,
//      FdoCommandType_DestroyMeasureUnit,
//      FdoCommandType_GetMeasureUnits,
//      FdoCommandType_SQLCommand,
//      FdoCommandType_AcquireLock,
//      FdoCommandType_GetLockInfo,
//      FdoCommandType_GetLockedObjects,
//      FdoCommandType_GetLockOwners,
//      FdoCommandType_ReleaseLock,
//      FdoCommandType_ActivateLongTransaction,
//      FdoCommandType_CommitLongTransaction,
//      FdoCommandType_CreateLongTransaction,
//      FdoCommandType_GetLongTransactions,
//      FdoCommandType_FreezeLongTransaction,
//      FdoCommandType_RollbackLongTransaction,
//      FdoCommandType_ActivateLongTransactionCheckpoint,
//      FdoCommandType_CreateLongTransactionCheckpoint,
//      FdoCommandType_GetLongTransactionCheckpoints,
//      FdoCommandType_RollbackLongTransactionCheckpoint,
//      FdoCommandType_ChangeLongTransactionPrivileges,
//      FdoCommandType_GetLongTransactionPrivileges,
//      FdoCommandType_ChangeLongTransactionSet,
//      FdoCommandType_GetLongTransactionsInSet,
		FdoCommandType_SelectAggregates,
        FdoCommandType_CreateDataStore,
		FdoCommandType_DestroyDataStore,
        SdfCommandType_CreateSDFFile
    };

    size = sizeof(commandsReadWrite) / sizeof(FdoCommandType);
    return (FdoInt32*)commandsReadWrite;
}


// Returns true if commands support parameterization, false otherwise.
bool SdfCommandCapabilities::SupportsParameters()
{
    // for SDF command parameters can't be changed and re-executed
    return false;
}


// Returns true if the feature provider supports command execution timeout.
bool SdfCommandCapabilities::SupportsTimeout()
{
    return false;
}


bool SdfCommandCapabilities::SupportsSelectExpressions() 
{ 
    return true; 
}

bool SdfCommandCapabilities::SupportsSelectFunctions() 
{ 
    return true; 
}

bool SdfCommandCapabilities::SupportsSelectDistinct() 
{ 
    return true; 
}

bool SdfCommandCapabilities::SupportsSelectOrdering() 
{ 
    return false; 
}

bool SdfCommandCapabilities::SupportsSelectGrouping() 
{ 
    return false; 
}

