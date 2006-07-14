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

#include "FdoCommonOSUtil.h"
#include "FdoRdbmsLockedObjectsReader.h"
#include "LockUtility.h"
//#include "inc/adb/col_names.h"

// Constant definitions

#define WSM_S  "shared"
#define WSM_E  "exclusive"
#define WSM_WE "workspace exclusive"
#define WSM_VE "version exclusive"

FdoRdbmsLockedObjectsReader::FdoRdbmsLockedObjectsReader ()

// +---------------------------------------------------------------------------
// | The function represents the class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the member variables.

    SetToZero();

    // Create the identity collection.

    identity_collection = LockUtility::CreateIdentityCollection();

}  //  FdoRdbmsLockedObjectsReader ()

FdoRdbmsLockedObjectsReader::FdoRdbmsLockedObjectsReader (
                                              FdoRdbmsConnection *a_dbi_connection,
                                              FdoString      *lk_owner)

// +---------------------------------------------------------------------------
// | The function represents the class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the member variables.

    SetToZero();

    dbi_connection = NULL;
    fdo_connection = a_dbi_connection;
    if (fdo_connection) {

        fdo_connection->AddRef();
        dbi_connection = fdo_connection->GetDbiConnection();

    }  //  if (fdo_connection) ...

    // Assign the given parameters to the corresponding class variables.

    requested_owner = LockUtility::SetValue(lk_owner);

    // Create the identity collection.

    identity_collection = LockUtility::CreateIdentityCollection();

}  //  FdoRdbmsLockedObjectsReader ()

FdoRdbmsLockedObjectsReader::~FdoRdbmsLockedObjectsReader ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Close the lock conflict reader first before destroying it.

    Close();

    if (fdo_connection)
        fdo_connection->Release();

}  //  ~FdoRdbmsLockedObjectsReader ()

FdoString *FdoRdbmsLockedObjectsReader::GetFeatureClassName ()

// +---------------------------------------------------------------------------
// | The function returns the class name for the object currently read.
// +---------------------------------------------------------------------------

{

    return (class_name);

}  //  GetFeatureClassName ()

FdoString *FdoRdbmsLockedObjectsReader::GetLongTransaction ()

// +---------------------------------------------------------------------------
// | The function returns the long transaction name for the object currently
// | read.
// +---------------------------------------------------------------------------

{

	if (lockInfoQueryHandler)
		return lockInfoQueryHandler->GetLtName();
	else
		return L"";

}  //  GetLongTransaction ()

FdoPropertyValueCollection *FdoRdbmsLockedObjectsReader::GetIdentity ()

// +---------------------------------------------------------------------------
// | The function returns the identity for the object currently read.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    int                         i,
                                count;

    FdoPropertyValue            *property_value             = NULL;

    FdoPropertyValueCollection  *property_value_collection  = NULL;

    try {

      // Create a property value collection object.

      if ((property_value_collection =
                            LockUtility::CreateIdentityCollection()) == NULL) {

           FreeMemory();
           throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                   LockUtility::IDENTITY_RETRIEVAL_FAILURE));

      }  //  if ((property_value_collection = ...

      // Copy all the entries from the internal identity collection to this
      // newly created one.

      if ( lockInfoQueryHandler->GetPrimaryKey() == NULL) return property_value_collection;

      count = lockInfoQueryHandler->GetPrimaryKey()->GetCount();
      for (i = 0; i < count; i++) {
		  
		  property_value = LockUtility::CreateIdentity(fdo_connection,
							lockInfoQueryHandler->GetPrimaryKey()->GetItem(i)->GetColumnName(),
						    lockInfoQueryHandler->GetPrimaryKey()->GetItem(i)->GetColumnValue(),
							class_name);
		  if (property_value)
		  {
			property_value_collection->Add(property_value);
			property_value->Release();
		  }

      }  //  for (i = 0; ...

      return property_value_collection;

    }  //  try ...

    catch (FdoException *ex) {

      FreeMemory();

      FdoCommandException *fdo_cmd_ex = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
      ex->Release();
      throw fdo_cmd_ex;

    }  //  catch (FdoException *ex)

    catch ( ... ) {

      FreeMemory();
      throw;

    }  //  catch ( ... ) ...

}  //  GetIdentity ()

FdoString *FdoRdbmsLockedObjectsReader::GetLockOwner ()

// +---------------------------------------------------------------------------
// | The function returns the lock owner for the object currently read.
// +---------------------------------------------------------------------------

{
    return(requested_owner);

}  //  GetLockOwner ()

const FdoLockType FdoRdbmsLockedObjectsReader::GetLockType ()

// +---------------------------------------------------------------------------
// | The function returns the lock type for the object currently read.
// +---------------------------------------------------------------------------

{

    // Return the FDO lock type reptresentation that corresponds to the current
    // lock type.

	if (lockInfoQueryHandler)
	{
		if (FdoCommonOSUtil::wcsicmp(lockInfoQueryHandler->GetLockType(), L"X") == 0) 
            return FdoLockType_AllLongTransactionExclusive;

		if (FdoCommonOSUtil::wcsicmp(lockInfoQueryHandler->GetLockType(), L"E") == 0)
			 return FdoLockType_Exclusive;

		if (FdoCommonOSUtil::wcsicmp(lockInfoQueryHandler->GetLockType(), L"V") == 0)
			 return FdoLockType_LongTransactionExclusive;

		if (FdoCommonOSUtil::wcsicmp(lockInfoQueryHandler->GetLockType(), L"S") == 0)
            return FdoLockType_Shared;   
	}

    return FdoLockType_Unsupported;

}  //  GetLockType ()

bool FdoRdbmsLockedObjectsReader::ReadNext ()

// +---------------------------------------------------------------------------
// | The function reads the next lock info data set. It returns TRUE back to
// | the calling routine if there is no more data to be read, FALSE otherwise.
// +---------------------------------------------------------------------------

{
    try {

      // Clear currently stored values.

      if (lt_name    != NULL) { delete[] lt_name;    lt_name    = NULL; }
      if (class_name != NULL) { delete[] class_name; class_name = NULL; }
      if (identity_collection->GetCount() > 0) identity_collection->Clear();

      // If the query to retrieve the information has not been started yet,
      // activate it.

      if (!mainQueryActive) {
		  tableQueryHandler = FdoPtr<FdoRdbmsLockManager>(fdo_connection->GetLockManager())->TableQuery(requested_owner);

          mainQueryActive = TRUE;
		  mIsEOFmainQ = FALSE;

      }  //  if (!query_activated) ...

	  if (mIsEOFmainQ == FALSE)
	  {
		  while (true)
		  {
			  if (mIsEOFsubQ == TRUE)
			  {
				  if (!tableQueryHandler->ReadNext())
				  {
					  mIsEOFmainQ = TRUE;
					  return false;
				  }
			  }
			  if (!ProcessNextRow())
					mIsEOFsubQ = TRUE;
			  else
				  return true;
		  }
	  }
	// not reached
	  return false;
    }  //  try 

    catch (FdoException *ex) {

      FreeMemory();

      FdoCommandException *fdo_cmd_ex = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
      ex->Release();
      throw fdo_cmd_ex;

    }  //  catch (FdoException *ex)

    catch ( ... ) {

      FreeMemory();
      throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                   LockUtility::LOCKED_OBJECTS_READ_ERROR));

    }  // catch ( ... ) ...

}  //  ReadNext ()

void FdoRdbmsLockedObjectsReader::Close ()

// +---------------------------------------------------------------------------
// | The function closes the lock info reader.
// +---------------------------------------------------------------------------

{

    // If there is still a DBI data communication object free the memory.

    if (lk_data_obj != NULL)
        dbi_connection->dbi_lock_terminate_lkd_query(&lk_data_obj);

    FreeMemory();
    SetToZero();

}  //  Close ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoRdbmsLockedObjectsReader::FreeMemory ()

// +---------------------------------------------------------------------------
// | The function frees memory.
// +---------------------------------------------------------------------------

{

    if (lt_name != NULL) {

        delete[] lt_name;
        lt_name = NULL;

    }  //  if (lt_name != NULL) ...

    if (class_name != NULL) {

        delete[] class_name;
        class_name = NULL;

    }  //  if (class_name != NULL) ...

    if (requested_owner != NULL) {

        delete[] requested_owner;
        requested_owner = NULL;

    }  //  if (requested_owner != NULL) ...

    if (identity_collection != NULL) {

        identity_collection->Clear();
        identity_collection->Release();
        identity_collection = NULL;

    }  //  if (identity_collection != NULL) ...

}  //  FreeMemory ()

void FdoRdbmsLockedObjectsReader::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    lk_data_obj         = NULL;

    lt_name             = NULL;
    class_name          = NULL;
    requested_owner     = NULL;

    identity_collection = NULL;

    mainQueryActive     = FALSE;
	subQueryActive		= FALSE;
	mIsEOFmainQ			= TRUE;
	mIsEOFsubQ			= TRUE;

}  //  SetToZero ()

bool FdoRdbmsLockedObjectsReader::ProcessNextRow()
{
	if (!subQueryActive)
	{
		lockInfoQueryHandler = 
			FdoPtr<FdoRdbmsLockManager>(fdo_connection->GetLockManager())->LockedObjectsQuery(tableQueryHandler->GetLockId(), 
														tableQueryHandler->GetDataStoreName(), 
														tableQueryHandler->GetTableName());
		mIsEOFsubQ = FALSE;
		subQueryActive = TRUE;
	}
	if (!mIsEOFsubQ)
	{
		if (!lockInfoQueryHandler->ReadNext())
		{
			lockInfoQueryHandler->Close();
			subQueryActive = FALSE;
			return false;
		}
		class_name = LockUtility::SetValue(LockUtility::GetClassName(fdo_connection, 
												  lockInfoQueryHandler->GetClassId(),
												  lockInfoQueryHandler->GetTableName()));
	
	}
	return true;
}

