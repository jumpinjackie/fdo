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
// | The file contains the definition of the class FdoRdbmsLockTypes. The class
// | implements a collection of lock types.
// =============================================================================

#ifndef FdoRdbmsLockTypes_h
#define FdoRdbmsLockTypes_h

// The links to the header files to be included.

#include <FdoRdbms.h>
#include "Collection.h"
#include "LockType.h"

class FdoRdbmsLockTypes : public FdoRdbmsCollection<FdoRdbmsLockType>
{

  protected:

    // Class Functions:
    //  FdoRdbmsLockTypes:
    //      Class constructor.
    //  ~FdoRdbmsLockTypes:
    //      Class destructor.
    //  Dispose:
    //      Deletes the current collection.

    FdoRdbmsLockTypes ():FdoRdbmsCollection<FdoRdbmsLockType> () {}

    virtual ~FdoRdbmsLockTypes () {}

    virtual void Dispose () { delete this; }


  public:

    // Class Functions:
    //  Create:
    //      Returns a reference to a new long transaction conflict info collec-
    //      tion.

    static FdoRdbmsLockTypes *Create ();

};  //  class FdoRdbmsLockTypes 

#endif
