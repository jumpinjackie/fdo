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

// =============================================================================
// | The file contains the definition of the class FdoRdbmsLockConflicts. The
// | class implements the lock conflict collection.
// =============================================================================

#ifndef FdoRdbmsLockConflicts_h
#define FdoRdbmsLockConflicts_h

// The links to the header files to be included.

// #include "FdoStd.h"
// #include "Fdo.h"
// #include "fdo/Collection.h"
#include <FdoRdbms.h>
#include "Collection.h"
#include "LockConflict.h"

// class FdoRdbmsLockConflicts : public FdoCollection<FdoRdbmsLockConflict, FdoCommandException>
class FdoRdbmsLockConflicts : public FdoRdbmsCollection<FdoRdbmsLockConflict>
{

  protected:

    // FdoRdbmsLockConflicts():FdoCollection<FdoRdbmsLockConflict, FdoCommandException> ()
    FdoRdbmsLockConflicts():FdoRdbmsCollection<FdoRdbmsLockConflict> ()
    {
    }  //  FdoRdbmsLockConflicts ()

    virtual ~FdoRdbmsLockConflicts ()
    {
    }  //  ~FdoRdbmsLockConflicts ()

    virtual void Dispose ()
    {
        delete this;
    }

  public:

    static FdoRdbmsLockConflicts *Create ();

};

#endif
