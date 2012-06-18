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
// | The file contains the implementation of the class FdoRdbmsLongTransaction-
// | Conflict.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include <malloc.h>

#include "LongTransactionEnvInfo.h"


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoRdbmsLongTransactionEnvInfo::FdoRdbmsLongTransactionEnvInfo ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsLongTransactionEnvInfo ()

FdoRdbmsLongTransactionEnvInfo::~FdoRdbmsLongTransactionEnvInfo ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Release the memory occupied by the class variables.

    ClearMemory ();

}  //  ~FdoRdbmsLongTransactionEnvInfo ()


// ----------------------------------------------------------------------------
// --                            Public Interfaces                           --
// ----------------------------------------------------------------------------

FdoRdbmsLongTransactionInfos 
                        *FdoRdbmsLongTransactionEnvInfo::GetLtSelectionSet ()

// +---------------------------------------------------------------------------
// | The function returns the reference to the long transaction selection set.
// +---------------------------------------------------------------------------

{

    return lt_select_set;

}  //  GetLtSelectionSet ()

void FdoRdbmsLongTransactionEnvInfo::SetLtSelectionSet (
                                        FdoRdbmsLongTransactionInfos *root)

// +---------------------------------------------------------------------------
// | The function sets the reference to the long transaction selection set.
// +---------------------------------------------------------------------------

{

    if (lt_select_set != NULL) {

        lt_select_set->Clear();
        lt_select_set = NULL;

    }  //  if (lt_select_set != NULL) ...

    lt_select_set = root;

}  //  SetLtSelectionSet ()

FdoRdbmsLongTransactionInfo *FdoRdbmsLongTransactionEnvInfo::GetActiveLt ()

// +---------------------------------------------------------------------------
// | The function returns the reference to the active long transaction.
// +---------------------------------------------------------------------------

{

    return active_lt;

}  //  GetDataStore ()

void FdoRdbmsLongTransactionEnvInfo::SetActiveLt (
                                        FdoRdbmsLongTransactionInfo *value)

// +---------------------------------------------------------------------------
// | The function sets the reference to the active long transaction.
// +---------------------------------------------------------------------------

{

    if (active_lt != NULL) {

        FDO_SAFE_RELEASE(active_lt);
        active_lt = NULL;

    }  //  if (active_lt != NULL) ...

    active_lt = value;

}  //  SetActiveLt ()

FdoRdbmsLongTransactionQueryModes 
                            FdoRdbmsLongTransactionEnvInfo::GetQueryMode ()

// +---------------------------------------------------------------------------
// | The function returns the value of the query mode property.
// +---------------------------------------------------------------------------

{

    return query_mode;

}  //  GetQueryMode ()

void FdoRdbmsLongTransactionEnvInfo::
                        SetQueryMode (FdoRdbmsLongTransactionQueryModes value)

// +---------------------------------------------------------------------------
// | The function sets the value of the query mode property.
// +---------------------------------------------------------------------------

{

    query_mode = value;

}  //  SetQueryMode ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoRdbmsLongTransactionEnvInfo::ClearMemory ()

// +---------------------------------------------------------------------------
// | The function releases memory allocated for the class variables.
// +---------------------------------------------------------------------------

{

    if (active_lt     != NULL) FDO_SAFE_RELEASE(active_lt);
    if (lt_select_set != NULL) lt_select_set->Clear();

    SetToZero();

}  //  ClearMemory ()

void FdoRdbmsLongTransactionEnvInfo::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    active_lt     = NULL;
    lt_select_set = NULL;

    query_mode    = FdoRdbmsLongTransactionQueryModes_DataView;

}  //  SetToZero ()
