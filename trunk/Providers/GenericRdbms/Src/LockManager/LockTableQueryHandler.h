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
// | The file contains the definition of the class FdoRdbmsLockInfoQueryHandler.
// | The class defines the interfaces and services the Lock Info Query handler
// | provides.
// =============================================================================


#ifndef FdoRdbmsLockTableQueryHandler_h
#define FdoRdbmsLockTableQueryHandler_h

#ifdef _WIN32
#pragma once
#endif

#include <FdoCommonThreadMutex.h>
#include <Inc/Nls/fdordbms_msg.h>
#include "FdoRdbmsUtil.h"
#include "FdoRdbmsException.h"
#include "Disposable.h"
#include "LockConstants.h"
#include "TableInfo.h"
#include "GdbiQueryResult.h"
#include "GdbiConnection.h"
#include "GdbiStatement.h"


class FdoRdbmsLockTableQueryHandler : public FdoLkDisposable
{

    public:

    // Class Functions:
    //  Close:
    //      Function to close the query handler.
    //  Get*:
    //      Functions to get the values of the class variables.
    //  ReadNext:
    //      Function to retrieve the next data set. This routine also initiates
    //      the query on its initial call.
    //  Set*:
    //      Functions to set the class variable values.

    virtual     bool ReadNext          () = 0;

    virtual      void Close             () = 0;

    virtual FdoString *GetDataStoreName () = 0;
    virtual FdoString *GetTableName     () = 0;
	virtual FdoInt64   GetLockId		() = 0;


    //protected:

    //// Class Functions:
    ////  FdoRdbmsLockTableQueryHandler:
    ////      Class constructor.
    ////  ~FdoRdbmsLockTableQueryHandler:
    ////      Class destructor.

    //    FdoRdbmsLockTableQueryHandler          ();
    //    virtual ~FdoRdbmsLockTableQueryHandler ();


};  //  class FdoRdbmsLockTableQueryHandler

#endif

