
// ExerciseFdoCapabilities.cpp
// 
//
// 
// Copyright (C) 2008  Autodesk, Inc.
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
//

#include "stdafx.h"
#include "ExerciseFdoCapabilities.h"

ExerciseFdoCapabilities::ExerciseFdoCapabilities(FdoIConnection * connection) {
	this->connection = connection;
	fdoCommandType_Select = false;
	fdoCommandType_Insert = false;
	fdoCommandType_Delete = false;
	fdoCommandType_Update = false;
	fdoCommandType_DescribeSchema = false;
	fdoCommandType_DescribeSchemaMapping = false;
	fdoCommandType_ApplySchema = false;
	fdoCommandType_DestroySchema = false;
	fdoCommandType_ActivateSpatialContext = false;
	fdoCommandType_CreateSpatialContext = false;
	fdoCommandType_DestroySpatialContext = false;
	fdoCommandType_GetSpatialContexts = false;
	fdoCommandType_CreateMeasureUnit = false;
	fdoCommandType_DestroyMeasureUnit = false;
	fdoCommandType_GetMeasureUnits = false;
	fdoCommandType_SQLCommand = false;
	fdoCommandType_AcquireLock = false;
	fdoCommandType_GetLockInfo = false;
	fdoCommandType_GetLockedObjects = false;
	fdoCommandType_GetLockOwners = false;
	fdoCommandType_ReleaseLock = false;
	fdoCommandType_ActivateLongTransaction = false;
	fdoCommandType_DeactivateLongTransaction = false;
	fdoCommandType_CommitLongTransaction = false;
	fdoCommandType_CreateLongTransaction = false;
	fdoCommandType_GetLongTransactions = false;
	fdoCommandType_FreezeLongTransaction = false;
	fdoCommandType_RollbackLongTransaction = false;
	fdoCommandType_ActivateLongTransactionCheckpoint = false;
	fdoCommandType_CreateLongTransactionCheckpoint = false;
	fdoCommandType_GetLongTransactionCheckpoints = false;
	fdoCommandType_RollbackLongTransactionCheckpoint = false;
	fdoCommandType_ChangeLongTransactionPrivileges = false;
	fdoCommandType_GetLongTransactionPrivileges = false;
	fdoCommandType_ChangeLongTransactionSet = false;
	fdoCommandType_GetLongTransactionsInSet = false;
	fdoCommandType_NetworkShortestPath = false;
	fdoCommandType_NetworkAllPaths = false;
	fdoCommandType_NetworkReachableNodes = false;
	fdoCommandType_NetworkReachingNodes = false;
	fdoCommandType_NetworkNearestNeighbors = false;
	fdoCommandType_NetworkWithinCost = false;
	fdoCommandType_NetworkTSP = false;
	fdoCommandType_ActivateTopologyArea = false;
	fdoCommandType_DeactivateTopologyArea = false;
	fdoCommandType_ActivateTopologyInCommandResult = false;
	fdoCommandType_DeactivateTopologyInCommandResults = false;
	fdoCommandType_SelectAggregates = false;

	commandCapabilities = connection->GetCommandCapabilities();
	SetCommandCapabilities();

}

void
ExerciseFdoCapabilities::SetCommandCapabilities() {
	FdoInt32 numCommandTypes = 0;
	FdoInt32 * commandTypes;
	commandTypes = commandCapabilities->GetCommands(numCommandTypes);
	for (int i = 0; i < numCommandTypes; i++) {
		switch (commandTypes[i]) {
			case FdoCommandType_Select:
				fdoCommandType_Select = true;
				break;
			case FdoCommandType_Insert:
				fdoCommandType_Insert = true;
				break;
			case FdoCommandType_Delete:
				fdoCommandType_Delete = true;
				break;
			case FdoCommandType_Update:
				fdoCommandType_Update = true;
				break;
			case FdoCommandType_DescribeSchema:
				fdoCommandType_DescribeSchema = true;
				break;
			case FdoCommandType_DescribeSchemaMapping:
				fdoCommandType_DescribeSchemaMapping = true;
				break;
			case FdoCommandType_ApplySchema:
				fdoCommandType_ApplySchema = true;
				break;
			case FdoCommandType_DestroySchema:
				fdoCommandType_DestroySchema = true;
				break;
			case FdoCommandType_ActivateSpatialContext:
				fdoCommandType_ActivateSpatialContext = true;
				break;
			case FdoCommandType_CreateSpatialContext:
				fdoCommandType_CreateSpatialContext = true;
				break;
			case FdoCommandType_DestroySpatialContext:
				fdoCommandType_DestroySpatialContext = true;
				break;
			case FdoCommandType_GetSpatialContexts:
				fdoCommandType_GetSpatialContexts = true;
				break;
			case FdoCommandType_CreateMeasureUnit:
				fdoCommandType_CreateMeasureUnit = true;
				break;
			case FdoCommandType_DestroyMeasureUnit:
				fdoCommandType_DestroyMeasureUnit = true;
				break;
			case FdoCommandType_GetMeasureUnits:
				fdoCommandType_GetMeasureUnits = true;
				break;
			case FdoCommandType_SQLCommand:
				fdoCommandType_SQLCommand = true;
				break;
			case FdoCommandType_AcquireLock:
				fdoCommandType_AcquireLock = true;
				break;
			case FdoCommandType_GetLockInfo:
				fdoCommandType_GetLockInfo = true;
				break;
			case FdoCommandType_GetLockedObjects:
				fdoCommandType_GetLockedObjects = true;
				break;
			case FdoCommandType_GetLockOwners:
				fdoCommandType_GetLockOwners = true;
				break;
			case FdoCommandType_ReleaseLock:
				fdoCommandType_ReleaseLock = true;
				break;
			case FdoCommandType_ActivateLongTransaction:
				fdoCommandType_ActivateLongTransaction = true;
				break;
			case FdoCommandType_DeactivateLongTransaction:
				fdoCommandType_DeactivateLongTransaction = true;
				break;
			case FdoCommandType_CommitLongTransaction:
				fdoCommandType_CommitLongTransaction = true;
				break;
			case FdoCommandType_CreateLongTransaction:
				fdoCommandType_CreateLongTransaction = true;
				break;
			case FdoCommandType_GetLongTransactions:
				fdoCommandType_GetLongTransactions = true;
				break;
			case FdoCommandType_FreezeLongTransaction:
				fdoCommandType_FreezeLongTransaction = true;
				break;
			case FdoCommandType_RollbackLongTransaction:
				fdoCommandType_RollbackLongTransaction = true;
				break;
			case FdoCommandType_ActivateLongTransactionCheckpoint:
				fdoCommandType_ActivateLongTransactionCheckpoint = true;
				break;
			case FdoCommandType_CreateLongTransactionCheckpoint:
				fdoCommandType_CreateLongTransactionCheckpoint = true;
				break;
			case FdoCommandType_GetLongTransactionCheckpoints:
				fdoCommandType_GetLongTransactionCheckpoints = true;
				break;
			case FdoCommandType_RollbackLongTransactionCheckpoint:
				fdoCommandType_RollbackLongTransactionCheckpoint = true;
				break;
			case FdoCommandType_ChangeLongTransactionPrivileges:
				fdoCommandType_ChangeLongTransactionPrivileges = true;
				break;
			case FdoCommandType_GetLongTransactionPrivileges:
				fdoCommandType_GetLongTransactionPrivileges = true;
				break;
			case FdoCommandType_ChangeLongTransactionSet:
				fdoCommandType_ChangeLongTransactionSet = true;
				break;
			case FdoCommandType_GetLongTransactionsInSet:
				fdoCommandType_GetLongTransactionsInSet = true;
				break;
			case FdoCommandType_NetworkShortestPath:
				fdoCommandType_NetworkShortestPath = true;
				break;
			case FdoCommandType_NetworkAllPaths:
				fdoCommandType_NetworkAllPaths = true;
				break;
			case FdoCommandType_NetworkReachableNodes:
				fdoCommandType_NetworkReachableNodes = true;
				break;
			case FdoCommandType_NetworkReachingNodes:
				fdoCommandType_NetworkReachingNodes = true;
				break;
			case FdoCommandType_NetworkNearestNeighbors:
				fdoCommandType_NetworkNearestNeighbors = true;
				break;
			case FdoCommandType_NetworkWithinCost:
				fdoCommandType_NetworkWithinCost = true;
				break;
			case FdoCommandType_NetworkTSP:
				fdoCommandType_NetworkTSP = true;
				break;
			case FdoCommandType_ActivateTopologyArea:
				fdoCommandType_ActivateTopologyArea = true;
				break;
			case FdoCommandType_DeactivateTopologyArea:
				fdoCommandType_DeactivateTopologyArea = true;
				break;
			case FdoCommandType_ActivateTopologyInCommandResult:
				fdoCommandType_ActivateTopologyInCommandResult = true;
				break;
			case FdoCommandType_DeactivateTopologyInCommandResults:
				fdoCommandType_DeactivateTopologyInCommandResults = true;
				break;
			case FdoCommandType_SelectAggregates:
				fdoCommandType_SelectAggregates = true;
				break;
			default:
				sprintf(errString, "ERROR unknown FdoCommandType %d", commandTypes[i]);
				throw(errString);
				}
	}
}


bool
ExerciseFdoCapabilities::SupportsSelect() {
	return fdoCommandType_Select;
}

bool
ExerciseFdoCapabilities::SupportsInsert() {
	return fdoCommandType_Insert;
}

bool
ExerciseFdoCapabilities::SupportsDelete() {
	return fdoCommandType_Delete;
}

bool
ExerciseFdoCapabilities::SupportsUpdate() {
	return fdoCommandType_Update;
}

bool
ExerciseFdoCapabilities::SupportsDescribeSchema() {
	return fdoCommandType_DescribeSchema;
}

bool
ExerciseFdoCapabilities::SupportsDescribeSchemaMapping() {
	return fdoCommandType_DescribeSchemaMapping;
}

bool
ExerciseFdoCapabilities::SupportsApplySchema() {
	return fdoCommandType_ApplySchema;
}

bool
ExerciseFdoCapabilities::SupportsDestroySchema() {
	return fdoCommandType_DestroySchema;
}

bool
ExerciseFdoCapabilities::SupportsActivateSpatialContext() {
	return fdoCommandType_ActivateSpatialContext;
}

bool
ExerciseFdoCapabilities::SupportsCreateSpatialContext() {
	return fdoCommandType_CreateSpatialContext;
}

bool
ExerciseFdoCapabilities::SupportsDestroySpatialContext() {
	return fdoCommandType_DestroySpatialContext;
}

bool
ExerciseFdoCapabilities::SupportsGetSpatialContexts() {
	return fdoCommandType_GetSpatialContexts;
}

bool
ExerciseFdoCapabilities::SupportsCreateMeasureUnit() {
	return fdoCommandType_CreateMeasureUnit;
}

bool
ExerciseFdoCapabilities::SupportsDestroyMeasureUnit() {
	return fdoCommandType_DestroyMeasureUnit;
}

bool
ExerciseFdoCapabilities::SupportsGetMeasureUnits() {
	return fdoCommandType_GetMeasureUnits;
}

bool
ExerciseFdoCapabilities::SupportsSQLCommand() {
	return fdoCommandType_SQLCommand;
}

bool
ExerciseFdoCapabilities::SupportsAcquireLock() {
	return fdoCommandType_AcquireLock;
}

bool
ExerciseFdoCapabilities::SupportsGetLockInfo() {
	return fdoCommandType_GetLockInfo;
}

bool
ExerciseFdoCapabilities::SupportsGetLockedObjects() {
	return fdoCommandType_GetLockedObjects;
}

bool
ExerciseFdoCapabilities::SupportsGetLockOwners() {
	return fdoCommandType_GetLockOwners;
}

bool
ExerciseFdoCapabilities::SupportsReleaseLock() {
	return fdoCommandType_ReleaseLock;
}

bool
ExerciseFdoCapabilities::SupportsActivateLongTransaction() {
	return fdoCommandType_ActivateLongTransaction;
}

bool
ExerciseFdoCapabilities::SupportsDeactivateLongTransaction() {
	return fdoCommandType_DeactivateLongTransaction;
}

bool
ExerciseFdoCapabilities::SupportsCommitLongTransaction() {
	return fdoCommandType_CommitLongTransaction;
}

bool
ExerciseFdoCapabilities::SupportsCreateLongTransaction() {
	return fdoCommandType_CreateLongTransaction;
}

bool
ExerciseFdoCapabilities::SupportsGetLongTransactions() {
	return fdoCommandType_GetLongTransactions;
}

bool
ExerciseFdoCapabilities::SupportsFreezeLongTransaction() {
	return fdoCommandType_FreezeLongTransaction;
}

bool
ExerciseFdoCapabilities::SupportsRollbackLongTransaction() {
	return fdoCommandType_RollbackLongTransaction;
}

bool
ExerciseFdoCapabilities::SupportsActivateLongTransactionCheckpoint() {
	return fdoCommandType_ActivateLongTransactionCheckpoint;
}

bool
ExerciseFdoCapabilities::SupportsCreateLongTransactionCheckpoint() {
	return fdoCommandType_CreateLongTransactionCheckpoint;
}

bool
ExerciseFdoCapabilities::SupportsGetLongTransactionCheckpoints() {
	return fdoCommandType_GetLongTransactionCheckpoints;
}

bool
ExerciseFdoCapabilities::SupportsRollbackLongTransactionCheckpoint() {
	return fdoCommandType_RollbackLongTransactionCheckpoint;
}

bool
ExerciseFdoCapabilities::SupportsChangeLongTransactionPrivileges() {
	return fdoCommandType_ChangeLongTransactionPrivileges;
}

bool
ExerciseFdoCapabilities::SupportsGetLongTransactionPrivileges() {
	return fdoCommandType_GetLongTransactionPrivileges;
}

bool
ExerciseFdoCapabilities::SupportsChangeLongTransactionSet() {
	return fdoCommandType_ChangeLongTransactionSet;
}

bool
ExerciseFdoCapabilities::SupportsGetLongTransactionsInSet() {
	return fdoCommandType_GetLongTransactionsInSet;
}

bool
ExerciseFdoCapabilities::SupportsNetworkShortestPath() {
	return fdoCommandType_NetworkShortestPath;
}

bool
ExerciseFdoCapabilities::SupportsNetworkAllPaths() {
	return fdoCommandType_NetworkAllPaths;
}

bool
ExerciseFdoCapabilities::SupportsNetworkReachableNodes() {
	return fdoCommandType_NetworkReachableNodes;
}

bool
ExerciseFdoCapabilities::SupportsNetworkReachingNodes() {
	return fdoCommandType_NetworkReachingNodes;
}

bool
ExerciseFdoCapabilities::SupportsNetworkNearestNeighbors() {
	return fdoCommandType_NetworkNearestNeighbors;
}

bool
ExerciseFdoCapabilities::SupportsNetworkWithinCost() {
	return fdoCommandType_NetworkWithinCost;
}

bool
ExerciseFdoCapabilities::SupportsNetworkTSP() {
	return fdoCommandType_NetworkTSP;
}

bool
ExerciseFdoCapabilities::SupportsActivateTopologyArea() {
	return fdoCommandType_ActivateTopologyArea;
}

bool
ExerciseFdoCapabilities::SupportsDeactivateTopologyArea() {
	return fdoCommandType_DeactivateTopologyArea;
}

bool
ExerciseFdoCapabilities::SupportsActivateTopologyInCommandResult() {
	return fdoCommandType_ActivateTopologyInCommandResult;
}

bool
ExerciseFdoCapabilities::SupportsDeactivateTopologyInCommandResults() {
	return fdoCommandType_DeactivateTopologyInCommandResults;
}

bool
ExerciseFdoCapabilities::SupportsSelectAggregates() {
	return fdoCommandType_SelectAggregates;
}
