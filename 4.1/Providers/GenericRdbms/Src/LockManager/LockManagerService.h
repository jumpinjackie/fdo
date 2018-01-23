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

#ifndef FdoRdbmsLockManagerService_h
#define FdoRdbmsLockManagerService_h

#ifdef _WIN32
#pragma once
#endif

#include <FdoCommonThreadMutex.h>
#include "Disposable.h"
#include <Sm/Ph/ColumnCollection.h>

class FdoRdbmsLockManagerService : public FdoLkDisposable
{

    public:
		virtual void	   Activate() = 0;
		virtual FdoInt64   GetLockId(wchar_t *dbUser) = 0;
		virtual FdoStringP SelectStmt(const FdoSmPhColumnCollection *colList, 
									  FdoString *tableName, FdoString *whereClause) = 0;
		virtual FdoStringP CreateConflictDbObject(const FdoSmPhColumnCollection *colList, 
												  FdoString *dbUser, FdoString *tableName, 
												  FdoString *subQuery) = 0;
		virtual FdoStringP GetConflictDbObjectName(FdoString *tableName) = 0;
		virtual FdoStringP SelectAllStmt(const FdoSmPhColumnCollection *colList, FdoString *tableName, FdoString *whereClause, bool updHint = false) = 0;
		virtual FdoStringP	   DropStmt(FdoString *tableName) = 0;
		virtual FdoStringP UpdateLockStmt(const FdoSmPhColumnCollection *colList, FdoString *tableName, 
			                              FdoString *whereClause, FdoString *subqryObject, FdoLockType lockType) = 0;
		virtual	FdoStringP SelectLockUsed(FdoInt64 lockId) = 0;
		virtual	FdoStringP SelectLockedObjects(const FdoSmPhColumnCollection *colList,FdoString *tableName, 
											   FdoString *dbName, FdoInt64 lockId) = 0;
		virtual void       InsertLockIdTable (FdoString *tableName) = 0;

		virtual FdoStringP SelectAllOwners() = 0;

		virtual void LockLt(FdoString *ltName) = 0;
		virtual void UnlockLt(FdoString *ltName) = 0;
		virtual FdoStringP BuildTabName(FdoString *dbName, FdoString *tableName) = 0;
		virtual bool IsTableEmpty(FdoString *tabName) = 0;
		virtual bool IsLockTable(FdoString *tabName) = 0;
};

#endif

	
