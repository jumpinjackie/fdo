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

// =============================================================================
// | The file contains the definition of the class FdoRdbmsLockManager. The 
// | class defines the interfaces a provider has to implement to support per-
// | sistent locking.
// =============================================================================


#ifndef FdoRdbmsLockManager_h
#define FdoRdbmsLockManager_h

#ifdef _WIN32
#pragma once
#endif

#include <vector>
#include <FdoCommonThreadMutex.h>
#include "Disposable.h"
#include "LockConstants.h"
#include "LockStrategy.h"
#include "LockTypeEnum.h"
#include "LockTypes.h"
#include "Operation.h"
#include "LockInfoQueryHandler.h"
#include "LockOwnersQueryHandler.h"
#include "LockTableQueryHandler.h"
#include "LockConflictQueryHandler.h"
#include "Sm/Lp/ClassDefinition.h"

using namespace std ;

class FdoRdbmsLockConflictQueryHandler;

class FdoRdbmsLockManager : public FdoLkDisposable
{

    public:

    // Class Functions:
    //  CheckConflicts:
    //      The function checks for lock conflicts for the specified set of
    //      objects and the specified class.
    //  LockedInfoQuery:
    //      The function retrieves lock information for the specified set of
    //      objects.
    //  LockedObjectsQuery:
    //      The function retrieves all objects in the current data store the
    //      named FDO user has applied a persistent lock on.
    //  OwnersQuery:
    //      The function retrieves the lock owners.
    //  TableQuery:
    //      The function retrieves the name of all tables in which the named
    //      FDO user has applied a persistent lock on objects.
    //  UnlockData
    //      The function unlocks the specified data.
	//  LockLt
    //      The function locks the specified long transaction.
	//  UnlockLt
    //      The function unlocks the specified long transaction.


    virtual void CheckConflicts         (
                                FdoSmLpClassDefinition *class_definition,
                                FdoString              *filter_sql,
                                FdoRdbmsOperation      operation,
                                FdoString              *view_name)       = 0;
    
    virtual int UnlockData             (
                                FdoString			   *tableName,
                                FdoString              *filter_sql,
                                FdoString              *confObject)       = 0;

	virtual int AcquireLock		(FdoString *tableName, FdoString *sqlFilter, FdoString *confObject, 
		                                       FdoLockType lockType) =0;
	
    virtual FdoRdbmsLockInfoQueryHandler  *LockedInfoQuery    (FdoString *classname, FdoString *sqlFilter)= 0;

	virtual FdoRdbmsLockConflictQueryHandler *LockConflictQuery  (FdoString *tableName, FdoString *sqlFilter)= 0;

    virtual FdoRdbmsLockInfoQueryHandler  *LockedObjectsQuery ( FdoInt64 lockId, FdoString *dbName, FdoString *tableName)        = 0;

    virtual FdoRdbmsLockTableQueryHandler *TableQuery (FdoString              *fdo_user)        = 0;

    virtual FdoRdbmsLockOwnersQueryHandler *OwnersQuery       ()         = 0;

	virtual void LockLt(FdoString *lt_name) = 0;

	virtual void UnlockLt(FdoString *lt_name) = 0;

	virtual FdoRdbmsLockConflictQueryHandler *TLockRequest(std::vector<FdoString*> dbVector, std::vector<FdoString*> tableVector, 
		                                             FdoString *sqlFilter, bool *foundConflict) = 0;

    protected:

		FdoRdbmsLockManager          () {};
		virtual ~FdoRdbmsLockManager () {};


};  //  class FdoRdbmsLockManager

typedef FdoPtr<FdoRdbmsLockManager> FdoRdbmsLockManagerP;

#endif

