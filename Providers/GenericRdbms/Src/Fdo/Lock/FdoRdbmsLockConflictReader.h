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
// | The file contains the definition of the class FdoRdbmsLockConflictReader.
// | The class implements functions for the lock conflict reader.
// =============================================================================

#ifndef FdoRdbmsLockConflictReader_h
#define FdoRdbmsLockConflictReader_h 1

// The links to the header files to be included.

#include "FdoRdbms.h"
#include "FdoRdbmsConnection.h"
#include "LockConflictQueryHandler.h"
#include "FdoRdbmsLockConflicts.h"
#include "LockConflict.h"
#include "LockUtility.h"

class FdoRdbmsLockConflict;
class FdoRdbmsLockConflicts;


class FdoRdbmsLockConflictReader : public FdoILockConflictReader
{

  friend class FdoRdbmsConnection;

  private:

    // Class Enumerations:
    //   ReadConflictStatus: The enumeration contains return values used by the
    //                       function "GetNextConflictData ()".

    enum ReadConflictStatus {SUCCESS, END_OF_BUFFER};

  public:

    // Class Functions:
    //   Close               : Closes the lock conflict reader object freeing any
    //                         resources it may be holding.
    //   GetConflictType     : The function returns the type of conflict for a
    //                         reported conflict.
    //   GetFeatureClassName : Returns the class name for the current feature
    //                         being read.
    //   GetIdentity         : Returns a PropertyValueCollection containing the
    //                         property values that uniquely identify the feature
    //                         currently being read.
    //   GetLockConflictCount: The function returns the number of lock conflicts
    //                         stored with the lock conflict reader.
    //   GetLockOwner        : Gets the name of the owner that holds a lock on
    //                         the feature currently read.
    //   GetLongTransaction  : The function returns the name of the long trans-
    //                         action in which the reported lock conflict was
    //                         detected.
    //   ReadNext            : Advances the reader to the next item. The default
    //                         position of the reader is prior to the first item.
    //                         Therefore ReadNext must be called to access any
    //                         data.

                               FdoRdbmsLockConflictReader ();
                               FdoRdbmsLockConflictReader (
                                            FdoRdbmsConnection *mFdoConnection,
											FdoRdbmsLockConflictQueryHandler *conflictQueryHandler,
											FdoIdentifier	*confClassName);
							   FdoRdbmsLockConflictReader (
                                            FdoRdbmsConnection *mFdoConnection,
											FdoRdbmsLockConflictQueryHandler *conflictQueryHandler);
											


    void                       Close                      ();
    bool                       ReadNext                   ();
    long                       GetLockConflictCount       ();
    FdoString                  *GetFeatureClassName       ();
    FdoString                  *GetLockOwner              ();
    FdoString                  *GetLongTransaction        ();
    FdoConflictType            GetConflictType            ();
    FdoPropertyValueCollection *GetIdentity               ();

  protected:

    // Class Functions:
    //   InitialLoad              : Initially loads lock conflict information
    //                              from the corresponding view in the data-
    //                              base.
    //   FreeMemory               : Frees all memory allocated by the member
    //                              variables.
    //   GetNextConflictData      : Reads the next lock conflict information
    //                              set.
    //   HandleLockConflictDataSet: Processes a lock conflict data set.
    //   SetToZero                : Initializes all class member variables.

    virtual            ~FdoRdbmsLockConflictReader ();
    void               SetToZero                   ();
    void               FreeMemory                  ();
    virtual void       Dispose                     () { delete this; }
	void			   checkValidReader			   ();

    // Class Variables:
    //   dbi_connection: References the DBI connection.
    //   fdo_connection: References the FDO connection.

    DbiConnection       *dbi_connection;

    FdoRdbmsConnection  *fdo_connection;


  private:

    // Class Variables:
    //   conflict_report_class_name: The variable holds the name of the class
    //                               for which conflicts are reported.
    //   current_lock_conflict     : Points to the current lock conflict in the
    //                               lock conflict collection.
    //   lock_conflicts            : Points to teh collection of lock conflicts.

    FdoIdentifier           *conflict_report_class_name;

    FdoRdbmsLockConflict    *current_lock_conflict;
    FdoRdbmsLockConflicts   *lock_conflicts;

	FdoRdbmsLockConflictQueryHandler	*lockConflictQueryHandler;
	bool					validReader;
    // Class Functions:


};  //  class FdoRdbmsLockConflictReader

#endif
