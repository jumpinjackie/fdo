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

#include "stdafx.h"
#include "SchemaManager.h"

FdoGrdSchemaManager::FdoGrdSchemaManager(GdbiConnection* connection) :
    mGdbiConnection(connection)
{
}

FdoGrdSchemaManager::~FdoGrdSchemaManager(void)
{
}

void FdoGrdSchemaManager::ApplySchema( 
    FdoFeatureSchemaP pFeatSchema, 
    FdoPhysicalSchemaMappingP pOverrides,
    bool bIgnoreStates
)
{
	GdbiConnection*	        pConn = GetGdbiConnection();
	bool			        tranBegun = false;
	static char*	        transName = "FdoApplySchema"; 

	try {
		// Start a transaction so we can rollback on error.
		pConn->GetCommands()->tran_begin( transName );
		tranBegun = true;

		// If the datastore has MetaSchema then lock one of the schema tables. 
        // This prevents 2 concurrent applySchema operations from messing each other up.

        FdoSmPhOwnerP owner = GetPhysicalSchema()->FindOwner();

        if ( owner && owner->GetHasMetaSchema() ) {
            GdbiStatement* lockStmt = pConn->Prepare( (const wchar_t*) GetSchemaLockStmt() );
            GdbiQueryResult* results = lockStmt->ExecuteQuery();

            results->End();
            delete results;
            lockStmt->Free();
            delete lockStmt;
        }

        // Apply the schema.
        FdoSchemaManager::ApplySchema( pFeatSchema, pOverrides, bIgnoreStates ); 

        // Commit to the database.
		pConn->GetCommands()->tran_end(transName);
		tranBegun = false;

		// Synchronize the element states in the Feature Schema
		pFeatSchema->AcceptChanges();
	}
	catch (...) {
		try {
			// Rollback the schema changes on failure.
            // This only rolls back the MetaSchema changes
            // Physical DDL changes are "rolled back" if a subsequence SynchPhysical()
            // is performed.
            if ( tranBegun ) 
    			pConn->GetCommands()->tran_rolbk();
			tranBegun = false;
		}
		catch ( ... ) {
		}

		throw;
	}
}

void FdoGrdSchemaManager::SynchPhysical( const wchar_t* schemaName, bool bRollbackOnly )
{
	GdbiConnection*	        pConn = GetGdbiConnection();
	bool			        tranBegun = false;
	static char*	        transName = "FdoSynchPhysical"; 

    // Proceed if synchronizing everything or the connection has rollback entries.
    if ( !bRollbackOnly || mPhysicalSchema->HasRollbackEntries() ) {
	    try {
		    // Start a transaction so we can rollback on error.
		    pConn->GetCommands()->tran_begin( transName );
		    tranBegun = true;

		    // If the datastore has MetaSchema then lock one of the schema tables. 
            // This prevents 2 concurrent applySchema operations from messing each other up.

            FdoSmPhOwnerP owner = GetPhysicalSchema()->FindOwner();

            if ( owner && owner->GetHasMetaSchema() ) {
                GdbiStatement* lockStmt = pConn->Prepare( (const wchar_t*) GetSchemaLockStmt() );
                GdbiQueryResult* results = lockStmt->ExecuteQuery();

                results->End();
                delete results;
                lockStmt->Free();
                delete lockStmt;
            }

            // Synchronize the schemas
            FdoSchemaManager::SynchPhysical( schemaName, bRollbackOnly ); 

            // Commit to the database.
		    pConn->GetCommands()->tran_end(transName);
		    tranBegun = false;
	    }
	    catch (...) {
		    try {
			    // Rollback the schema changes on failure.
                // This only rolls back the MetaSchema changes
                // Physical DDL changes are "rolled back" if a subsequence SynchPhysical()
                // is performed.
                if ( tranBegun ) 
    			    pConn->GetCommands()->tran_rolbk();
			    tranBegun = false;
		    }
		    catch ( ... ) {
		    }

		    throw;
	    }
    }
}

FdoStringP FdoGrdSchemaManager::GetSchemaLockStmt()
{
    return L"select schemaname from f_schemainfo for update";
}
