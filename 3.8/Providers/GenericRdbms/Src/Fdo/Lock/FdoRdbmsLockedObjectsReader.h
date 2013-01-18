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
// | The file contains the definition of the class FdoRdbmsLockedObjectsReader.
// | The class implements functions for the locked objects reader.
// =============================================================================

#ifndef FdoRdbmsLockedObjectsReader_h
#define FdoRdbmsLockedObjectsReader_h 1

// The links to the header files to be included.

#include "FdoRdbms.h"
#include "Fdo.h"
#include "FdoRdbmsConnection.h"
#include "LockUtility.h"
#include "LockManager.h"
//#include "../lock/lk_data_obj.h"


class FdoRdbmsLockedObjectsReader : public FdoILockedObjectReader
{

  private:

    // Class Enumerations:

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

                               FdoRdbmsLockedObjectsReader ();
                               FdoRdbmsLockedObjectsReader (
                                                FdoRdbmsConnection *a_dbi_connection,
                                                FdoString      *lk_owner);

    bool                       ReadNext                    ();
    void                       Close                       ();
    FdoString                  *GetFeatureClassName        ();
    FdoString                  *GetLongTransaction         ();
    FdoString                  *GetLockOwner               ();
    const FdoLockType          GetLockType                 ();
    FdoPropertyValueCollection *GetIdentity                ();

  protected:

    // Class Functions:
    //   SetToZero     : Initializes all class member variables.
    //   FreeMemory    : Frees all memory allocated by the member variables.

    void         SetToZero                    ();
    void         FreeMemory                   ();
	bool		 ProcessNextRow				  ();

    virtual      ~FdoRdbmsLockedObjectsReader ();
    virtual void Dispose                      () { delete this; }

    // Class Variables:
    //   dbi_connection: References the DBI connection.
    //   fdo_connection: References the FDO connection.

	DbiConnection      *dbi_connection;
    FdoRdbmsConnection *fdo_connection;


  private:

    // Class Variables:
    //   lk_data_obj        : Represents a pointer to the DBI communication
    //                        object used to transfer data between DBI and
    //                        the Oracle Provider.
    //   query_activated    : A flag to indicate whether the query to retrieve
    //                        the locked objects information has been started.
    //   class_name         : Stores the current class name for the current
    //                        object.
    //   requested_owner    : Stores the named lock owner for which the
    //                        information is to be retrieved
    //   identity_collection: Stores the identity for a given object.

    bool						mainQueryActive;
	bool						subQueryActive;	
	bool						mIsEOFmainQ;
	bool						mIsEOFsubQ;

    FdoString                  *lt_name,
                               *class_name,
                               *requested_owner;

    dbi_lk_data_obj            *lk_data_obj;

    FdoPropertyValueCollection *identity_collection;

	FdoRdbmsLockTableQueryHandler	*tableQueryHandler;
	FdoRdbmsLockInfoQueryHandler	*lockInfoQueryHandler;

    // Class Functions:


};  //  class FdoRdbmsLockedObjectsReader

#endif
