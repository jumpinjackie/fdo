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
// | The file contains the definition of the class FdoRdbmsLongTransactionReader.
// | The class implements functions for the long transaction reader.
// =============================================================================

#ifndef FdoRdbmsLongTransactionReader_h
#define FdoRdbmsLongTransactionReader_h 1

// The links to the header files to be included.

#include "FdoRdbms.h"
#include "Fdo.h"
#include "FdoRdbmsConnection.h"
#include "LongTransactionLTQueryHandler.h"
#include "LongTransactionDataRequestType.h"

#define DBI_VERSION_NAME_LENGTH      30         /* length of a version name */
#define DBI_VERSION_NAME_SIZE        32
#define DBI_VERSION_DESC_SIZE       255         /* size of a version description */
#define DBI_VERSION_DESC_RET_SIZE  1000         /* size of a version description in WM */

#define DBI_LK_LOCK_CONFL_ERROR           1001
#define DBI_LK_VERSION_CONFL_ERROR        1002
#define DBI_LT_DOES_NOT_EXIST_ERROR       1003
#define DBI_LT_FREEZE_ERROR               1004
#define DBI_LT_LEAF_ERROR                 1005
#define DBI_LT_LIVE_ERROR                 1006
#define DBI_LT_NAME_UNIQUENESS_ERROR      1007
#define DBI_LT_NO_ACCESS_ERROR            1008
#define DBI_LT_SESSION_CONFL_ERROR        1009
#define DBI_LT_USE_FORCE_ERROR            1010
#define DBI_LT_VERSION_CONFL_ERROR        1011


#define DBI_LT_VERSION_CONFL_ENV_ERROR    1201
#define DBI_LT_VERSION_CONFL_LOAD_ERROR   1202
#define DBI_LT_LOCK_CONFL_LOAD_ERROR      1203

#define DBI_ROOT_LT_ID                  "_ROOT_"

#define DBI_CONFLICT_QUIT           1
#define DBI_CONFLICT_CONTINUE       2

#define DBI_LT_CFL_SOL_UNRESOLVED       0

class FdoRdbmsLongTransactionReader : public FdoILongTransactionReader
{

  public:

    // ************************************************************************
    // *                           Public Interfaces                          *
    // ************************************************************************

    //  FdoRdbmsLongTransactionReader:
    //      A class constructor.

    FdoRdbmsLongTransactionReader ();

    //  FdoRdbmsLongTransactionReader:
    //      A class constructor.

    FdoRdbmsLongTransactionReader (
                FdoIConnection                         *a_fdo_connection,
                FdoString                              *the_lt_name,
                FdoRdbmsLongTransactionDataRequestType the_query_request);


    //  IsActive:
    //      Returns TRUE if the long transaction currently read represents the
    //      user's active long transaction, FALSE otherwise.

    bool IsActive ();

    //  IsFrozen:
    //      Returns TRUE if the long transaction currently read is frozen,
    //      FALSE otherwise.

    bool IsFrozen ();

    //  ReadNext:
    //      Advances the reader to the next item. The default position of the
    //      reader is prior to the first item. Therefore ReadNext must be called
    //      to access any data.

    bool ReadNext ();


    //  Close:
    //      Closes the long transaction reader object freeing any resources it
    //      may be holding.

    void Close ();


    //  GetName:
    //      Returns the name for the long transaction currently read.

    FdoString *GetName ();

    //  GetDescription:
    //      Returns the description for the current long transaction currently
    //      read.

    FdoString *GetDescription ();

    //  GetOwner:
    //      Returns the name of the user who created the long transaction
    //      currently read.

    FdoString *GetOwner ();

    //  GetCreationDate:
    //      Returns the date the long transaction currently read was created.


    FdoDateTime GetCreationDate ();


    //  GetChildren:
    //      Returns a long transaction reader with all descendent long transac-
    //      tions for the long transaction currently read.

    FdoILongTransactionReader *GetChildren ();

    //  GetParents:
    //      Returns a long transaction reader with all ascendant long transac-
    //      tions for the long transaction currently read.

    FdoILongTransactionReader *GetParents ();


  protected:

    // ************************************************************************
    // *                         Protected Interfaces                         *
    // ************************************************************************

    //  ClearMemory:
    //      Frees all memory allocated by the member variables.

    void ClearMemory ();

    //  SetToZero:
    //      Initializes all class member variables.

    void SetToZero ();


    //  InitialLoad:
    //      Initially loads lock info information.

    void InitialLoad ();


    //  SetValue:
    //      The function allocates memory for a string and initializes it with
    //      the given value.

    wchar_t *SetValue (FdoString *value_buffer);


    //  ~FdoRdbmsLongTransactionLtQueryHandler:
    //      Class destructor.

    virtual ~FdoRdbmsLongTransactionReader ();


    //  Dispose:
    //      Class dispose operator.

    virtual void Dispose () { delete this; }


    // ************************************************************************
    // *                      Protected Member Variables                      *
    // ************************************************************************

    //  fdo_connection:
    //      References the FDO connection.

    FdoRdbmsConnection *fdo_connection;


  private:

    // ************************************************************************
    // *                       Private Member Variables                       *
    // ************************************************************************

    //  query_request:
    //      Indicates the kind of request that is issued.

    FdoRdbmsLongTransactionDataRequestType query_request;


    //  lt_info_loaded:
    //      A flag indicating whether or not the query to retrieve the requested
    //      information has been initiated.

    bool lt_info_loaded;

    //  reader_positioned:
    //      A flag set to TRUE if the reader has been positioned (an initial
    //      ReadNext operation has been called). Otherwise, the flag is set to
    //      FALSE.

    bool reader_positioned;

    //  valid_reader_position:
    //      A flag indicating whether or not the current reader position is
    //      valid.

    bool valid_reader_position;


    //  cur_active_lt:
    //      Holds the name of the currently active long transaction.

    FdoString *cur_active_lt;


    //  lt_name:
    //      A variable to store retrieved information in.

    FdoString *lt_name;


    //  lt_query_handler:
    //      References the long transaction query handler used to retrieve the
    //      requested data.

    FdoRdbmsLongTransactionLtQueryHandler *lt_query_handler;


};  //  class FdoRdbmsLongTransactionReader

#endif
