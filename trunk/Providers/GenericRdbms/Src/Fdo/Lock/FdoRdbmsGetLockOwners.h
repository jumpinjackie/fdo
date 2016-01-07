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
// | The file contains the definition of the class FdoRdbmsGetLockOwners. The
// | class retrieves a list of available locks in the current schema. The in-
// | formation is provided to the user via the ILockOwnersReader interface.
// =============================================================================

#ifndef FdoRdbmsGetLockOwners_h
#define FdoRdbmsGetLockOwners_h 1

// The links to the header files to be included.

#include "FdoRdbms.h"
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsCommand.h"
#include "FdoRdbmsLockOwnersReader.h"

class FdoRdbmsGetLockOwners : public FdoRdbmsCommand<FdoIGetLockOwners>
{

  friend class FdoRdbmsConnection;

  public:

    // Class Functions:
    //   Execute: Executes a lock request.

    FdoILockOwnersReader *Execute ();

  protected:

    // Class Functions:

            FdoRdbmsGetLockOwners  ();
            FdoRdbmsGetLockOwners  (FdoIConnection *connection);
    virtual ~FdoRdbmsGetLockOwners ();

  protected:

    // Class Variables:
    //   *_connection: References various connection objects.

    DbiConnection       *dbi_connection;
    FdoRdbmsConnection  *fdo_rdbms_connection;

  private:

    // Class Enumerations:

    // Class Functions:

};

#endif
