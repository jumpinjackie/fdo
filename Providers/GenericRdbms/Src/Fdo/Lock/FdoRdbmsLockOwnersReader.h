/***************************************************************************
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ***************************************************************************/

// =============================================================================
// | The file contains the definition of the class FdoRdbmsLockOwnersReader.
// | The class implements functions for the lock owner reader.
// =============================================================================

#ifndef FdoRdbmsLockOwnersReader_h
#define FdoRdbmsLockOwnersReader_h 1

// The links to the header files to be included.

#include "FdoRdbms.h"
#include "Fdo.h"
#include "FdoRdbmsConnection.h"


class FdoRdbmsLockOwnersReader : public FdoILockOwnersReader
{

  private:

    // Class Enumerations:
    //   ReadStatus:   The enumeration contains return values used by the
    //                 function "GetNextConflictData ()".

    enum ReadStatus {SUCCESS, FAILURE, END_OF_BUFFER};


  public:

    // Class Functions:
    //   GetLockOwner: Gets the name of the owner that holds a lock on the
    //                 feature currently read.
    //   ReadNext    : Advances the reader to the next item. The default
    //                 position of the reader is prior to the first item.
    //                 Therefore you must call ReadNext to begin accessing
    //                 any data.
    //   Close       : Closes the lock conflict reader object freeing any
    //                 resources it may be holding.

                  FdoRdbmsLockOwnersReader ();
                  FdoRdbmsLockOwnersReader (FdoIConnection *a_dbi_connection);

    bool          ReadNext                 ();
    void          Close                    ();
    FdoString     *GetLockOwner            ();

  protected:

    // Class Functions:
    //   SetToZero   : Initializes all class member variables.
    //   FreeMemory  : Frees all memory allocated by the member variables.
    //   InitialLoad : Initially loads lock owner information from the
    //                 corresponding table in the database.
    //   GetNextOwner: Reads the next lock owner information set.

    void         SetToZero                 ();

    virtual      ~FdoRdbmsLockOwnersReader ();
    virtual void Dispose                   () { delete this; }

    // Class Variables:
    //   dbi_connection: References the DBI connection.
    //   fdo_connection: References the FDO connection.

    DbiConnection      *dbi_connection;
    FdoRdbmsConnection *fdo_connection;


  private:

    // Class Variables:
    //   qid               : Represents a DBI query id.
    //   lock_owners_loaded: To retrieve lock conflict information the system
    //                       executes a DBI read operation that loads the lock
    //                       conflict data into memory. The data member "lock_-
    //                       owners_loaded" indicates whether or not an initial
    //                       load process has already taken place.
    //   current_lock_owner: Identifies a lock owner.

    int       qid;
    bool      lock_owners_loaded;

	FdoRdbmsLockOwnersQueryHandler *mLockOwnersQueryHandler;

    // Class Functions:

};  //  class FdoRdbmsLockOwnersReader

#endif
