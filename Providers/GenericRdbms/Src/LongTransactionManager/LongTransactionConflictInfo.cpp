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
