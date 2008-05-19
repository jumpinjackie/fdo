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
// | The file contains the definition of the class FdoRdbmsGetLockedObjects. The
// | class initiates the retrieval of the objects locked by a named owner.
// =============================================================================

#ifndef FdoRdbmsGetLockedObjects_h
#define FdoRdbmsGetLockedObjects_h 1

// The links to the header files to be included.

#include "FdoRdbms.h"
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsCommand.h"
#include "FdoRdbmsLockedObjectsReader.h"

class FdoRdbmsGetLockedObjects : public FdoRdbmsCommand<FdoIGetLockedObjects>
{

  friend class FdoRdbmsConnection;

  public:

    // Class Functions:
    //   Execute      : Executes a lock request.
    //   GetLockOwner : Returns the lock owner for which the information is to
    //                  be retrieved.
    //   SetLockOwner : Sets the lock owner for which the information is to be
    //                  retrieved.

    void                   SetLockOwner  (FdoString* value);
    FdoString              *GetLockOwner ();
    FdoILockedObjectReader *Execute      ();

  protected:

    // Class Functions:
    //   SetToZero  : Initializes all class member variables.
    //   FreeMemory : Frees all memory allocated by the member variables.

            FdoRdbmsGetLockedObjects  ();
            FdoRdbmsGetLockedObjects  (FdoIConnection *connection);
    virtual ~FdoRdbmsGetLockedObjects ();

    void     SetToZero                ();
    void     FreeMemory               ();


    // Class Variables:
    //   requested_owner : Stores the named lock owner for which the information
    //                     is to be retrieved
    //   dbi_connection  : References the DBI connection.

    FdoString     *requested_owner;

	FdoRdbmsConnection  *fdo_rdbms_connection;

  private:

    // Class Enumerations:


    // Class Functions:


};  //  class FdoRdbmsGetLockedObjects

#endif
