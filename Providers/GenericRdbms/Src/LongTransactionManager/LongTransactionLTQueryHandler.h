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
// | The file contains the definition of the class FdoRdbmsLongTransactionLt-
// | QueryHandler. The class defines the interfaces and services the Long Trans-
// | action LT Query handler provides.
// =============================================================================


#ifndef FdoRdbmsLongTransactionLtQueryHandler_h
#define FdoRdbmsLongTransactionLtQueryHandler_h

#ifdef _WIN32
#pragma once
#endif

#include <FdoCommonThreadMutex.h>
#include <Inc/Nls/fdordbms_msg.h>
#include "FdoRdbmsUtil.h"
#include "FdoRdbmsException.h"
#include "Disposable.h"
#include "LongTransactionInfo.h"
#include "LongTransactionDataRequestType.h"
#include "LongTransactionManagerService.h"
#include "GdbiQueryResult.h"
#include "GdbiConnection.h"
#include "GdbiStatement.h"
#include "../LockManager/LockManager.h"


class FdoRdbmsLongTransactionLtQueryHandler : public FdoLtmDisposable
{

    public:

    // ************************************************************************
    // *                           Public Interfaces                          *
    // ************************************************************************

    //  Close:
    //      Function to close the long transaction query handler.

    virtual void Close () = 0;

    //  GetCreationDate:
    //      Function to get creation date for the current long transaction.

    virtual FdoDateTime GetCreationDate () = 0;

    //  GetDescription:
    //      Function to get the description for the current long transaction.

    virtual FdoString *GetDescription () = 0;

    //  GetFreezeMode:
    //      Function to get the freeze mode for the current long transaction.

    virtual FdoString *GetFreezeMode () = 0;

    //  GetFreezeOwner:
    //      Function to get the freeze owner for the current long transaction.

    virtual FdoString *GetFreezeOwner () = 0;

    //  GetFreezeWriter:
    //      Function to get the freeze writer for the current long transaction.

    virtual FdoString *GetFreezeWriter () = 0;

    //  GetIsActiveFlag:
    //      Function to retrieve the value of the flag indicating whether or
    //      not the current long transaction is active.

    virtual bool GetIsActiveFlag () = 0;

    //  GetIsRefreshedFlag:
    //      Function to retrieve the value of the flag indicating whether or
    //      not the current long transaction represents a continously refreshed
    //      one.

    virtual bool GetIsRefreshedFlag () = 0;

    //  GetLtId:
    //      Function to get the long transaction id for the current long trans-
    //      action.

    virtual FdoInt64 GetLtId () = 0;

    //  GetName:
    //      Function to get the name for the current long transaction.

    virtual FdoString *GetName () = 0;

    //  GetOwner:
    //      Function to get the owner of the current long transaction.

    virtual FdoString *GetOwner () = 0;

    //  ReadNext:
    //      Function to get the next long transaction from the GDBI buffer.

    virtual bool ReadNext () = 0;


    protected:

    // ************************************************************************
    // *                      Protected Member Variables                      *
    // ************************************************************************

    //  gdbi_connection:
    //      A reference to the necessary connection used to execute the re-
    //      quested query.

    GdbiConnection *gdbi_connection;

    //  lock_manager:
    //      A reference to the Lock Manager required to handle lock related
    //      operations like applying a persistent lock on a long transaction.

    FdoRdbmsLockManager *lock_manager;

    //  long_transaction_manager_service:
    //      The variable keeps a reference to the currently valid long transac-
    //      tion manager service.

    FdoRdbmsLongTransactionManagerService  *long_transaction_manager_service;

    //  lt_info:
    //      The variable holds a reference to the data structure used to
    //      store values returned by the query in.

    FdoRdbmsLongTransactionInfo *lt_info;


};  //  class FdoRdbmsLongTransactionLtQueryHandler

#endif

