/******************************************************************************
 * (C) Copyright 2003 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 * 
  *
 ******************************************************************************/

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

	
