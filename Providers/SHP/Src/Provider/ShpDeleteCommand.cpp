/*
 * 
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
 */

#include "stdafx.h"

#include "ShpLpClassDefinition.h"

ShpDeleteCommand::ShpDeleteCommand (FdoIConnection *connection) :
    FdoCommonFeatureCommand<FdoIDelete, ShpConnection> (connection)
{
}

/** Do not implement the copy constructor. **/
//ShpDeleteCommand::ShpDeleteCommand (const ShpDeleteCommand &right) { }

ShpDeleteCommand::~ShpDeleteCommand (void)
{
	FdoPtr<ShpConnection> connection = (ShpConnection *)GetConnection();
	if ( connection->GetLastEditedFileSet() && (connection->GetConnectionState() == FdoConnectionState_Open))
		connection->GetLastEditedFileSet()->ReopenFileset( FdoCommonFile::IDF_OPEN_READ );
}

/// <summary>Executes the delete command and returns the number of instances
/// deleted</summary>
/// <returns>Returns the number of instances deleted.</returns> 
FdoInt32 ShpDeleteCommand::Execute ()
{
    FdoString* class_name;
    ShpFileSet* fileset;
	ShpFileSet* lastFileset;
    FdoStringP featid;
    FdoPtr<ShpFeatureReader> reader;
    int id;
    FdoInt32 ret;
	
    ret = 0;

    try
    {
		FdoPtr<ShpConnection> connection = (ShpConnection *)GetConnection();

        class_name = FdoPtr<FdoIdentifier> (GetFeatureClassName ())->GetName ();
        fileset = FdoPtr<ShpLpClassDefinition>(ShpSchemaUtilities::GetLpClassDefinition (mConnection, class_name))->GetPhysicalFileSet ();
		
		lastFileset = connection->GetLastEditedFileSet();

		// Attempt to open the fileset in write mode. It fails in case other process is doing update.
		try
		{
			// Give up the write permissions if the fileset changed
			if ( lastFileset && ( lastFileset != fileset ) )
				lastFileset->ReopenFileset( FdoCommonFile::IDF_OPEN_READ );

			fileset->ReopenFileset( FdoCommonFile::IDF_OPEN_UPDATE );
		}
		catch (FdoException* ex)
		{
			// Revert to read-only state the ones changed above
			fileset->ReopenFileset( FdoCommonFile::IDF_OPEN_READ );
			throw ex;
		}
		connection->SetLastEditedFileSet(fileset);

		featid = ShpSchemaUtilities::GetIdentityPropertyName (mConnection, class_name);
        reader = new ShpFeatureReader (FdoPtr<ShpConnection> ((ShpConnection*)GetConnection ()), class_name, mFilter, NULL);
        while (reader->ReadNext ())
        {
            id = (int)reader->GetInt32 (featid);
            id--;  // featids are 0-based internally and 1-based externally (outside the FDO API)
            fileset->DeleteObjectAt (id);
            ret++;
        }
    }
    catch (FdoException* ge)
    {
        throw ge;
    }
    catch (...)
    {
        throw FdoException::Create (NlsMsgGet (SHP_DELETE_SETUP_FAILED, "Failed to initialize the delete operation."));
    }

    return (ret);
}

/// <summary> Deleting objects might result in lock conflicts if objects
/// to be deleted are not exclusively locked for the user attempting to
/// delete the object. A lock conflict report is generated.
/// The function GetLockConflicts returns a lock conflict reader that provides
/// access to the list of lock conflicts that occurred during the execution
/// of the delete operation.</summary>
/// <returns>Returns a lock conflict reader.</returns> 
FdoILockConflictReader* ShpDeleteCommand::GetLockConflicts ()
{
    throw FdoException::Create (NlsMsgGet (SHP_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"GetLockConflicts"));
}

