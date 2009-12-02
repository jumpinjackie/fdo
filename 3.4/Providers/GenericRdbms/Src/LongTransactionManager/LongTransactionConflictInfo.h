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
// | The file contains the definition of the class FdoRdbmsLongTransactionCon-
// | flictInfo. The class defines the structure used to store a single long
// | transaction conflict.
// =============================================================================


#ifndef FdoRdbmsLongTransactionConflictInfo_h
#define FdoRdbmsLongTransactionConflictInfo_h

#ifdef _WIN32
#pragma once
#endif

#include <FdoCommonThreadMutex.h>
#include "Disposable.h"
#include "FdoRdbmsPrimaryKey.h"
#include "LongTransactionConflictSolution.h"

class FdoRdbmsLongTransactionConflictInfo: public FdoLtmDisposable
{

    public:

    // ************************************************************************
    // *                           Public Interfaces                          *
    // ************************************************************************

    //  GetConflictSolution:
    //      The function returns the conflict solution set an identified long
    //      transaction conflict.

    FdoRdbmsLongTransactionConflictSolution GetConflictSolution ();

    //  GetPk:
    //      The function returns a reference to the primary key identifying
    //      the object causing a long transaction conflict.

    FdoRdbmsPrimaryKey *GetPk ();

    //  SetConflictSolution:
    //      The function allows the setting of the conflict solution indicator
    //      to be applied for the current long transaction conflict.

    void SetConflictSolution (FdoRdbmsLongTransactionConflictSolution value);

    //  SetPk:
    //      The function sets the reference to the primary key identifying
    //      the object causing a long transaction conflict.

    void SetPk (FdoRdbmsPrimaryKey *value);


    protected:

    // ************************************************************************
    // *                         Protected Interfaces                         *
    // ************************************************************************

    //  Dispose:
    //      Function to delete an object of this class.

    virtual void Dispose () { delete this; }

    //  FdoRdbmsLongTransactionConflictInfo:
    //      The class constructor.

    FdoRdbmsLongTransactionConflictInfo ();

    //  ~FdoRdbmsLongTransactionConflictInfo:
    //      The class destructor.

    virtual ~FdoRdbmsLongTransactionConflictInfo ();


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

    //  conflict_pk:
    //      The variable holds a reference to the primary key of the object
    //      causing a long transaction conflict.

    FdoRdbmsPrimaryKey *conflict_pk;

    //  conflict_solution_flag:
    //      The variable holds the indicator on how to solve the conflict.

    FdoRdbmsLongTransactionConflictSolution conflict_solution_flag;


};  //  class FdoRdbmsLongTransactionConflictInfo

#endif

