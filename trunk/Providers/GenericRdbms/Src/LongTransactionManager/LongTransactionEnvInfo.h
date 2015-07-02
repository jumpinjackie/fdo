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
// | The file contains the definition of the class FdoRdbmsLongTransactionEnv-
// | Info. The class defines the structure used to store long transaction envi-
// | ronment enviromation.
// =============================================================================


#ifndef FdoRdbmsLongTransactionEnvInfo_h
#define FdoRdbmsLongTransactionEnvInfo_h

#ifdef _WIN32
#pragma once
#endif

#include <FdoCommonThreadMutex.h>
#include <Inc/Nls/fdordbms_msg.h>
#include "FdoRdbmsUtil.h"
#include "FdoRdbmsException.h"
#include "LongTransactionQueryModes.h"
#include "LongTransactionInfos.h"

class FdoRdbmsLongTransactionEnvInfo: public FdoLtmDisposable
{

    public:

    // ************************************************************************
    // *                           Public Interfaces                          *
    // ************************************************************************

    //  GetActiveLt:
    //      The function returns a reference to the information for the active
    //      long transaction.

    FdoRdbmsLongTransactionInfo *GetActiveLt ();

    //  GetLtSelectionSet:
    //      The function returns a reference to the collection of long trans-
    //      actions that define the long transaction selection set.

    FdoRdbmsLongTransactionInfos *GetLtSelectionSet ();

    //  GetQueryMode:
    //      The function returns the currently active query mode.

    FdoRdbmsLongTransactionQueryModes GetQueryMode ();

    //  SetActiveLt:
    //      The function sets the reference to the information for the active
    //      long transaction.

    void SetActiveLt (FdoRdbmsLongTransactionInfo *value);

    //  SetLtSelectionSet:
    //      The function sets the reference to the collection of long trans-
    //      actions that define the long transaction selection set.

    void SetLtSelectionSet (FdoRdbmsLongTransactionInfos *root);

    //  SetQueryMode:
    //      The function sets the currently active query mode.

    void SetQueryMode (FdoRdbmsLongTransactionQueryModes value);


    protected:

    // ************************************************************************
    // *                         Protected Interfaces                         *
    // ************************************************************************

    //  Dispose:
    //      Function to delete an object of this class.

    virtual void Dispose () { delete this; }

    //  FdoRdbmsLongTransactionEnvInfo:
    //      The class constructor.

    FdoRdbmsLongTransactionEnvInfo ();

    //  ~FdoRdbmsLongTransactionEnvInfo:
    //      The class destructor.

    virtual ~FdoRdbmsLongTransactionEnvInfo ();


    private:

    // ************************************************************************
    // *                          Private Interfaces                          *
    // ************************************************************************

    //  ClearMemory:
    //      The function releases memory allocated for the class variables.

    void ClearMemory ();

    //  SetToZero:
    //      The function initializes the class variables.

    void SetToZero ();


    // ************************************************************************
    // *                        Private Member Variables                      *
    // ************************************************************************

    //  active_lt:
    //      The variable references the long transaction data set representing
    //      the currently active long transaction.

    FdoRdbmsLongTransactionInfo *active_lt;

    //  lt_selection_set:
    //      The variable references the collection of long transactions that
    //      define the current long transaction selection set.

    FdoRdbmsLongTransactionInfos *lt_select_set;

    //  query_mode:
    //      The variable holds the value of the current query mode.

    FdoRdbmsLongTransactionQueryModes query_mode;


};  //  class FdoRdbmsLongTransactionEnvInfo

#endif

