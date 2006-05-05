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
// | The file contains the implementation of the class FdoRdbmsLockConflict-
// | Reader. The class implements functions for the lock conflict reader.
// =============================================================================

// The links to the header files to be included.

#include "stdafx.h"
#include <malloc.h>

#include "FdoRdbmsLockInfoReader.h"
#include "LockUtility.h"
//#include "inc/adb/col_names.h"
#include "Inc/ut.h"
#include "FdoCommonOSUtil.h"

// Constant definitions

#define WSM_S  "shared"
#define WSM_E  "exclusive"
#define WSM_WE "workspace exclusive"
#define WSM_VE "version exclusive"

FdoRdbmsLockInfoReader::FdoRdbmsLockInfoReader ()

// +---------------------------------------------------------------------------
// | The function represents the class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the member variables.

    SetToZero();

    // Create the identity collection.

    identity_collection = LockUtility::CreateIdentityCollection();

}  //  FdoRdbmsLockInfoReader ()

FdoRdbmsLockInfoReader::FdoRdbmsLockInfoReader (
                                            FdoRdbmsConnection *fdoConnection,
											char		   *className,
                                            char           *sqlFilter,
                                            char           *tableName)

// +---------------------------------------------------------------------------
// | The function represents the class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the member variables.

    SetToZero();

    dbi_connection = NULL;
    fdo_connection = fdoConnection;
    if (fdo_connection) {

        fdo_connection->AddRef();
        dbi_connection = fdo_connection->GetDbiConnection();

    }  //  if (fdo_connection) ...

    // Assign the given parameters to the corresponding class variables.

	if (sqlFilter)
		sql_filter      = LockUtility::ConvertString(sqlFilter);
    request_class_name = LockUtility::ConvertString(className);
	request_table_name = LockUtility::ConvertString(tableName);

    // Create the identity collection.

    identity_collection = LockUtility::CreateIdentityCollection();

}  //  FdoRdbmsLockInfoReader ()

FdoRdbmsLockInfoReader::~FdoRdbmsLockInfoReader()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Close the lock info reader first before destroying it.

    Close();

    if (fdo_connection)
        fdo_connection->Release();

}  //  ~FdoRdbmsLockInfoReader ()

FdoString *FdoRdbmsLockInfoReader::GetFeatureClassName ()

// +---------------------------------------------------------------------------
// | The function returns the class name for the object currently read.
// +---------------------------------------------------------------------------

{

    // Return the class name given to the reader when created.
    // NOTE: If the member variable holding the given class name identifies
    //       the keyword "FEATURE", special processing is required. This is
    //       not implemented at this point and hence an error is returned.

    if (dbi_connection->GetUtility()->StrCmp(request_class_name,
                                             L"FEATURE"         ) == 0) {

        FreeMemory(TRUE);
        throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                   LockUtility::CLASS_NAME_RETRIEVAL_FAILURE));

    }  //  if (dbi_connection->GetUtility()-> ...
    else
      return(request_class_name);

}  //  GetFeatureClassName ()

FdoString *FdoRdbmsLockInfoReader::GetLongTransaction ()

// +---------------------------------------------------------------------------
// | The function returns the long transaction name for the object currently
// | read.
// +---------------------------------------------------------------------------

{
	validQuery();

    return(current_lt_name);

}  //  GetLongTransaction ()

FdoPropertyValueCollection *FdoRdbmsLockInfoReader::GetIdentity ()

// +---------------------------------------------------------------------------
// | The function returns the identity for the object currently read.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    int                         i,
                                count;

    FdoPropertyValue            *property_value             = NULL;

    FdoPropertyValueCollection *property_value_collection   = NULL;

	validQuery();

    try {

      // Create a property value collection object.

      if ((property_value_collection =
                            LockUtility::CreateIdentityCollection()) == NULL) {

           FreeMemory(TRUE);
           throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                   LockUtility::IDENTITY_RETRIEVAL_FAILURE));

      }  //  if ((property_value_collection = ...

      // Copy all the entries from the internal identity collection to this
      // newly created one.

      if (identity_collection == NULL) return property_value_collection;

      count = identity_collection->GetCount();
      for (i = 0; i < count; i++) {

        property_value = identity_collection->GetItem(i);
        property_value_collection->Add(property_value);
        property_value->Release();

      }  //  for (i = 0; ...

      return property_value_collection;

    }  //  try ...

    catch (FdoException *ex) {

      FreeMemory(TRUE);

      FdoCommandException *fdo_cmd_ex = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
      ex->Release();
      throw fdo_cmd_ex;

    }  //  catch (FdoException *ex)

    catch ( ... ) {

      FreeMemory(TRUE);
      throw;

    }  //  catch ( ... ) ...

}  //  GetIdentity ()

FdoString *FdoRdbmsLockInfoReader::GetLockOwner ()

// +---------------------------------------------------------------------------
// | The function returns the lock owner for the object currently read.
// +---------------------------------------------------------------------------

{
	validQuery();

    return (current_lock_owner);

}  //  GetLockOwner ()

const FdoLockType FdoRdbmsLockInfoReader::GetLockType ()

// +---------------------------------------------------------------------------
// | The function returns the lock type for the object currently read.
// +---------------------------------------------------------------------------

{

	validQuery();
    // Return the FDO lock type reptresentation that corresponds to the current
    // lock type.

    return (
       (current_lock_type == NULL)
         ? FdoLockType_None
		 : (((FdoCommonOSUtil::stricmp(current_lock_type, WSM_S ) == 0) ||
		 (FdoCommonOSUtil::stricmp(current_lock_type, "s") == 0)    )
              ? FdoLockType_Shared
			  : ((FdoCommonOSUtil::stricmp(current_lock_type, WSM_E)  == 0)
                  ? FdoLockType_AllLongTransactionExclusive
				  : (((FdoCommonOSUtil::stricmp(current_lock_type, WSM_WE) == 0) ||
				  (FdoCommonOSUtil::stricmp(current_lock_type, "e") == 0)    )
                       ? FdoLockType_Exclusive
                       : (((FdoCommonOSUtil::stricmp(current_lock_type, WSM_VE) == 0) ||
                           (FdoCommonOSUtil::stricmp(current_lock_type, "v") == 0)    )
                            ? FdoLockType_LongTransactionExclusive
                            : FdoLockType_Unsupported)))));

}  //  GetLockType ()

bool FdoRdbmsLockInfoReader::ReadNext ()

// +---------------------------------------------------------------------------
// | The function reads the next lock info data set. It returns TRUE back to
// | the calling routine if there is no more data to be read, FALSE otherwise.
// +---------------------------------------------------------------------------

{

    // Declare all necessary local variables.

    ReadStatus          read_status;

    FdoCommandException *fdo_cmd_ex  = NULL;

    try {

      // If the lock conflict information hasn't been loaded yet retrieve the
      // data. If this fails terminate the routine.

      if (!lock_infos_loaded) {

          if (!InitialLoad())
              throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                   LockUtility::ERROR_LOADING_LOCK_INFO_DATA));

          lock_infos_loaded = TRUE;
		  mEOF = FALSE;

      }  //  if (!lock_conflicts_loaded) ...

      // Get the next available set of lock conflict data. If an error is
      // detected terminate the routine.
	  if (!mEOF)
	  {
		  read_status = GetNextDataSet();
		  if (read_status == END_OF_BUFFER)
		  {
			  mEOF = TRUE;
			  return FALSE;
		  }
		  if (read_status == SUCCESS) return TRUE;
	  }

      // At this point the read status is FAILURE and an exception must be
      // raised.

      throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                    LockUtility::READ_NEXT_LOCK_INFO_FAILURE));

    }  //  try ...

    catch (FdoException *ex) {

      FreeMemory(TRUE);

      fdo_cmd_ex = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
      ex->Release();
      throw fdo_cmd_ex;

    }  //  catch (FdoException *ex)

    catch ( ... ) {

      FreeMemory(TRUE);
      throw;

    }  // catch ( ... ) ...

}  //  ReadNext ()

void FdoRdbmsLockInfoReader::Close ()

// +---------------------------------------------------------------------------
// | The function closes the lock info reader.
// +---------------------------------------------------------------------------

{

    // If there is a query id still pending close it. Then reset the class
    // data members.

	lockInfoQueryHandler->Close();
    FreeMemory(TRUE);
    SetToZero();

}  //  Close ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

bool FdoRdbmsLockInfoReader::InitialLoad ()

// +---------------------------------------------------------------------------
// | The function loads the lock info information into the DBI cache.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool execution_status = TRUE;

	lockInfoQueryHandler = FdoPtr<FdoRdbmsLockManager>(fdo_connection->GetLockManager())->LockedInfoQuery(request_table_name, sql_filter);

    return execution_status;

}  //  InitialLoad ()

FdoRdbmsLockInfoReader::ReadStatus FdoRdbmsLockInfoReader::GetNextDataSet ()

// +---------------------------------------------------------------------------
// | The function controls the reading of the next lock information data set
// | from the DBI cache.
// +---------------------------------------------------------------------------

{
	FdoRdbmsPrimaryKeyColumn *curr_pky_col;
	FdoPropertyValue		 *identity_value;

	try 
	{
		FreeMemory(false);
		if (!lockInfoQueryHandler->ReadNext())
			return END_OF_BUFFER;

		if (lockInfoQueryHandler->GetClassId() != -1)
			current_class_id = LockUtility::SetValue((const char*)FdoStringP::Format(L"%ld",lockInfoQueryHandler->GetClassId()));
		if (lockInfoQueryHandler->GetLockType() != NULL)
			current_lock_type = (char *)LockUtility::SetValue((const char*)lockInfoQueryHandler->GetLockType());
		if (lockInfoQueryHandler->GetLockOwner() != NULL)
			current_lock_owner  = (FdoString *)LockUtility::SetValue((const wchar_t*)lockInfoQueryHandler->GetLockOwner());
		FdoRdbmsPrimaryKey *current_primary_key = lockInfoQueryHandler->GetPrimaryKey();
		for (int i=0; i<current_primary_key->GetCount(); i++)
		{
			curr_pky_col = current_primary_key->GetItem(i);
			if (LockUtility::GetColumnToken((char *)curr_pky_col->GetColumnName()) == 
											 LockUtility::IDENTITY_COLUMN)

				if ((identity_value =
						   LockUtility::CreateIdentity(
										   fdo_connection,
										   curr_pky_col->GetColumnName(),
										   curr_pky_col->GetColumnValue(),
										   request_class_name    )) == NULL)
					return FAILURE;
			   identity_collection->Add(identity_value);
			   identity_value->Release();
		}

      return SUCCESS;

    } //  try ...

    catch (FdoException *ex) {

      FreeMemory(TRUE);

      FdoCommandException *fdo_cmd_ex = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
      ex->Release();
      throw fdo_cmd_ex;

    }  //  catch (FdoException *ex)

    catch ( ... ) {

      FreeMemory(TRUE);
      throw;

    }  //  catch ...
}  //  GetNextDataSet ()

void FdoRdbmsLockInfoReader::FreeMemory (bool include_members)

// +---------------------------------------------------------------------------
// | The function frees memory.
// +---------------------------------------------------------------------------

{

    if (current_lt_name    != NULL) delete[] current_lt_name;
    if (current_class_id   != NULL) delete[] current_class_id;
    if (current_lock_type  != NULL) delete[] current_lock_type;
    if (current_lock_owner != NULL) delete[] current_lock_owner;

    if (identity_collection != NULL) identity_collection->Clear();

    current_lt_name    = NULL;
    current_class_id   = NULL;
    current_lock_type  = NULL;
    current_lock_owner = NULL;

    if (include_members) {

        if (identity_collection != NULL) identity_collection->Release();
        if (sql_filter       != NULL) delete[] sql_filter;
        if (request_class_name  != NULL) delete[] request_class_name;

        identity_collection = NULL;
        sql_filter       = NULL;
        request_class_name  = NULL;

    }  //  if (include_members) ...

}  //  FreeMemory ()

void FdoRdbmsLockInfoReader::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    sql_filter       = NULL;
    current_lt_name     = NULL;
    current_class_id    = NULL;
    current_lock_type   = NULL;
    request_class_name  = NULL;
	request_table_name  = NULL;
    current_lock_owner  = NULL;

    lock_infos_loaded   = FALSE;

    identity_collection = NULL;
	mEOF = TRUE;

}  //  SetToZero ()

void FdoRdbmsLockInfoReader::validQuery()
{
	if (!lock_infos_loaded || mEOF)
		throw FdoCommandException::Create(
							 NlsMsgGet(FDORDBMS_453,
							 "Lock Info Reader query is not active."));
}

