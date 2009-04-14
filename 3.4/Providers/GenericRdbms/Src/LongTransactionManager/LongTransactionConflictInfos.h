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
