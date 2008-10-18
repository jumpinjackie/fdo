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

#include "FdoRdbmsLockConflictReader.h"
#include "LockConflict.h"
#include "FdoRdbmsException.h"


FdoRdbmsLockConflictReader::FdoRdbmsLockConflictReader()

// +---------------------------------------------------------------------------
// | The function represents the class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the member variables.

    SetToZero();

}  //  FdoRdbmsLockConflictReader ()

FdoRdbmsLockConflictReader::FdoRdbmsLockConflictReader(
                                           FdoRdbmsConnection *fdoConnection,
										   FdoRdbmsLockConflictQueryHandler *conflictQueryHandler,
										   FdoIdentifier *confClassName):
										   fdo_connection(fdoConnection),
										   lockConflictQueryHandler(conflictQueryHandler)

// +---------------------------------------------------------------------------
// | The function represents the class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the member variables.

    SetToZero();
    dbi_connection = NULL;
    if (fdo_connection) {

        fdo_connection->AddRef();
        dbi_connection = fdo_connection->GetDbiConnection();

    }  //  if (fdo_coConnection) ...

	
    conflict_report_class_name =
                        FdoIdentifier::Create(confClassName->GetText());

}  //  FdoRdbmsLockConflictReader ()

FdoRdbmsLockConflictReader::FdoRdbmsLockConflictReader(
                                           FdoRdbmsConnection *fdoConnection,
										   FdoRdbmsLockConflictQueryHandler *conflictQueryHandler):
										   fdo_connection(fdoConnection),
										   lockConflictQueryHandler(conflictQueryHandler)

// +---------------------------------------------------------------------------
// | The function represents the class constructor.
// +---------------------------------------------------------------------------

{

    // Initialize the member variables.

    SetToZero();
    dbi_connection = NULL;
    if (fdo_connection) {

        fdo_connection->AddRef();
        dbi_connection = fdo_connection->GetDbiConnection();

    }  //  if (fdo_coConnection) ...

}  //  FdoRdbmsLockConflictReader ()

FdoRdbmsLockConflictReader::~FdoRdbmsLockConflictReader()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{

    // Close the lock conflict reader first before destroying it.

    Close();

    if (fdo_connection)
        fdo_connection->Release();

}  //  ~FdoRdbmsLockConflictReader ()

FdoString *FdoRdbmsLockConflictReader::GetFeatureClassName ()

// +---------------------------------------------------------------------------
// | The function returns the class name for the currently read lock conflict
// | object.
// +---------------------------------------------------------------------------

{
	if (conflict_report_class_name == NULL)	{
		checkValidReader();
		return lockConflictQueryHandler->GetFeatureClassName();
	}
	else
		return conflict_report_class_name->GetText();

}  //  GetFeatureClassName ()

FdoPropertyValueCollection *FdoRdbmsLockConflictReader::GetIdentity ()

// +---------------------------------------------------------------------------
// | The function returns the identity for the currently read lock conflict
// | object.
// +---------------------------------------------------------------------------

{
	checkValidReader();

    return (lockConflictQueryHandler->GetIdentity());

}  //  GetIdentity ()

FdoString *FdoRdbmsLockConflictReader::GetLockOwner ()
{
	checkValidReader();

	return lockConflictQueryHandler->GetLockOwner();

}  //  GetLockOwner ()

FdoConflictType FdoRdbmsLockConflictReader::GetConflictType ()

// +---------------------------------------------------------------------------
// | The function returns the long transaction in which the currently read lock
// | conflict object was locked.
// +---------------------------------------------------------------------------

{
	checkValidReader();

    return lockConflictQueryHandler->GetConflictType();

}  //  GetConflictType ()

FdoString *FdoRdbmsLockConflictReader::GetLongTransaction ()

// +---------------------------------------------------------------------------
// | The function returns the long transaction in which the currently read lock
// | conflict object was locked.
// +---------------------------------------------------------------------------

{

    // If the reader has not been positioned correctly issue an error message.
    // Otherwise return the requested information.
	checkValidReader();

	return lockConflictQueryHandler->GetLongTransaction();

}  //  GetLongTransaction ()

bool FdoRdbmsLockConflictReader::ReadNext ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{
    try {

		if (lockConflictQueryHandler)	
			validReader = lockConflictQueryHandler->ReadNext();

    }  //  try ...

    catch (FdoException *ex) {

		FdoCommandException *fdo_cmd_ex = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
		ex->Release();
		throw fdo_cmd_ex;

    }  //  catch (FdoException *ex)

    catch ( ... ) {

      throw;

    }  // catch ...

	return validReader;

}  //  ReadNext ()

void FdoRdbmsLockConflictReader::Close ()

// +---------------------------------------------------------------------------
// | The function closes the lock conflict reader.
// +---------------------------------------------------------------------------

{

    try {

      // Release all the memory occupied by the member variables.
		if (lockConflictQueryHandler != NULL)	{
			lockConflictQueryHandler->Close();
			delete lockConflictQueryHandler;
			lockConflictQueryHandler = NULL;
		}
		FreeMemory();
    }  // try ...

    catch ( ... ) {

      throw FdoCommandException::Create(
                    LockUtility::GetExceptionMessage(
                           LockUtility::CLOSING_LOCK_CONFLICT_READER_FAILURE));

    }  // catch ...

}  //  Close ()

long FdoRdbmsLockConflictReader::GetLockConflictCount ()

// +---------------------------------------------------------------------------
// | The function returns the number of lock conflicts stored with the lock
// | conflict reader.
// +---------------------------------------------------------------------------

{

    return ((lock_conflicts == NULL) ? 0 : lock_conflicts->GetCount());

}  //  GetLockConflictCount ()

void FdoRdbmsLockConflictReader::FreeMemory ()

// +---------------------------------------------------------------------------
// | The function frees memory.
// +---------------------------------------------------------------------------

{

    if (current_lock_conflict != NULL) {

        current_lock_conflict->Release();
        current_lock_conflict = NULL;

    }  //  if (current_lock_conflict != NULL) ...

    if (lock_conflicts != NULL) lock_conflicts->Release();
    lock_conflicts = NULL;

    if (conflict_report_class_name != NULL) {

        conflict_report_class_name->Release();
        conflict_report_class_name = NULL;

    }  //  if (conflict_report_class_name != NULL) ...

}  //  FreeMemory ()

void FdoRdbmsLockConflictReader::SetToZero ()

// +---------------------------------------------------------------------------
// | The function initializes the member variables.
// +---------------------------------------------------------------------------

{

    lock_conflicts        = NULL;
    current_lock_conflict = NULL;

	conflict_report_class_name = NULL;
	validReader = false;

}  //  SetToZero ()

void FdoRdbmsLockConflictReader::checkValidReader()
{
	if (validReader == false)
		throw FdoCommandException::Create(
                        LockUtility::GetExceptionMessage(
                        LockUtility::READER_POSITIONING_ERROR,
                        L"FdoILockConflictReader"));
}
