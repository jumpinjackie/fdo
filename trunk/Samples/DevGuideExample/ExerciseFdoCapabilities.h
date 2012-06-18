// ExerciseFdoCapabilities.h

#ifndef _EXERCISEFDOCAPABILITIES_H_
#define _EXERCISEFDOCAPABILITIES_H_
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

#include <Fdo.h>
#include <cstring>

class ExerciseFdoCapabilities {
public:
	ExerciseFdoCapabilities(FdoIConnection * connection);
	bool SupportsSelect();
	bool SupportsInsert();
	bool SupportsDelete();
	bool SupportsUpdate();
	bool SupportsDescribeSchema();
	bool SupportsDescribeSchemaMapping();
	bool SupportsApplySchema();
	bool SupportsDestroySchema();
	bool SupportsActivateSpatialContext();
	bool SupportsCreateSpatialContext();
	bool SupportsDestroySpatialContext();
	bool SupportsGetSpatialContexts();
	bool SupportsCreateMeasureUnit();
	bool SupportsDestroyMeasureUnit();
	bool SupportsGetMeasureUnits();
	bool SupportsSQLCommand();
	bool SupportsAcquireLock();
	bool SupportsGetLockInfo();
	bool SupportsGetLockedObjects();
	bool SupportsGetLockOwners();
	bool SupportsReleaseLock();
	bool SupportsActivateLongTransaction();
	bool SupportsDeactivateLongTransaction();
	bool SupportsCommitLongTransaction();
	bool SupportsCreateLongTransaction();
	bool SupportsGetLongTransactions();
	bool SupportsFreezeLongTransaction();
	bool SupportsRollbackLongTransaction();
	bool SupportsActivateLongTransactionCheckpoint();
	bool SupportsCreateLongTransactionCheckpoint();
	bool SupportsGetLongTransactionCheckpoints();
	bool SupportsRollbackLongTransactionCheckpoint();
	bool SupportsChangeLongTransactionPrivileges();
	bool SupportsGetLongTransactionPrivileges();
	bool SupportsChangeLongTransactionSet();
	bool SupportsGetLongTransactionsInSet();
	bool SupportsNetworkShortestPath();
	bool SupportsNetworkAllPaths();
	bool SupportsNetworkReachableNodes();
	bool SupportsNetworkReachingNodes();
	bool SupportsNetworkNearestNeighbors();
	bool SupportsNetworkWithinCost();
	bool SupportsNetworkTSP();
	bool SupportsActivateTopologyArea();
	bool SupportsDeactivateTopologyArea();
	bool SupportsActivateTopologyInCommandResult();
	bool SupportsDeactivateTopologyInCommandResults();
	bool SupportsSelectAggregates();


	bool SupportsCreateSpatialIndex();
	bool SupportsDestroySpatialIndex();
	bool SupportsGetSpatialIndexes();

private:
	char errString[256];
	FdoIConnection * connection;
	FdoPtr<FdoICommandCapabilities> commandCapabilities;
	void SetCommandCapabilities();
	bool fdoCommandType_Select;
	bool fdoCommandType_Insert;
	bool fdoCommandType_Delete;
	bool fdoCommandType_Update;
	bool fdoCommandType_DescribeSchema;
	bool fdoCommandType_DescribeSchemaMapping;
	bool fdoCommandType_ApplySchema;
	bool fdoCommandType_DestroySchema;
	bool fdoCommandType_ActivateSpatialContext;
	bool fdoCommandType_CreateSpatialContext;
	bool fdoCommandType_DestroySpatialContext;
	bool fdoCommandType_GetSpatialContexts;
	bool fdoCommandType_CreateMeasureUnit;
	bool fdoCommandType_DestroyMeasureUnit;
	bool fdoCommandType_GetMeasureUnits;
	bool fdoCommandType_SQLCommand;
	bool fdoCommandType_AcquireLock;
	bool fdoCommandType_GetLockInfo;
	bool fdoCommandType_GetLockedObjects;
	bool fdoCommandType_GetLockOwners;
	bool fdoCommandType_ReleaseLock;
	bool fdoCommandType_ActivateLongTransaction;
	bool fdoCommandType_DeactivateLongTransaction;
	bool fdoCommandType_CommitLongTransaction;
	bool fdoCommandType_CreateLongTransaction;
	bool fdoCommandType_GetLongTransactions;
	bool fdoCommandType_FreezeLongTransaction;
	bool fdoCommandType_RollbackLongTransaction;
	bool fdoCommandType_ActivateLongTransactionCheckpoint;
	bool fdoCommandType_CreateLongTransactionCheckpoint;
	bool fdoCommandType_GetLongTransactionCheckpoints;
	bool fdoCommandType_RollbackLongTransactionCheckpoint;
	bool fdoCommandType_ChangeLongTransactionPrivileges;
	bool fdoCommandType_GetLongTransactionPrivileges;
	bool fdoCommandType_ChangeLongTransactionSet;
	bool fdoCommandType_GetLongTransactionsInSet;
	bool fdoCommandType_NetworkShortestPath;
	bool fdoCommandType_NetworkAllPaths;
	bool fdoCommandType_NetworkReachableNodes;
	bool fdoCommandType_NetworkReachingNodes;
	bool fdoCommandType_NetworkNearestNeighbors;
	bool fdoCommandType_NetworkWithinCost;
	bool fdoCommandType_NetworkTSP;
	bool fdoCommandType_ActivateTopologyArea;
	bool fdoCommandType_DeactivateTopologyArea;
	bool fdoCommandType_ActivateTopologyInCommandResult;
	bool fdoCommandType_DeactivateTopologyInCommandResults;
	bool fdoCommandType_SelectAggregates;
};

#endif

