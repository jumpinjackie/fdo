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
// | The file contains the implementation of the class FdoRdbmsLongTransaction-
// | Reader. The class implements functions for the long transaction reader.
// =============================================================================

// The links to the header files to be included.

#include "stdafx.h"
#include <malloc.h>

#include "FdoRdbmsLongTransactionReader.h"
#include "Inc/ut.h"


// ----------------------------------------------------------------------------
// --                         Constructors/Destructors                       --
// ----------------------------------------------------------------------------

FdoRdbmsLongTransactionReader::FdoRdbmsLongTransactionReader ()

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the member variables.

    SetToZero();

}  //  FdoRdbmsLongTransactionReader ()

FdoRdbmsLongTransactionReader::FdoRdbmsLongTransactionReader (
                    FdoIConnection                         *a_fdo_connection,
                    FdoString                              *the_lt_name,
                    FdoRdbmsLongTransactionDataRequestType the_query_request)

// +---------------------------------------------------------------------------
// | The function represents a class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the member variables.

    SetToZero();

    // Assign the given parameters to the corresponding class variables.

    fdo_connection = dynamic_cast<FdoRdbmsConnection*>(a_fdo_connection);
    if (fdo_connection)
        fdo_connection->AddRef();

    if (the_lt_name != NULL)
        if ((lt_name = SetValue(the_lt_name)) == NULL)
             throw FdoCommandException::Create(
                           NlsMsgGet(FDORDBMS_91, "Failed to allocate memory"));
    query_request = the_query_request;

}  //  FdoRdbmsLongTransactionReader ()

FdoRdbmsLongTransactionReader::~FdoRdbmsLongTransactionReader ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Close the long transaction reader first before destroying it.

    Close();

    if (fdo_connection)
        fdo_connection->Release();

}  //  ~FdoRdbmsLongTransactionReader ()


// ----------------------------------------------------------------------------
// --                            Public Interfaces                           --
// ----------------------------------------------------------------------------

FdoString *FdoRdbmsLongTransactionReader::GetName ()

// +---------------------------------------------------------------------------
// | The function returns the name of the long transaction currently read in
// | the reader back to the caller.
// +---------------------------------------------------------------------------

{

    // If the reader has not been positioned correctly issue an error message.
    // Otherwise return the requested information.

    if ((!reader_positioned) || (!valid_reader_position))
        throw FdoCommandException::Create(
                     NlsMsgGet1(FDORDBMS_383,
                                "Reader '%1$ls' is not positioned correctly",
                                L"FdoILongTransactionReader"));

    return lt_query_handler->GetName();

}  //  GetName ()

FdoString *FdoRdbmsLongTransactionReader::GetDescription ()

// +---------------------------------------------------------------------------
// | The function returns the description of the long transaction currently
// | read in the reader back to the caller.
// +---------------------------------------------------------------------------

{

    // If the reader has not been positioned correctly issue an error message.
    // Otherwise return the requested information.

    if ((!reader_positioned) || (!valid_reader_position))
        throw FdoCommandException::Create(
                     NlsMsgGet1(FDORDBMS_383,
                                "Reader '%1$ls' is not positioned correctly",
                                L"FdoILongTransactionReader"));

    // If the description is empty return a NULL pointer.

    return lt_query_handler->GetDescription();

}  //  GetDescription ()

FdoString *FdoRdbmsLongTransactionReader::GetOwner ()

// +---------------------------------------------------------------------------
// | The function returns the name of the owner of the long transaction cur-
// | rently read in the reader back to the caller.
// +---------------------------------------------------------------------------

{

    // If the reader has not been positioned correctly issue an error message.
    // Otherwise return the requested information.

    if ((!reader_positioned) || (!valid_reader_position))
        throw FdoCommandException::Create(
                     NlsMsgGet1(FDORDBMS_383,
                                "Reader '%1$ls' is not positioned correctly",
                                L"FdoILongTransactionReader"));

    return lt_query_handler->GetOwner();

}  //  GetOwner ()

FdoDateTime FdoRdbmsLongTransactionReader::GetCreationDate ()

// +---------------------------------------------------------------------------
// | The function returns date the long transaction currently read in the
// | reader was created back to the caller.
// +---------------------------------------------------------------------------

{

    // If the reader has not been positioned correctly issue an error message.
    // Otherwise return the requested information.

    if ((!reader_positioned) || (!valid_reader_position))
        throw FdoCommandException::Create(
                     NlsMsgGet1(FDORDBMS_383,
                                "Reader '%1$ls' is not positioned correctly",
                                L"FdoILongTransactionReader"));

    return lt_query_handler->GetCreationDate();

}  //  GetCreationDate ()

FdoILongTransactionReader *FdoRdbmsLongTransactionReader::GetChildren ()

// +---------------------------------------------------------------------------
// | The function returns a long transaction reader with all the descendent
// | long transactions for the long transaction currently read.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoRdbmsLongTransactionReader *long_transaction_reader = NULL;

    // If the reader has not been positioned correctly issue an error message.
    // Otherwise return the requested information.

    if ((!reader_positioned) || (!valid_reader_position))
        throw FdoCommandException::Create(
                     NlsMsgGet1(FDORDBMS_383,
                                "Reader '%1$ls' is not positioned correctly",
                                L"FdoILongTransactionReader"));

    // Create the long transaction reader to retrieve the requested data.

    if ((long_transaction_reader =
           new FdoRdbmsLongTransactionReader(
                fdo_connection,
                lt_query_handler->GetName(),
                FdoRdbmsLongTransactionDataRequestType_Children)) == NULL) {

         ClearMemory();
         throw FdoCommandException::Create(
                          NlsMsgGet(FDORDBMS_87,
                                    "Failed to create a lock object reader"));

    }  //  if ((long_transaction_reader = ...

    return long_transaction_reader;

}  //  GetChildren ()

FdoILongTransactionReader *FdoRdbmsLongTransactionReader::GetParents ()

// +---------------------------------------------------------------------------
// | The function returns a long transaction reader with all the ascdendant
// | long transactions for the long transaction currently read.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoRdbmsLongTransactionReader *long_transaction_reader = NULL;

    // If the reader has not been positioned correctly issue an error message.
    // Otherwise return the current value.

    if ((!reader_positioned) || (!valid_reader_position))
        throw FdoCommandException::Create(
                     NlsMsgGet1(FDORDBMS_383,
                                "Reader '%1$ls' is not positioned correctly",
                                L"FdoILongTransactionReader"));

    // Create the long transaction reader to retrieve the requested data.

    if ((long_transaction_reader =
            new FdoRdbmsLongTransactionReader(
                    fdo_connection,
                    lt_query_handler->GetName(),
                    FdoRdbmsLongTransactionDataRequestType_Parents)) == NULL) {

         ClearMemory();
         throw FdoCommandException::Create(
                          NlsMsgGet(FDORDBMS_87,
                                    "Failed to create a lock object reader"));

    }  //  if ((long_transaction_reader = ...

    return long_transaction_reader;

}  //  GetParents ()

bool FdoRdbmsLongTransactionReader::IsActive ()

// +---------------------------------------------------------------------------
// | The function returns TRUE back to the caller if the long transaction cur-
// | rently read is the active one for the current user, FALSE otherwise.
// +---------------------------------------------------------------------------

{

    // If the reader has not been positioned correctly issue an error message.
    // Otherwise return the current value.

    if ((!reader_positioned) || (!valid_reader_position))
        throw FdoCommandException::Create(
                     NlsMsgGet1(FDORDBMS_383,
                                "Reader '%1$ls' is not positioned correctly",
                                L"FdoILongTransactionReader"));

    // Compare the current long transaction name to the name of the active long
    // transaction as detetermined when the reader was executed. If the names
    // match, return TRUE, FALSE otherwise.

    return lt_query_handler->GetIsActiveFlag();

}  //  IsActive ()

bool FdoRdbmsLongTransactionReader::IsFrozen ()

// +---------------------------------------------------------------------------
// | The function returns TRUE back to the caller if the long transaction cur-
// | rently read is frozen, FALSE otherwise.
// +---------------------------------------------------------------------------

{

    // If the reader has not been positioned correctly issue an error message.
    // Otherwise return the current value.

    if ((!reader_positioned) || (!valid_reader_position))
        throw FdoCommandException::Create(
                     NlsMsgGet1(FDORDBMS_383,
                                "Reader '%1$ls' is not positioned correctly",
                                L"FdoILongTransactionReader"));

    return (lt_query_handler->GetFreezeMode() != NULL);

}  //  IsFrozen ()

bool FdoRdbmsLongTransactionReader::ReadNext ()

// +---------------------------------------------------------------------------
// | The function reads the next long transaction info data set. It returns
// | TRUE back to the calling routine if there is no more data to be read,
// | FALSE otherwise.
// +---------------------------------------------------------------------------

{

    try {

      // If the long transaction data hasn't been requested yet, issue the
      // request.

      if (!lt_info_loaded) {

          InitialLoad();
          lt_info_loaded = true;

          reader_positioned     = true;
          valid_reader_position = true;

      }  //  if (!lt_infos_loaded) ...

      // Position the query handler reader to be ready to get the first set
      // of data.

      try {

        if (!lt_query_handler->ReadNext()) {

            reader_positioned     = false;
            valid_reader_position = false;
            return false;

        }  //  if (!lt_query_handler->ReadNext()) ...

      }  //  try ...

      catch ( ... ) {

        throw FdoCommandException::Create(
               NlsMsgGet(FDORDBMS_360,
                         "Failed to read next long transaction info data set"));

      }  //  catch ...

      return true;

    }  //  try ...

    catch (FdoCommandException *ex) {

      ex;
      ClearMemory();
      throw;

    }  //  catch (FdoCommandException *ex)

    catch (FdoException *ex) {

      ClearMemory();
      throw FdoCommandException::Create(ex->GetExceptionMessage(), ex);

    }  //  catch (FdoException *ex)

    catch ( ... ) {

      ClearMemory();
      throw;

    }  // catch ( ... ) ...

}  //  ReadNext ()

void FdoRdbmsLongTransactionReader::Close ()

// +---------------------------------------------------------------------------
// | The function closes the lock info reader.
// +---------------------------------------------------------------------------

{

    ClearMemory();

}  //  Close ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoRdbmsLongTransactionReader::InitialLoad ()

// +---------------------------------------------------------------------------
// | The function issues the query to retrieve the requested information.
// +---------------------------------------------------------------------------

{

    try {

      // Issue the query to retrieve the requested information and keep a
      // reference to the provided query handler.

      lt_query_handler = 
        FdoPtr<FdoRdbmsLongTransactionManager>(fdo_connection->GetLongTransactionManager())->LTQuery(lt_name,
                                                             query_request);

    }  //  try ...

    catch ( ... ) {

      throw FdoCommandException::Create(
                NlsMsgGet(
                    FDORDBMS_356,
                    "Error during initial load of long transaction info data"));

    }  //  catch ...

}  //  InitialLoad ()

void FdoRdbmsLongTransactionReader::ClearMemory ()

// +---------------------------------------------------------------------------
// | The function frees memory.
// +---------------------------------------------------------------------------

{

    if (lt_name          != NULL) delete[] lt_name;
    if (cur_active_lt    != NULL) delete[] cur_active_lt;
    if (lt_query_handler != NULL) {

        lt_query_handler->Close();
        lt_query_handler->Release();

    }  //  if (lt_query_handler != NULL) ...

    SetToZero();

}  //  ClearMemory ()

void FdoRdbmsLongTransactionReader::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    lt_name               = NULL;
    cur_active_lt         = NULL;
    lt_query_handler      = NULL;

    query_request         = FdoRdbmsLongTransactionDataRequestType_Data;

    lt_info_loaded        = false;
    reader_positioned     = false;
    valid_reader_position = false;

}  //  SetToZero ()

wchar_t *FdoRdbmsLongTransactionReader::SetValue (FdoString *value_buffer)

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
