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

ShpUpdateCommand::ShpUpdateCommand (FdoIConnection *connection) :
    superclass (connection)
{
}

/** Do not implement the copy constructor. **/
//ShpUpdateCommand::ShpUpdateCommand (const ShpUpdateCommand &right) { }

ShpUpdateCommand::~ShpUpdateCommand (void)
{
	FdoPtr<ShpConnection> connection = (ShpConnection *)GetConnection();
	if ( connection->GetLastEditedFileSet() && (connection->GetConnectionState() == FdoConnectionState_Open))
		connection->GetLastEditedFileSet()->ReopenFileset( FdoCommonFile::IDF_OPEN_READ );
}

//
// FdoIUpdate interface
//

/// <summary>Gets the FdoPropertyValueCollection that specifies the names and values of the 
/// properties to be updated.</summary>
/// <returns>Returns the list of properties and their values.</returns> 
FdoPropertyValueCollection* ShpUpdateCommand::GetPropertyValues ()
{
    return (superclass::GetPropertyValues ());
}


/// <summary>Executes the update command and returns the number of modified 
/// instances.</summary>
/// <returns>Returns the number of features updated.</returns> 
FdoInt32 ShpUpdateCommand::Execute ()
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
        class_name = FdoPtr<FdoIdentifier> (GetFeatureClassName ())->GetName ();
        FdoPtr<ShpLpClassDefinition> lpClass = ShpSchemaUtilities::GetLpClassDefinition (mConnection, class_name);
        FdoPtr<FdoClassDefinition> logicalClass = lpClass->GetLogicalClass();
        fileset = lpClass->GetPhysicalFileSet ();
		ShpConnection *connection = (ShpConnection *)GetConnection();
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
            id = reader->GetInt32 (featid);
            id--; // featid are 0-based within the provider and 1-based outside the FDO API.

            eShapeTypes shape_type;
            Shape* shape;
            RowData* row;
            fileset->GetObjectAt (&row, shape_type, &shape, id);
            FdoPtr<FdoPropertyValueCollection> values = GetPropertyValues ();

            FdoCommonMiscUtil::HandleReadOnlyAndDefaultValues(logicalClass, values, false);

            fileset->PutData (FdoPtr<ShpConnection>((ShpConnection*)GetConnection()), FdoPtr<FdoIdentifier> (GetFeatureClassName ())->GetName(), 
                values, row, shape);
            ret++;
        }
    }
    catch (FdoException* ge)
    {
        throw ge;
    }
    catch (...)
    {
        throw FdoException::Create (NlsMsgGet (SHP_UPDATE_SETUP_FAILED, "Failed to initialize the update operation."));
    }

    return (ret);
}


/// <summary> Updating objects might result in lock conflicts if objects
/// to be updated are not exclusively locked for the user attempting to
/// update the object. If objects to be updated are not exclusively locked for the 
/// user attempting to update the object, a lock conflict report is generated.
/// The function GetLockConflicts returns a lock conflict reader providing
/// access to the list of lock conflicts that occurred during the execution
/// of the update operation.</summary>
/// <returns>Returns a lock conflict reader.</returns> 
FdoILockConflictReader* ShpUpdateCommand::GetLockConflicts ()
{
    throw FdoException::Create (NlsMsgGet (SHP_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"GetLockConflicts"));
}

