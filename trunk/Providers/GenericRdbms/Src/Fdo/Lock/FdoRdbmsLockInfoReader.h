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
// | The file contains the definition of the class FdoRdbmsLockInfoReader. The
// | class implements functions for the lock info reader.
// =============================================================================

#ifndef FdoRdbmsLockInfoReader_h
#define FdoRdbmsLockInfoReader_h 1

// The links to the header files to be included.

#include "FdoRdbms.h"
#include "Fdo.h"
#include "FdoRdbmsConnection.h"
#include "LockUtility.h"
#include "LockManager.h"


class FdoRdbmsLockInfoReader : public FdoILockedObjectReader
{

  private:

    // Class Enumerations:
    //   ReadConflictStatus: The enumeration contains return values used by the
    //                       function "GetNextConflictData ()".

    enum ReadStatus {SUCCESS, FAILURE, END_OF_BUFFER};

  public:

    // Class Functions:
    //   GetFeatureClassName: Returns the class name for the current feature
    //                        being read.
    //   GetLongTransaction : The function returns the name of the long trans-
    //                        action.
    //   GetIdentity        : Returns a PropertyValueCollection containing the
    //                        property values that uniquely identify the feature
    //                        currently being read.
    //   GetLockType        : Gets the lock type applied on the feature cur-
    //                        rently read.
    //   GetLockOwner       : Gets the name of the owner that holds a lock on
    //                        the feature currently read.
    //   ReadNext           : Advances the reader to the next item. The default
    //                        position of the reader is prior to the first item.
    //                        Therefore ReadNext must be called to access any
    //                        data.
    //   Close              : Closes the lock info reader object freeing any
    //                        resources it may be holding.

                               FdoRdbmsLockInfoReader ();
                               FdoRdbmsLockInfoReader (
                                            FdoRdbmsConnection *fdoConnection,
											char		   *className,
                                            char           *sqlFilter,
                                            char           *tableName);

    bool                       ReadNext               ();
    void                       Close                  ();
    FdoString                  *GetFeatureClassName   ();
    FdoString                  *GetLongTransaction    ();
    FdoString                  *GetLockOwner          ();
    const FdoLockType          GetLockType            ();
    FdoPropertyValueCollection *GetIdentity           ();

  protected:

    // Class Functions:
    //   SetToZero     : Initializes all class member variables.
    //   FreeMemory    : Frees all memory allocated by the member variables.
    //   InitialLoad   : Initially loads lock info information.
    //   GetNextDataSet: Reads the next lock info information set.

    void         SetToZero               ();
    void         FreeMemory              (bool include_members);
    bool         InitialLoad             ();
    ReadStatus   GetNextDataSet          ();
	void		 validQuery			  ();

    virtual      ~FdoRdbmsLockInfoReader ();
    virtual void Dispose                 () { delete this; }

    // Class Variables:
    //   dbi_connection: References the DBI connection.
    //   fdo_connection: References the FDO connection.

    DbiConnection      *dbi_connection;

    FdoRdbmsConnection *fdo_connection;


  private:

    // Class Variables:
    //   lock_infos_loaded  : To retrieve lock information the system executes
    //                        a DBI read operation that loads the lock data into
    //                        memory. The data member "lock_infos_loaded" indi-
    //                        cates whether or not this initial load process has
    //                        already taken place.
    //   sql_statement      : The variable holds the sql statement necessary to
    //                        retrieve the lock info data. The statement is pro-
    //                        vided by the GetLockInfo command.
    //   current_lock_type  : Identifies the lock type applied on an object for
    //                        which lock information is being requested.
    //   current_lt_name    : Identifies the long transaction in which the ob-
    //                        ject being reported is locked.
    //   current_class_name : Represents the class name of the object for which
    //                        lock information is being requested.
    //   current_lock_owner : Identifies the owner of an object for which lock
    //                        information is being requested.
    //   identity_collection: Stores the identity for a given object.

    bool                       lock_infos_loaded;
	bool					   mEOF;

    FdoString                  *sql_filter;
    char                       *current_class_id;
    char                       *current_lock_type;

    FdoString                  *current_lt_name;
    FdoString                  *request_class_name;
	FdoString				   *request_table_name;
    FdoString                  *current_lock_owner;

    FdoPropertyValueCollection *identity_collection;

	FdoRdbmsLockInfoQueryHandler	*lockInfoQueryHandler;


    // Class Functions:


};  //  class FdoRdbmsLockInfoReader

#endif
