/***************************************************************************
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
 ***************************************************************************/

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
