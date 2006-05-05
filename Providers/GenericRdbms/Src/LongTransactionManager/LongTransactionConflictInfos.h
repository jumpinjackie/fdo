//
//  Copyright (C) 2001-2003 Autodesk, Inc. All Rights Reserved.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
//  CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
//  IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
//  DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
//  DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject
//  to restrictions set forth in FAR 52.227-19 (Commercial Computer
//  Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
//  (Rights in Technical Data and Computer Software), as applicable.

// =============================================================================
// | The file contains the definition of the class FdoRdbmsLongTransactionInfos.
// | The class implements the collection of long transaction info sets.
// =============================================================================

#ifndef FdoRdbmsLongTransactionConflictInfos_h
#define FdoRdbmsLongTransactionConflictInfos_h

// The links to the header files to be included.

#include <FdoRdbms.h>
#include "Collection.h"
#include "LongTransactionConflictInfo.h"

class FdoRdbmsLongTransactionConflictInfos : 
                public FdoRdbmsCollection<FdoRdbmsLongTransactionConflictInfo>
{

  public:

    // ************************************************************************
    // *                           Public Interfaces                          *
    // ************************************************************************

    //  Create:
    //      Returns a reference to a new long transaction conflict info collec-
    //      tion.

    static FdoRdbmsLongTransactionConflictInfos *Create ();


  protected:

    // ************************************************************************
    // *                         Protected Interfaces                         *
    // ************************************************************************

    //  Dispose:
    //      Function to delete an object of this class.

    virtual void Dispose () { delete this; }

    //  FdoRdbmsLongTransactionConflictInfos:
    //      The class constructor.

    FdoRdbmsLongTransactionConflictInfos ():
                FdoRdbmsCollection<FdoRdbmsLongTransactionConflictInfo> () {}

    //  ~FdoRdbmsLongTransactionConflictInfos:
    //      The class destructor.

    virtual ~FdoRdbmsLongTransactionConflictInfos () {}


};  //  class FdoRdbmsLongTransactionConflictInfos 

#endif
