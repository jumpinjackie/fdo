#ifndef _COMMANDTYPE_H_
#define _COMMANDTYPE_H_
// 

//
// Copyright (C) 2004-2006  Autodesk, Inc.
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

#ifdef _WIN32
#pragma once
#endif

/// \brief
/// The FdoCommandType enumeration defines the list of abstract commands a feature
/// provider can support.
/// <param name="FdoCommandType_Select">
/// Represents the Select command
/// </param>
/// <param name="FdoCommandType_Insert">
/// Represents the Insert command
/// </param>
/// <param name="FdoCommandType_Delete">
/// Represents the Delete command
/// </param>
/// <param name="FdoCommandType_Update">
/// Represents the Update command
/// </param>
/// <param name="FdoCommandType_DescribeSchema">
/// Represents the DescribeSchema command
/// </param>
/// <param name="FdoCommandType_DescribeSchemaMapping">
/// Represents the DescribeSchemaMapping command
/// </param>
/// <param name="FdoCommandType_ApplySchema">
/// Represents the ApplySchema command.
/// </param>
/// <param name="FdoCommandType_DestroySchema">
/// Represents the DestroySchema command.
/// </param>
/// <param name="FdoCommandType_ActivateSpatialContext">
/// Represents the ActivateSpatialContext command.
/// </param>
/// <param name="FdoCommandType_CreateSpatialContext">
/// Represents the CreateSpatialContext command.
/// </param>
/// <param name="FdoCommandType_DestroySpatialContext">
/// Represents the DestroySpatialContext command.
/// </param>
/// <param name="FdoCommandType_GetSpatialContexts">
/// Represents the GetSpatialContexts command.
/// </param>
/// <param name="FdoCommandType_CreateMeasureUnit">
/// Represents the CreateMeasureUnit command.
/// </param>
/// <param name="FdoCommandType_DestroyMeasureUnit">
/// Represents the DestroyMeasureUnit command.
/// </param>
/// <param name="FdoCommandType_GetMeasureUnits">
/// Represents the GetMeasureUnits command.
/// </param>
/// <param name="FdoCommandType_SQLCommand">
/// Represents the SQLCommand command.
/// </param>
/// <param name="FdoCommandType_AcquireLock">
/// Represents the AcquireLock command.
/// </param>
/// <param name="FdoCommandType_GetLockInfo">
/// Represents the GetLockInfo command.
/// </param>
/// <param name="FdoCommandType_GetLockedObjects">
/// Represents the GetLockedObjects command.
/// </param>
/// <param name="FdoCommandType_GetLockOwners">
/// Represents the GetLockOwners command.
/// </param>
/// <param name="FdoCommandType_ReleaseLock">
/// Represents the ReleaseLock command.
/// </param>
/// <param name="FdoCommandType_ActivateLongTransaction">
/// Represents the ActivateLongTransaction command.
/// </param>
/// <param name="FdoCommandType_CommitLongTransaction">
/// Represents the CommitLongTransaction command.
/// </param>
/// <param name="FdoCommandType_CreateLongTransaction">
/// Represents the CreateLongTransaction command.
/// </param>
/// <param name="FdoCommandType_GetLongTransactions">
/// Represents the GetLongTransactions command.
/// </param>
/// <param name="FdoCommandType_FreezeLongTransaction">
/// Represents the FreezeLongTransaction command.
/// </param>
/// <param name="FdoCommandType_RollbackLongTransaction">
/// Represents the RollbackLongTransaction command.
/// </param>
/// <param name="FdoCommandType_ActivateLongTransactionCheckpoint">
/// Represents the ActivateLongTransactionCheckpoint command.
/// </param>
/// <param name="FdoCommandType_CreateLongTransactionCheckpoint">
/// Represents the CreateLongTransactionCheckpoint command.
/// </param>
/// <param name="FdoCommandType_GetLongTransactionCheckpoints">
/// Represents the GetLongTransactionCheckpoints command.
/// </param>
/// <param name="FdoCommandType_RollbackLongTransactionCheckpoint">
/// Represents the RollbackLongTransactionCheckpoint command.
/// </param>
/// <param name="FdoCommandType_ChangeLongTransactionPrivileges">
/// Represents the ChangeLongTransactionPrivileges command.
/// </param>
/// <param name="FdoCommandType_GetLongTransactionPrivileges">
/// Represents the GetLongTransactionPrivileges command.
/// </param>
/// <param name="FdoCommandType_ChangeLongTransactionSet">
/// Represents the ChangeLongTransactionSet command.
/// </param>
/// <param name="FdoCommandType_GetLongTransactionsInSet">
/// Represents the GetLongTransactionsInSet command.
/// </param>
/// <param name="FdoCommandType_FirstProviderCommand">
/// Represents the index of the first provider specific command.
/// </param>
/// <param name="FdoCommandType_SelectAggregates">
/// Represents the SelectAggregate command.
/// </param>
/// <param name="(See Description note)">
/// \note
/// The following parameters are not supported in this release. 
/// They are provided here for informational use only. Autodesk 
/// reserves the right to change the software related to the content herein.
/// </param>
/// <param name="FdoCommandType_NetworkShortestPath">
/// Represents the NetworkShortestPath command (see <b>Note</b>).
/// </param>
/// <param name="FdoCommandType_NetworkAllPaths">
/// Represents the NetworkAllPaths command (see <b>Note</b>).
/// </param>
/// <param name="FdoCommandType_NetworkReachableNodes">
/// Represents the NetworkReachableNodes command (see <b>Note</b>).
/// </param>
/// <param name="FdoCommandType_NetworkReachingNodes">
/// Represents the NetworkReachingNodes command (see <b>Note</b>).
/// </param>
/// <param name="FdoCommandType_NetworkNearestNeighbors">
/// Represents the NetworkNearestNeighbors command (see <b>Note</b>).
/// </param>
/// <param name="FdoCommandType_NetworkWithinCost">
/// Represents the NetworkWithinCost command (see <b>Note</b>).
/// </param>
/// <param name="FdoCommandType_NetworkTSP">
/// Represents the NetworkTSP command (see <b>Note</b>).
/// </param>
/// <param name="FdoCommandType_ActivateTopologyArea">
/// Represents the ActivateTopologyArea command (see <b>Note</b>).
/// </param>
/// <param name="FdoCommandType_DeactivateTopologyArea">
/// Represents the DeactivateTopologyArea command (see <b>Note</b>).
/// </param>
/// <param name="FdoCommandType_ActivateTopologyInCommandResult">
/// Represents the ActivateTopologyInCommandResult command (see <b>Note</b>).
/// </param>
/// <param name="FdoCommandType_DeactivateTopologyInCommandResults">
/// Represents the DeactivateTopologyInCommandResults command (see <b>Note</b>).
/// </param>
/// <param name="FdoCommandType_CreateDataStore">
/// Represents the CreateDataStore command.
/// </param>
/// <param name="FdoCommandType_DestroyDataStore">
/// Represents the DestroyDataStore command.
/// </param>
/// <param name="FdoCommandType_ListDataStores">
/// Represents the ListDataStores command.
/// </param>

enum FdoCommandType{
    /// Represents the Select Command.
    FdoCommandType_Select,

    /// Represents the Insert Command.
    FdoCommandType_Insert,

    /// Represents the Delete Command.
    FdoCommandType_Delete,

    /// Represents the Update Command.
    FdoCommandType_Update,

    /// Represents the DescribeSchema Command.
    FdoCommandType_DescribeSchema,

    /// Represents the DescribeSchemaMapping Command.
    FdoCommandType_DescribeSchemaMapping,

    /// Represents the ApplySchema command.
    FdoCommandType_ApplySchema,

    /// Represents the DestroySchema command.
    FdoCommandType_DestroySchema,

    /// Represents the ActivateSpatialContext command.
    FdoCommandType_ActivateSpatialContext,

    /// Represents the CreateSpatialContext command.
    FdoCommandType_CreateSpatialContext,

    /// Represents the DestroySpatialContext command.
    FdoCommandType_DestroySpatialContext,

    /// Represents the GetSpatialContexts command.
    FdoCommandType_GetSpatialContexts,

    /// Represents the CreateMeasureUnit command.
    FdoCommandType_CreateMeasureUnit,

    /// Represents the DestroyMeasureUnit command.
    FdoCommandType_DestroyMeasureUnit,

    /// Represents the GetMeasureUnits command.
    FdoCommandType_GetMeasureUnits,

    /// Represents the SQLCommand command.
    FdoCommandType_SQLCommand,

    /// Represents the AcquireLock command.
    FdoCommandType_AcquireLock,

    /// Represents the GetLockInfo command.
    FdoCommandType_GetLockInfo,

    /// Represents the GetLockedObjects command.
    FdoCommandType_GetLockedObjects,

    /// Represents the GetLockOwners command.
    FdoCommandType_GetLockOwners,

    /// Represents the ReleaseLock command.
    FdoCommandType_ReleaseLock,

    /// Represents the ActivateLongTransaction command.
    FdoCommandType_ActivateLongTransaction,

    /// Represents the DeactivateLongTransaction command.
    FdoCommandType_DeactivateLongTransaction,

    /// Represents the CommitLongTransaction command.
    FdoCommandType_CommitLongTransaction,

    /// Represents the CreateLongTransaction command.
    FdoCommandType_CreateLongTransaction,

    /// Represents the GetLongTransactions command.
    FdoCommandType_GetLongTransactions,

    /// Represents the FreezeLongTransaction command.
    FdoCommandType_FreezeLongTransaction,

    /// Represents the RollbackLongTransaction command.
    FdoCommandType_RollbackLongTransaction,

    /// Represents the ActivateLongTransactionCheckpoint command.
    FdoCommandType_ActivateLongTransactionCheckpoint,

    /// Represents the CreateLongTransactionCheckpoint command.
    FdoCommandType_CreateLongTransactionCheckpoint,

    /// Represents the GetLongTransactionCheckpoints command.
    FdoCommandType_GetLongTransactionCheckpoints,

    /// Represents the RollbackLongTransactionCheckpoint command.
    FdoCommandType_RollbackLongTransactionCheckpoint,

    /// Represents the ChangeLongTransactionPrivileges command.
    FdoCommandType_ChangeLongTransactionPrivileges,

    /// Represents the GetLongTransactionPrivileges command.
    FdoCommandType_GetLongTransactionPrivileges,

    /// Represents the ChangeLongTransactionSet command.
    FdoCommandType_ChangeLongTransactionSet,

    /// Represents the GetLongTransactionsInSet command.
    FdoCommandType_GetLongTransactionsInSet,

    /// Represents the NetworkShortestPath command.
    FdoCommandType_NetworkShortestPath,

    /// Represents the NetworkAllPaths command.
    FdoCommandType_NetworkAllPaths,

    /// Represents the NetworkReachableNodes command.
    FdoCommandType_NetworkReachableNodes,

    /// Represents the NetworkReachingNodes command.
    FdoCommandType_NetworkReachingNodes,

    /// Represents the NetworkNearestNeighbors command.
    FdoCommandType_NetworkNearestNeighbors,

    /// Represents the NetworkWithinCost command.
    FdoCommandType_NetworkWithinCost,

    /// Represents the NetworkTSP command.
    FdoCommandType_NetworkTSP,

    /// Represents the ActivateTopologyArea command.
    FdoCommandType_ActivateTopologyArea,

    /// Represents the DeactivateTopologyArea command.
    FdoCommandType_DeactivateTopologyArea,

    /// Represents the ActivateTopologyInCommandResult command.
    FdoCommandType_ActivateTopologyInCommandResult,

    /// Represents the DeactivateTopologyInCommandResults command.
    FdoCommandType_DeactivateTopologyInCommandResults,

    /// Represents the SelectAggregates command.
    FdoCommandType_SelectAggregates,

    /// Represents the CreateDataStore command.
    FdoCommandType_CreateDataStore,

    /// Represents the DestroyDataStore command.
    FdoCommandType_DestroyDataStore,

    /// Represents the ListDatastores command
	FdoCommandType_ListDataStores,

    /// Represents the index of the first provider specific command.
    FdoCommandType_FirstProviderCommand

};


#endif


