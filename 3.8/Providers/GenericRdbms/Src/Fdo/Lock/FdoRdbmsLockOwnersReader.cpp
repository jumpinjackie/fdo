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
// | The file contains the implementation of the class FdoRdbmsLockOwnersReader.
// | The class implements functions for the lock owner reader.
// =============================================================================

// The links to the header files to be included.

#include "stdafx.h"
#include <malloc.h>

#include "FdoRdbmsLockOwnersReader.h"
#include "LockUtility.h"
#include "FdoRdbmsException.h"
#include "LockManager.h"


FdoRdbmsLockOwnersReader::FdoRdbmsLockOwnersReader ()

// +---------------------------------------------------------------------------
// | The function represents the class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the member variables.

    SetToZero();

}  //  FdoRdbmsLockOwnersReader ()

FdoRdbmsLockOwnersReader::FdoRdbmsLockOwnersReader (
                                              FdoIConnection *a_dbi_connection)

// +---------------------------------------------------------------------------
// | The function represents the class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the member variables.

    SetToZero();
    dbi_connection = NULL;
    fdo_connection = dynamic_cast<FdoRdbmsConnection*>(a_dbi_connection);
    if (fdo_connection) {

        fdo_connection->AddRef();
        dbi_connection = fdo_connection->GetDbiConnection();

    }  //  if (fdo_connection) ...

}  //  FdoRdbmsLockOwnersReader ()

FdoRdbmsLockOwnersReader::~FdoRdbmsLockOwnersReader ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Close the lock conflict reader first before destroying it.

    Close();

    if (fdo_connection)
        fdo_connection->Release();

}  //  ~FdoRdbmsLockConflictReader ()

FdoString *FdoRdbmsLockOwnersReader::GetLockOwner ()

// +---------------------------------------------------------------------------
// | The function returns the lock owner for the currently read lock conflict
// | object.
// +---------------------------------------------------------------------------

{

	return mLockOwnersQueryHandler->GetLockOwner();

}  //  GetLockOwner ()

bool FdoRdbmsLockOwnersReader::ReadNext ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

      if (!lock_owners_loaded) {

		  mLockOwnersQueryHandler = fdo_connection->GetLockManager()->OwnersQuery();

          if (mLockOwnersQueryHandler == NULL)
              throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                                LockUtility::ERROR_LOADING_OWNER_INFORMATION));

          lock_owners_loaded = TRUE;

      }  //  if (!lock_owners_loaded) ...


	  return mLockOwnersQueryHandler->ReadNext();

}  //  ReadNext ()

void FdoRdbmsLockOwnersReader::Close ()

// +---------------------------------------------------------------------------
// | The function closes the lock conflict reader.
// +---------------------------------------------------------------------------

{

    // If there is a query id still pending close it. Then reset the class
    // data members.

	if (mLockOwnersQueryHandler)
		mLockOwnersQueryHandler->Close();

    SetToZero();

}  //  Close ()


// ----------------------------------------------------------------------------
// --                          Supporting functions                          --
// ----------------------------------------------------------------------------

void FdoRdbmsLockOwnersReader::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    lock_owners_loaded = FALSE;
	mLockOwnersQueryHandler = NULL;


}  //  SetToZero ()
