/***************************************************************************
 * Copyright (C) 2004-2007  Autodesk, Inc.
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
// | The file contains the implementation of the class FdoRdbmsGetLongTransac-
// | tions. The class provides access to a collection of long transactions. De-
// | pending on the given flag the collection either contains only the infor-
// | mation for the user's active long transaction or all long transactions
// | currently available in the data store.
// =============================================================================

// The links to the header file(s) to be included.

#include "stdafx.h"
#include <malloc.h>

#include "FdoRdbmsGetLongTransactions.h"
#include "FdoRdbmsException.h"


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoRdbmsGetLongTransactions::FdoRdbmsGetLongTransactions ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsGetLongTransactions ()

FdoRdbmsGetLongTransactions::FdoRdbmsGetLongTransactions (
                                                    FdoIConnection *connection)
    : FdoRdbmsCommand<FdoIGetLongTransactions>(connection)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the class variables.

    SetToZero();

}  //  FdoRdbmsGetLongTransactions ()

FdoRdbmsGetLongTransactions::~FdoRdbmsGetLongTransactions ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    ClearMemory();

}  //  ~FdoRdbmsGetLongTransactions ()


// ----------------------------------------------------------------------------
// --                            Public Interfaces                           --
// ----------------------------------------------------------------------------

FdoString *FdoRdbmsGetLongTransactions::GetName ()

// +---------------------------------------------------------------------------
// | The function returns the value of the property holding the name of the
// | long transaction set for this command. The value could be a NULL pointer.
// +---------------------------------------------------------------------------

{

    return (lt_name);

}  //  GetName ()

void FdoRdbmsGetLongTransactions::SetName (FdoString *value)

// +---------------------------------------------------------------------------
// | The function sets the value of the property holding the name of the long
// | transaction used by this command. The value to be assigned to the property
// | could be a NULL pointer.
// +---------------------------------------------------------------------------

{

    // Check if the given long transaction name is valid.

    if (value != NULL)
        if ((wcslen(value) >  DBI_VERSION_NAME_LENGTH) ||
            (wcslen(value) == 0                      )    )
             throw FdoCommandException::Create(
                      NlsMsgGet2(
                        FDORDBMS_358, 
                        "%1$ls: Long transaction name ('%2$ls') too long (0 < name <= 30)",
                        L"FdoIGetLongTransactions",
                        value));

    // The basic tests to ensure the given long transaction name is valid have
    // been completed and the given name can be stored with the command.

    ClearMemory();
    if (value != NULL)
        if ((lt_name = SetValue(value)) == NULL)
             throw FdoCommandException::Create(
                           NlsMsgGet(FDORDBMS_91, "Failed to allocate memory"));

}  //  SetName ()

FdoILongTransactionReader *FdoRdbmsGetLongTransactions::Execute ()

// +---------------------------------------------------------------------------
// | The function executes a request to retrieve long transaction information
// | for either the named or for all available long transactions.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoCommandException           *fdo_cmd_ex              = NULL;

    FdoRdbmsLongTransactionReader *long_transaction_reader = NULL;

    try {

      // Retrieving the correct information is delayed until the user navigates
      // the reader. Therefore, all that needs to be done is to create the long
      // transaction reader.

      if ((long_transaction_reader =
                new FdoRdbmsLongTransactionReader(
                        FdoPtr<FdoIConnection>(GetConnection()),
                        lt_name,
                        FdoRdbmsLongTransactionDataRequestType_Data)) == NULL)
           throw FdoCommandException::Create(
                            NlsMsgGet(FDORDBMS_87,
                                      "Failed to create a lock object reader"));

      return long_transaction_reader;

    }  //  try ...

    catch (FdoException *ex) {

      ClearMemory();
      fdo_cmd_ex = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
      ex->Release();
      throw fdo_cmd_ex;

    }  //  catch (FdoException *ex)

    catch ( ... ) {

      ClearMemory();
      throw;

    }  //  catch ( ... ) ...

}  //  Execute ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoRdbmsGetLongTransactions::ClearMemory ()

// +---------------------------------------------------------------------------
// | The function frees memory.
// +---------------------------------------------------------------------------

{

    if (lt_name != NULL) delete[] lt_name;
    SetToZero();

}  //  FdoRdbmsGetLongTransactions ()

void FdoRdbmsGetLongTransactions::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    lt_name = NULL;

}  //  SetToZero ()

wchar_t *FdoRdbmsGetLongTransactions::SetValue (FdoString *value_buffer)

// +---------------------------------------------------------------------------
// | The function allocates memory for a string and initializes it with the
// | given value.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    wchar_t *value = NULL;

    // Allocate memory for the given buffer. If this fails terminate the
    // routine.

    if ((value = new wchar_t[wcslen(value_buffer) + 1]) == NULL) return value;
    wcscpy(value, value_buffer);
    return value;

}  //  SetValue ()
