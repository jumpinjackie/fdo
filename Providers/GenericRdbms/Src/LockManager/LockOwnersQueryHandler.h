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
// | The file contains the definition of the class FdoRdbmsLockOwnersQuery-
// | Handler. The class defines the interfaces and services the Lock Owners
// | Query Handler provides.
// =============================================================================


#ifndef FdoRdbmsLockOwnersQueryHandler_h
#define FdoRdbmsLockOwnersQueryHandler_h

#ifdef _WIN32
#pragma once
#endif

#include <FdoCommonThreadMutex.h>
#include <Inc/Nls/fdordbms_msg.h>
#include "FdoRdbmsUtil.h"
#include "FdoRdbmsException.h"
#include "Disposable.h"
#include "LockConstants.h"
#include "OwnerInfo.h"
#include "GdbiQueryResult.h"
#include "GdbiConnection.h"
#include "GdbiStatement.h"


class FdoRdbmsLockOwnersQueryHandler : public FdoLkDisposable
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

    virtual bool      ReadNext      () = 0;

    virtual void      Close         () = 0;

    virtual FdoString *GetLockOwner () = 0;


};  //  class FdoRdbmsLockOwnersQueryHandler

#endif

