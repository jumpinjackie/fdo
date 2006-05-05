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
// | ConflictInfo.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include <malloc.h>

#include "LongTransactionConflictInfo.h"


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoRdbmsLongTransactionConflictInfo::FdoRdbmsLongTransactionConflictInfo ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsLongTransactionConflictInfo ()

FdoRdbmsLongTransactionConflictInfo::~FdoRdbmsLongTransactionConflictInfo ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Release the memory occupied by the class variables.

    ClearMemory ();

}  //  ~FdoRdbmsLongTransactionConflictInfo ()


// ----------------------------------------------------------------------------
// --                            Public Interfaces                           --
// ----------------------------------------------------------------------------

FdoRdbmsPrimaryKey *FdoRdbmsLongTransactionConflictInfo::GetPk ()

// +---------------------------------------------------------------------------
// | The function returns a reference to the primary key property.
// +---------------------------------------------------------------------------

{

    return conflict_pk;

}  //  GetPk ()

void FdoRdbmsLongTransactionConflictInfo::SetPk (FdoRdbmsPrimaryKey *value)

// +---------------------------------------------------------------------------
// | The function sets the reference of the primary key property.
// +---------------------------------------------------------------------------

{

    conflict_pk = value;

}  //  SetPk ()

FdoRdbmsLongTransactionConflictSolution 
                    FdoRdbmsLongTransactionConflictInfo::GetConflictSolution ()

// +---------------------------------------------------------------------------
// | The function returns the value of the conflict solution flag property.
// +---------------------------------------------------------------------------

{

    return conflict_solution_flag;

}  //  GetConflictSolution ()

void FdoRdbmsLongTransactionConflictInfo::SetConflictSolution (
                                FdoRdbmsLongTransactionConflictSolution value)

// +---------------------------------------------------------------------------
// | The function sets the value of the conflict solution flag property.
// +---------------------------------------------------------------------------

{

    conflict_solution_flag = value;

}  //  SetConflictSolution ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoRdbmsLongTransactionConflictInfo::ClearMemory ()

// +---------------------------------------------------------------------------
// | The function releases memory allocated for the class variables.
// +---------------------------------------------------------------------------

{

    if (conflict_pk  != NULL) conflict_pk->Clear();

    SetToZero();

}  //  ClearMemory ()

void FdoRdbmsLongTransactionConflictInfo::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    conflict_pk            = NULL;
    conflict_solution_flag =
                        FdoRdbmsLongTransactionConflictSolution_Unresolved;

}  //  SetToZero ()
