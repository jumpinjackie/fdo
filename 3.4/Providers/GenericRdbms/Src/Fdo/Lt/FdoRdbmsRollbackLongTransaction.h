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
// | The file contains the definition of the class FdoRdbmsRollbackLongTransac-
// | tion. The class executes either a full or partial rollback operation on
// | the identified data. A full rollback operation includes the rolling back
// | of the named long transaction and all its descendents and removing of all
// | the processed long transactions. A partial rollback operation rolls back
// | specified data in the named long transaction and all its descendents, but
// | does not remove the processed long transactions.
// =============================================================================

#ifndef FdoRdbmsRollbackLongTransaction_h
#define FdoRdbmsRollbackLongTransaction_h 1

// The links to the header files to be included.

#include "FdoRdbms.h"
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsFeatureCommand.h"
#include "LongTransactionManager.h"

class FdoRdbmsRollbackLongTransaction : 
                    public FdoRdbmsFeatureCommand<FdoIRollbackLongTransaction>
{

  friend class FdoRdbmsConnection;

  public:

    // ************************************************************************
    // *                           Public Interfaces                          *
    // ************************************************************************

    //  Execute:
    //      Executes the rollback command.

    void Execute ();

    //  SetName:
    //      Sets the name of the long transaction to be rolled back when the
    //      command is executed.

    void SetName (FdoString *value);

    //  GetName:
    //      Returns the name of the long transaction to be rolled back set
    //      for the command.

    FdoString *GetName ();

    //  GetLockConflictReader:
    //      Returns the lock conflict reader. The lock conflict reader identi-
    //      fies objects affected by the requested rollback operation that
    //      are locked with a persistent lock by a user other than the one
    //      requesting the rollback operation. The rollback operation will
    //      not be executed if lock conflicts are detected.

    FdoILockConflictReader *GetLockConflictReader ();


  protected:

    // ************************************************************************
    // *                         Protected Interfaces                         *
    // ************************************************************************

    //  FdoRdbmsRollbackLongTransaction:
    //      A class constructor.

    FdoRdbmsRollbackLongTransaction ();
    FdoRdbmsRollbackLongTransaction (FdoIConnection *connection);

    //  ~FdoRdbmsRollbackLongTransaction:
    //      The class destructor.

    virtual ~FdoRdbmsRollbackLongTransaction ();


  private:

    // ************************************************************************
    // *                          Private Interfaces                          *
    // ************************************************************************

    //  ClearMemory:
    //      The function frees memory used by the class variables.

    void ClearMemory ();

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


};  //  class FdoRdbmsRollbackLongTransaction

#endif
