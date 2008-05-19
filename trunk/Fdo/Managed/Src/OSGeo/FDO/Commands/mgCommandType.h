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

/// \cond DOXYGEN-IGNORE
BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS
/// \endcond

/// \ingroup (OSGeoFDOCommands)
/// \brief
/// The CommandType enumeration defines the list of abstract commands a feature provider can support.
public __value enum CommandType {
    /// Represents the Select command
	CommandType_Select = FdoCommandType_Select,

    /// Represents the Insert command
	CommandType_Insert = FdoCommandType_Insert,

    /// Represents the Delete command
	CommandType_Delete = FdoCommandType_Delete,

    /// Represents the Update command
	CommandType_Update = FdoCommandType_Update,

    /// Represents the DescribeSchema command
	CommandType_DescribeSchema = FdoCommandType_DescribeSchema,

    /// Represents the DescribeSchemaMapping command
	CommandType_DescribeSchemaMapping = FdoCommandType_DescribeSchemaMapping,

    /// Represents the ApplySchema command.
	CommandType_ApplySchema = FdoCommandType_ApplySchema,

    /// Represents the DestroySchema command.
	CommandType_DestroySchema = FdoCommandType_DestroySchema,

    /// Represents the ActivateSpatialContext command.
	CommandType_ActivateSpatialContext = FdoCommandType_ActivateSpatialContext,

    /// Represents the CreateSpatialContext command.
	CommandType_CreateSpatialContext = FdoCommandType_CreateSpatialContext,

    /// Represents the DestroySpatialContext command.
	CommandType_DestroySpatialContext = FdoCommandType_DestroySpatialContext,

    /// Represents the GetSpatialContexts command.
	CommandType_GetSpatialContexts = FdoCommandType_GetSpatialContexts,

    /// Represents the CreateMeasureUnit command.
	CommandType_CreateMeasureUnit = FdoCommandType_CreateMeasureUnit,

    /// Represents the DestroyMeasureUnit command.
	CommandType_DestroyMeasureUnit = FdoCommandType_DestroyMeasureUnit,

    /// Represents the GetMeasureUnits command.
	CommandType_GetMeasureUnits = FdoCommandType_GetMeasureUnits,

    /// Represents the SQLCommand command.
	CommandType_SQLCommand = FdoCommandType_SQLCommand,

    /// Represents the AcquireLock command.
	CommandType_AcquireLock = FdoCommandType_AcquireLock,

    /// Represents the GetLockInfo command.
	CommandType_GetLockInfo = FdoCommandType_GetLockInfo,

    /// Represents the GetLockedObjects command.
	CommandType_GetLockedObjects = FdoCommandType_GetLockedObjects,

    /// Represents the GetLockOwners command.
	CommandType_GetLockOwners = FdoCommandType_GetLockOwners,

    /// Represents the ReleaseLock command.
	CommandType_ReleaseLock = FdoCommandType_ReleaseLock,

    /// Represents the ActivateLongTransaction command.
	CommandType_ActivateLongTransaction = FdoCommandType_ActivateLongTransaction,

    /// Represents the DeactivateLongTransaction command.
	CommandType_DeactivateLongTransaction = FdoCommandType_DeactivateLongTransaction,

    /// Represents the CommitLongTransaction command.
	CommandType_CommitLongTransaction = FdoCommandType_CommitLongTransaction,

    /// Represents the CreateLongTransaction command.
	CommandType_CreateLongTransaction = FdoCommandType_CreateLongTransaction,

    /// Represents the GetLongTransactions command.
	CommandType_GetLongTransactions = FdoCommandType_GetLongTransactions,

    /// Represents the FreezeLongTransaction command.
	CommandType_FreezeLongTransaction = FdoCommandType_FreezeLongTransaction,

    /// Represents the RollbackLongTransaction command.
	CommandType_RollbackLongTransaction = FdoCommandType_RollbackLongTransaction,

    /// Represents the ActivateLongTransactionCheckpoint command.
	CommandType_ActivateLongTransactionCheckpoint = FdoCommandType_ActivateLongTransactionCheckpoint,

    /// Represents the CreateLongTransactionCheckpoint command.
	CommandType_CreateLongTransactionCheckpoint = FdoCommandType_CreateLongTransactionCheckpoint,

    /// Represents the GetLongTransactionCheckpoints command.
	CommandType_GetLongTransactionCheckpoints = FdoCommandType_GetLongTransactionCheckpoints,

    /// Represents the RollbackLongTransactionCheckpoint command.
	CommandType_RollbackLongTransactionCheckpoint = FdoCommandType_RollbackLongTransactionCheckpoint,

    /// Represents the ChangeLongTransactionPrivileges command.
	CommandType_ChangeLongTransactionPrivileges = FdoCommandType_ChangeLongTransactionPrivileges,

    /// Represents the GetLongTransactionPrivileges command.
	CommandType_GetLongTransactionPrivileges = FdoCommandType_GetLongTransactionPrivileges,

    /// Represents the ChangeLongTransactionSet command.
	CommandType_ChangeLongTransactionSet = FdoCommandType_ChangeLongTransactionSet,

    /// Represents the GetLongTransactionsInSet command.
	CommandType_GetLongTransactionsInSet = FdoCommandType_GetLongTransactionsInSet,

    /// Represents the NetworkShortestPath command.
	CommandType_NetworkShortestPath = FdoCommandType_NetworkShortestPath,

    /// Represents the NetworkAllPaths command.
	CommandType_NetworkAllPaths = FdoCommandType_NetworkAllPaths,

    /// Represents the NetworkReachingNodes command.
	CommandType_NetworkReachingNodes = FdoCommandType_NetworkReachingNodes,

    /// Represents the NetworkReachableNodes command.
	CommandType_NetworkReachableNodes = FdoCommandType_NetworkReachableNodes,

    /// Represents the NetworkNearestNeighbors command.
	CommandType_NetworkNearestNeighbors = FdoCommandType_NetworkNearestNeighbors,

    /// Represents the NetworkWithinCost command.
	CommandType_NetworkWithinCost = FdoCommandType_NetworkWithinCost,

    /// Represents the NetworkTSP command.
	CommandType_NetworkTSP = FdoCommandType_NetworkTSP,

    /// Represents the ActivateTopologyArea command.
	CommandType_ActivateTopologyArea = FdoCommandType_ActivateTopologyArea,

    /// Represents the DeactivateTopologyArea command.
	CommandType_DeactivateTopologyArea = FdoCommandType_DeactivateTopologyArea,

    /// Represents the ActivateTopologyInCommandResult command.
	CommandType_ActivateTopologyInCommandResult = FdoCommandType_ActivateTopologyInCommandResult,

    /// Represents the DeactivateTopologyInCommandResults command.
	CommandType_DeactivateTopologyInCommandResults = FdoCommandType_DeactivateTopologyInCommandResults,

    /// Represents the SelectAggregate command.
	CommandType_SelectAggregates = FdoCommandType_SelectAggregates,

    /// Represents the CreateDataStore command.
    CommandType_CreateDataStore = FdoCommandType_CreateDataStore,

    /// Represents the DestroyDataStore command.
    CommandType_DestroyDataStore = FdoCommandType_DestroyDataStore,

    /// Represents the ListDataStores command.
	CommandType_ListDataStores = FdoCommandType_ListDataStores,

    /// Represents the index of the first provider specific command.
	CommandType_FirstProviderCommand = FdoCommandType_FirstProviderCommand
};

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_FDO_COMMANDS
/// \endcond


