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
// | The file contains the definition of the class FdoRdbmsGetLockInfo. The
// | class retrieves lock information for objects identified by the user.
// =============================================================================

#ifndef FdoRdbmsGetLockInfo_h
#define FdoRdbmsGetLockInfo_h 1

// The links to the header files to be included.

#include "FdoRdbms.h"
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsFeatureCommand.h"
#include "FdoRdbmsLockInfoReader.h"

class FdoRdbmsGetLockInfo : public FdoRdbmsFeatureCommand<FdoIGetLockInfo>
{

  friend class FdoRdbmsConnection;

  public:

    // Class Functions:
    //   Execute        : Executes a lock request.

    FdoILockedObjectReader *Execute ();

  protected:

    // Class Functions:
    //   SetToZero        : Initializes all class member variables.
    //   FreeMemory       : Frees all memory allocated by the member variables.
    //   GetSQLLockInfoCmd: Constructs the SQL command to be used to retrieve
    //                      lock information in the current environment.

            FdoRdbmsGetLockInfo  ();
            FdoRdbmsGetLockInfo  (FdoIConnection *connection);
    virtual ~FdoRdbmsGetLockInfo ();

    bool	    GetSQLLockInfoCmd   ();

    void     SetToZero           ();
    void     FreeMemory          ();


    // Class Variables:
    //   req_class_name   : Stores the class name for which the lock info is
    //                      retrieved
    //   *_connection     : References to connection objects.

    char                *req_class_name;
	char				*tableName;
	char				*sqlFilter;

    //DbiConnection       *dbi_connection;

    FdoRdbmsConnection  *fdo_rdbms_connection;

  private:

    // Class Enumerations:


    // Class Functions:


};  //  class FdoRdbmsGetLockInfo

#endif
