/*
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

#pragma once

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS

/// \brief
/// The CommandType enumeration defines the list of abstract commands a feature provider can support.
///
/// \param CommandType_Select
/// Represents the Select command
///
/// \param CommandType_Insert
/// Represents the Insert command
///
/// \param CommandType_Delete
/// Represents the Delete command
///
/// \param CommandType_Update
/// Represents the Update command
///
/// \param CommandType_DescribeSchema
/// Represents the DescribeSchema command
///
/// \param CommandType_DescribeSchemaMapping
/// Represents the DescribeSchemaMapping command
///
/// \param CommandType_ApplySchema
/// Represents the ApplySchema command.
///
/// \param CommandType_DestroySchema
/// Represents the DestroySchema command.
///
/// \param CommandType_ActivateSpatialContext
/// Represents the ActivateSpatialContext command.
///
/// \param CommandType_CreateSpatialContext
/// Represents the CreateSpatialContext command.
///
/// \param CommandType_DestroySpatialContext
/// Represents the DestroySpatialContext command.
///
/// \param CommandType_GetSpatialContexts
/// Represents the GetSpatialContexts command.
///
/// \param CommandType_CreateMeasureUnit
/// Represents the CreateMeasureUnit command.
///
/// \param CommandType_DestroyMeasureUnit
/// Represents the DestroyMeasureUnit command.
///
/// \param CommandType_GetMeasureUnits
/// Represents the GetMeasureUnits command.
///
/// \param CommandType_SQLCommand
/// Represents the SQLCommand command.
///
/// \param CommandType_AcquireLock
/// Represents the AcquireLock command.
///
/// \param CommandType_GetLockInfo
/// Represents the GetLockInfo command.
///
/// \param CommandType_GetLockedObjects
/// Represents the GetLockedObjects command.
///
/// \param CommandType_GetLockOwners
/// Represents the GetLockOwners command.
///
/// \param CommandType_ReleaseLock
/// Represents the ReleaseLock command.
///
/// \param CommandType_ActivateLongTransaction
/// Represents the ActivateLongTransaction command.
///
/// \param CommandType_CommitLongTransaction
/// Represents the CommitLongTransaction command.
///
/// \param CommandType_CreateLongTransaction
/// Represents the CreateLongTransaction command.
///
/// \param CommandType_GetLongTransactions
/// Represents the GetLongTransactions command.
///
/// \param CommandType_FreezeLongTransaction
/// Represents the FreezeLongTransaction command.
///
/// \param CommandType_RollbackLongTransaction
/// Represents the RollbackLongTransaction command.
///
/// \param CommandType_ActivateLongTransactionCheckpoint
/// Represents the ActivateLongTransactionCheckpoint command.
///
/// \param CommandType_CreateLongTransactionCheckpoint
/// Represents the CreateLongTransactionCheckpoint command.
///
/// \param CommandType_GetLongTransactionCheckpoints
/// Represents the GetLongTransactionCheckpoints command.
///
/// \param CommandType_RollbackLongTransactionCheckpoint
/// Represents the RollbackLongTransactionCheckpoint command.
///
/// \param CommandType_ChangeLongTransactionPrivileges
/// Represents the ChangeLongTransactionPrivileges command.
///
/// \param CommandType_GetLongTransactionPrivileges
/// Represents the GetLongTransactionPrivileges command.
///
/// \param CommandType_ChangeLongTransactionSet
/// Represents the ChangeLongTransactionSet command.
///
/// \param CommandType_GetLongTransactionsInSet
/// Represents the GetLongTransactionsInSet command.
///
/// \param CommandType_FirstProviderCommand
/// Represents the index of the first provider specific command.
///
/// \param CommandType_SelectAggregates
/// Represents the SelectAggregate command.
///
/// \param CommandType_NetworkShortestPath
/// Represents the NetworkShortestPath command (see <b>remarks</b>).
///
/// \param CommandType_NetworkAllPaths
/// Represents the NetworkAllPaths command (see <b>remarks</b>).
///
/// \param CommandType_NetworkReachableNodes
/// Represents the NetworkReachableNodes command (see <b>remarks</b>).
///
/// \param CommandType_NetworkReachingNodes
/// Represents the NetworkReachingNodes command (see <b>remarks</b>).
///
/// \param CommandType_NetworkNearestNeighbors
/// Represents the NetworkNearestNeighbors command (see <b>remarks</b>).
///
/// \param CommandType_NetworkWithinCost
/// Represents the NetworkWithinCost command (see <b>remarks</b>).
///
/// \param CommandType_NetworkTSP
/// Represents the NetworkTSP command (see <b>remarks</b>).
///
/// \param CommandType_ActivateTopologyArea
/// Represents the ActivateTopologyArea command (see <b>remarks</b>).
///
/// \param CommandType_DeactivateTopologyArea
/// Represents the DeactivateTopologyArea command (see <b>remarks</b>).
///
/// \param CommandType_ActivateTopologyInCommandResult
/// Represents the ActivateTopologyInCommandResult command (see <b>remarks</b>).
///
/// \param CommandType_DeactivateTopologyInCommandResults
/// Represents the DeactivateTopologyInCommandResults command (see <b>remarks</b>).
///
/// \param CommandType_CreateDataStore
/// Represents the CreateDataStore command.
///
/// \param CommandType_DestroyDataStore
/// Represents the DestroyDataStore command.
///
/// \param CommandType_ListDataStores
/// Represents the ListDataStores command.
///
/// \remarks
/// These parameters are not supported in this release. 
/// They are provided here for informational use only. OSGeo 
/// reserves the right to change the software related to the content herein.
///

public __value enum CommandType {
	CommandType_Select = FdoCommandType_Select,
	CommandType_Insert = FdoCommandType_Insert,
	CommandType_Delete = FdoCommandType_Delete,
	CommandType_Update = FdoCommandType_Update,
	CommandType_DescribeSchema = FdoCommandType_DescribeSchema,
	CommandType_DescribeSchemaMapping = FdoCommandType_DescribeSchemaMapping,
	CommandType_ApplySchema = FdoCommandType_ApplySchema,
	CommandType_DestroySchema = FdoCommandType_DestroySchema,
	CommandType_ActivateSpatialContext = FdoCommandType_ActivateSpatialContext,
	CommandType_CreateSpatialContext = FdoCommandType_CreateSpatialContext,
	CommandType_DestroySpatialContext = FdoCommandType_DestroySpatialContext,
	CommandType_GetSpatialContexts = FdoCommandType_GetSpatialContexts,
	CommandType_CreateMeasureUnit = FdoCommandType_CreateMeasureUnit,
	CommandType_DestroyMeasureUnit = FdoCommandType_DestroyMeasureUnit,
	CommandType_GetMeasureUnits = FdoCommandType_GetMeasureUnits,
	CommandType_SQLCommand = FdoCommandType_SQLCommand,
	CommandType_AcquireLock = FdoCommandType_AcquireLock,
	CommandType_GetLockInfo = FdoCommandType_GetLockInfo,
	CommandType_GetLockedObjects = FdoCommandType_GetLockedObjects,
	CommandType_GetLockOwners = FdoCommandType_GetLockOwners,
	CommandType_ReleaseLock = FdoCommandType_ReleaseLock,
	CommandType_ActivateLongTransaction = FdoCommandType_ActivateLongTransaction,
	CommandType_DeactivateLongTransaction = FdoCommandType_DeactivateLongTransaction,
	CommandType_CommitLongTransaction = FdoCommandType_CommitLongTransaction,
	CommandType_CreateLongTransaction = FdoCommandType_CreateLongTransaction,
	CommandType_GetLongTransactions = FdoCommandType_GetLongTransactions,
	CommandType_FreezeLongTransaction = FdoCommandType_FreezeLongTransaction,
	CommandType_RollbackLongTransaction = FdoCommandType_RollbackLongTransaction,
	CommandType_ActivateLongTransactionCheckpoint = FdoCommandType_ActivateLongTransactionCheckpoint,
	CommandType_CreateLongTransactionCheckpoint = FdoCommandType_CreateLongTransactionCheckpoint,
	CommandType_GetLongTransactionCheckpoints = FdoCommandType_GetLongTransactionCheckpoints,
	CommandType_RollbackLongTransactionCheckpoint = FdoCommandType_RollbackLongTransactionCheckpoint,
	CommandType_ChangeLongTransactionPrivileges = FdoCommandType_ChangeLongTransactionPrivileges,
	CommandType_GetLongTransactionPrivileges = FdoCommandType_GetLongTransactionPrivileges,
	CommandType_ChangeLongTransactionSet = FdoCommandType_ChangeLongTransactionSet,
	CommandType_GetLongTransactionsInSet = FdoCommandType_GetLongTransactionsInSet,
	CommandType_NetworkShortestPath = FdoCommandType_NetworkShortestPath,
	CommandType_NetworkAllPaths = FdoCommandType_NetworkAllPaths,
	CommandType_NetworkReachableNodes = FdoCommandType_NetworkReachableNodes,
	CommandType_NetworkReachingNodes = FdoCommandType_NetworkReachingNodes,
	CommandType_NetworkNearestNeighbors = FdoCommandType_NetworkNearestNeighbors,
	CommandType_NetworkWithinCost = FdoCommandType_NetworkWithinCost,
	CommandType_NetworkTSP = FdoCommandType_NetworkTSP,
	CommandType_ActivateTopologyArea = FdoCommandType_ActivateTopologyArea,
	CommandType_DeactivateTopologyArea = FdoCommandType_DeactivateTopologyArea,
	CommandType_ActivateTopologyInCommandResult = FdoCommandType_ActivateTopologyInCommandResult,
	CommandType_DeactivateTopologyInCommandResults = FdoCommandType_DeactivateTopologyInCommandResults,
	CommandType_SelectAggregates = FdoCommandType_SelectAggregates,
	CommandType_FirstProviderCommand = FdoCommandType_FirstProviderCommand,
    CommandType_CreateDataStore = FdoCommandType_CreateDataStore,
    CommandType_DestroyDataStore = FdoCommandType_DestroyDataStore,
	CommandType_ListDataStores = FdoCommandType_ListDataStores,
};

END_NAMESPACE_OSGEO_FDO_COMMANDS


