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

