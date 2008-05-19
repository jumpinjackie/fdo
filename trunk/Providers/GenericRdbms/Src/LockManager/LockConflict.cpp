//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

// =============================================================================
// | The file contains the implementation of the class LockConflict. The class
// | implements functions used to store a lock conflict data set.
// =============================================================================

// The links to the header files to be included.

#include "stdafx.h"
#include <malloc.h>

#include "LockConflict.h"

#include "LockUtility.h"
#include "FdoRdbmsException.h"
#include "Inc/ut.h"
#include "FdoCommonOSUtil.h"

// Define some constants.

#define CVT_STRING_LEN 50

#define AGDS_S "s"
#define AGDS_E "e"
#define AGDS_V "v"
#define WSM_S  "shared"
#define WSM_E  "exclusive"
#define WSM_WE "workspace exclusive"
#define WSM_VE "version exclusive"


FdoRdbmsLockConflict::FdoRdbmsLockConflict (FdoRdbmsConnection *a_dbi_connection)

// +---------------------------------------------------------------------------
// | The function represents the class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the member variables.

    SetToZero();

    // Assign the given parameters to the corresponding class variables.

    dbi_connection = a_dbi_connection;

    // Create the identity collection.

    identities = LockUtility::CreateIdentityCollection();

}  //  FdoRdbmsLockConflict ()

FdoRdbmsLockConflict::~FdoRdbmsLockConflict ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Free used memory.

    FreeMemory();

}  //  ~FdoRdbmsLockConflict ()

FdoString *FdoRdbmsLockConflict::GetFeatureClassName ()

// +---------------------------------------------------------------------------
// | The function returns the value of the class name property.
// +---------------------------------------------------------------------------

{

    return (class_name) ;

}  //  GetClassName ()

FdoPropertyValueCollection *FdoRdbmsLockConflict::GetIdentity ()

// +---------------------------------------------------------------------------
// | The function returns a handle to the identity property collection.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    int                         i,
                                count;

    FdoPropertyValue            *property_value             = NULL;

    FdoPropertyValueCollection  *property_value_collection  = NULL;

    // Create a property value collection object.

    if ((property_value_collection =
                            LockUtility::CreateIdentityCollection()) == NULL) {

         FreeMemory();
         throw FdoCommandException::Create(
                    LockUtility::GetExceptionMessage(
                                    LockUtility::IDENTITY_RETRIEVAL_FAILURE));

    }  //  if ((property_value_collection = ...

    // Copy all the entries from the internal identity collection to this newly
    // created one.

    if (identities == NULL) return property_value_collection;

    count = identities->GetCount();
    for (i = 0; i < count; i++) {

      property_value = identities->GetItem(i);
      property_value_collection->Add(property_value);
      property_value->Release();

    }  //  for (i = 0; ...

    return property_value_collection;

}  //  GetIdentity ()

FdoString *FdoRdbmsLockConflict::GetLockOwner ()

// +---------------------------------------------------------------------------
// | The function returns the value of the lock owner property.
// +---------------------------------------------------------------------------

{

    return (lock_owner);

}  //  GetLockOwner ()

FdoString *FdoRdbmsLockConflict::GetLongTransaction ()

// +---------------------------------------------------------------------------
// | The function returns the value of the long transaction property.
// +---------------------------------------------------------------------------

{

    return (long_transaction);

}  //  GetLongTransaction ()

FdoLockType FdoRdbmsLockConflict::GetLockType ()

// +---------------------------------------------------------------------------
// | The function returns the lock type property.
// +---------------------------------------------------------------------------

{

      return (lock_type);

}  //  GetLockType ()

FdoConflictType FdoRdbmsLockConflict::GetConflictType ()

// +---------------------------------------------------------------------------
// | The function returns the conflict type property.
// +---------------------------------------------------------------------------

{

      return ((lock_owner == NULL)
                ? FdoConflictType_VersionConflict
                : FdoConflictType_LockConflict);

}  //  GetConflictType ()

void FdoRdbmsLockConflict::AddIdentity ( FdoString *property_name,
                                         FdoString *property_value)

// +---------------------------------------------------------------------------
// | The function creates an identity object based on the given data and adds
// | it to the identity collection.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoPropertyValue *identity = NULL;

    try {

      // Create the identity object. If it fails throw an exception. Otherwise
      // add the identity object to the collection of identities.

      if ((identity = LockUtility::CreateIdentity(dbi_connection,
                                                  property_name,
                                                  property_value,
                                                  class_name     )) == NULL)
           throw FdoRdbmsException::Create(
                                LockUtility::GetExceptionMessage(
                                     LockUtility::IDENTITY_RETRIEVAL_FAILURE));

      identities->Add(identity);
      identity->Release();

    }  //  try ...

    catch ( ... ) {

      if (identity != NULL) { identity->Release(); identity = NULL; }
      FreeMemory();
      throw;

    }  //  catch  ...

}  //  AddIdentity ()

void FdoRdbmsLockConflict::SetClassName (FdoString *lk_class_name)

// +---------------------------------------------------------------------------
// | The function sets the class name property for a lock conflict.
// +---------------------------------------------------------------------------

{

    // Set the class name property for the current object. If the class name
    // is already set, delete it first.

    if (class_name != NULL) { delete[] class_name; class_name = NULL; }
    class_name = new wchar_t [wcslen(lk_class_name) + 1];
	if (class_name != NULL)
		wcscpy((wchar_t*)class_name, lk_class_name);

}  //  SetClassName ()

void FdoRdbmsLockConflict::SetOwner (FdoString *lk_owner)

// +---------------------------------------------------------------------------
// | The function sets the owner property for a lock conflict.
// +---------------------------------------------------------------------------

{

	// Set the owner property for the current object. If the owner is already
	// set, delete it first.

	if (lock_owner != NULL) 
	{ 
		delete[] lock_owner; 
		lock_owner = NULL; 
	}
	lock_owner = new wchar_t[wcslen(lk_owner) + 1];
	if (lock_owner != NULL)
		wcscpy((wchar_t*)lock_owner, lk_owner);

}  //  SetOwner ()

void FdoRdbmsLockConflict::SetLongTransaction (FdoString *lk_lt_name)

// +---------------------------------------------------------------------------
// | The function sets the long transaction property for a lock conflict.
// +---------------------------------------------------------------------------

{
      // Set the long transaction property for the current object. If the long
      // transaction property is already set, delete it first.

      if (long_transaction != NULL) {

          delete[] long_transaction;
          long_transaction = NULL;

      }  //  if (long_transaction != NULL) ...

	  long_transaction = new wchar_t[wcslen(lk_lt_name)+1];
	  if (long_transaction != NULL)
		  wcscpy((wchar_t*)long_transaction, lk_lt_name);

}  //  SetLongTransaction ()

void FdoRdbmsLockConflict::SetLockType (FdoString *lk_type)

// +---------------------------------------------------------------------------
// | The function sets the lock type property for a lock conflict.
// +---------------------------------------------------------------------------

{

    // The lock type being reported is a string. It is translated into the cor-
    // responding FDO lock type constant and stored with the current object.

    lock_type = ((lk_type == NULL)
                  ? FdoLockType_None
				  : (((FdoRdbmsUtil::StrCmp(lk_type, L"WSM_S" ) == 0) ||
					  (FdoRdbmsUtil::StrCmp(lk_type, L"AGDS_S") == 0)    )
                       ? FdoLockType_Shared
					   : ((FdoRdbmsUtil::StrCmp(lk_type, L"WSM_E")  == 0)
                           ? FdoLockType_AllLongTransactionExclusive
						   : (((FdoRdbmsUtil::StrCmp(lk_type, L"WSM_WE") == 0) ||
						   (FdoRdbmsUtil::StrCmp(lk_type, L"AGDS_E") == 0)    )
                                ? FdoLockType_Exclusive
                                : (((FdoRdbmsUtil::StrCmp(lk_type, L"WSM_VE") == 0) ||
                                    (FdoRdbmsUtil::StrCmp(lk_type, L"AGDS_V") == 0)    )
                                     ? FdoLockType_LongTransactionExclusive
                                     : FdoLockType_Unsupported)))));

}  //  SetLockType ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoRdbmsLockConflict::FreeMemory ()

// +---------------------------------------------------------------------------
// | The function frees memory.
// +---------------------------------------------------------------------------

{

    if (lock_owner       != NULL) 
		delete[] lock_owner;

	lock_owner = NULL;
    if (class_name       != NULL) 
		delete[] class_name;
	class_name = NULL;

    if (long_transaction != NULL) 
		delete[] long_transaction;

	long_transaction = NULL;

    if (identities != NULL) 
        identities->Clear();

}  //  FreeMemory ()

void FdoRdbmsLockConflict::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    dbi_connection   = NULL;

    lock_owner       = NULL;
    class_name       = NULL;
    long_transaction = NULL;

    identities       = NULL;

}  //  SetToZero ()
