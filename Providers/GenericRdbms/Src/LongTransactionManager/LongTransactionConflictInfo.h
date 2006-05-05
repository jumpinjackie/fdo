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

