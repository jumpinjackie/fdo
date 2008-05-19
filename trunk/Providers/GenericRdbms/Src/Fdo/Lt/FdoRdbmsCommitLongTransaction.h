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
// | The file contains the definition of the class FdoRdbmsCommitLongTransac-
// | tion. The class executes either a full or partial commit operation on the
// | identified data. A full commit operation includes the committing of the
// | named long transaction to its ancestor long transaction and its removal.
// | A partial commit operation commits specified data in the named long trans-
// | action to its parent long transaction, but does not remove the processed
// | long transactions.
// =============================================================================

#ifndef FdoRdbmsCommitLongTransaction_h
#define FdoRdbmsCommitLongTransaction_h 1

// The links to the header files to be included.

#include "FdoRdbms.h"
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsFeatureCommand.h"
#include "FdoRdbmsLongTransactionConflictDirectiveEnumerator.h"
#include "LongTransactionManager.h"


class FdoRdbmsCommitLongTransaction :
                    public FdoRdbmsFeatureCommand<FdoICommitLongTransaction>
{

  friend class FdoRdbmsConnection;

  public:

    // ************************************************************************
    // *                           Public Interfaces                          *
    // ************************************************************************

    //  Execute:
    //      Executes the commit command.

    FdoILongTransactionConflictDirectiveEnumerator *Execute ();

    //  GetLockConflictReader:
    //      Returns the lock conflict reader. The lock conflict reader identi-
    //      fies objects affected by the requested commit operation that are
    //      locked with a persistent lock by a user other than the one request-
    //      ing the commit operation. The commit operation will not be executed
    //      if lock conflicts are detected.

    FdoILockConflictReader *GetLockConflictReader ();

    //  GetName:
    //      Returns the name of the long transaction set for the command to be
    //      committed.

    FdoString *GetName ();

    //  SetName:
    //      Sets the name of the long transaction to be committed when the
    //      command is executed.

    void SetName (FdoString *value);


  protected:

    // ************************************************************************
    // *                         Protected Interfaces                         *
    // ************************************************************************

    //  FdoRdbmsCommitLongTransaction:
    //      Class constructors.

    FdoRdbmsCommitLongTransaction ();
    FdoRdbmsCommitLongTransaction (FdoIConnection *connection);

    //  ~FdoRdbmsCommitLongTransaction:
    //      The class destructor.

    virtual ~FdoRdbmsCommitLongTransaction ();


  private:

    // ************************************************************************
    // *                          Private Interfaces                          *
    // ************************************************************************

    //  CheckConflictResolutionFlags:
    //      The function checks whether or not the long transaction conflict
    //      resolution flags have been set. Long Transaction conflicts cannot
    //      be resolved if the flag does nnot indicate which version of the
    //      object causing a conflict is to be kept.

    bool CheckConflictResolutionFlags ();

    //  ClearMemory:
    //      The function frees memory used by the class variables.

    void ClearMemory ();

    //  ProcessLTConflicts:
    //      The function resolves long transaction conflicts.

    void ProcessLTConflicts ();

    //  SetToZero:
    //      Initializes the class variables.

    void SetToZero ();

    //  SetValue:
    //      The function creates a copy of the given value buffer and returns
    //      a reference to it back to the caller.

    wchar_t *SetValue (FdoString *value_buffer);


    // ************************************************************************
    // *                        Private Member Variables                      *
    // ************************************************************************

    //  lt_name:
    //      Holds the name of the long transaction provided to this command.

    FdoString *lt_name;

    //  fdo_i_connection:
    //      References the FDO connection.

    FdoIConnection *fdo_i_connection;

    //  fdo_rdbms_connection:
    //      References the FDO RDBMS connection.

    FdoRdbmsConnection *fdo_rdbms_connection;

    //  lt_lk_cfl_reader:
    //      Holds the reference to the lock conflict reader.

    FdoILockConflictReader *lt_lk_cfl_reader;

    //  lt_conflicts:
    //      References detected long transaction conflicts.

    FdoRdbmsLongTransactionConflicts *lt_conflicts;

    //  lt_cfl_enum:
    //      References the long transaction conflict directive enumerator.

    FdoRdbmsLongTransactionConflictDirectiveEnumerator *lt_cfl_enum;


};  //  class FdoRdbmsCommitLongTransaction

#endif
