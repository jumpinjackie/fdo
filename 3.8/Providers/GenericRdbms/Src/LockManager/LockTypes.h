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
