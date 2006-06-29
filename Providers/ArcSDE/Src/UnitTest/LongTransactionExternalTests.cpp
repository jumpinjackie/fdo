/*
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

#include "Pch.h"
#include "LongTransactionExternalTests.h"

//CPPUNIT_TEST_SUITE_REGISTRATION (LongTransactionExternalTests);
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (LongTransactionExternalTests, "LongTransactionExternalTests");

/*
  These tests require manually setting up a version with the parameters given below.
  Use ArcCatalog and set the connection parameters to log in as OZZIE/test.
  Create a version with SDE.DEFAULT as the parent.
  Edit the Australia map with that as the current version and save the changes but don't post.

  It is best to start with a clean (versionless) instance before doing this.
  Here is the code that cleans up the versions and states to be injected into
  ArcSDECreateLongTransactionCommand::Execute () for example:

    // if debugging, restore default to state 0, delete any ROOT version and delete all but state 0
    if (1)
    {
        SE_STREAM stream;
        CHAR* tables[1];
        const CHAR* columns[1];
        SE_SQL_CONSTRUCT  sql_construct;
        LONG result;
        LONG row;
        LONG states[1000];
        int index;
        SHORT row_ind;

        // create the empty version object
        result = SE_versioninfo_create (&version);
        handle_sde_err<FdoCommandException> (conn, result, ARCSDE_VERSION_INFO_ALLOC, "Cannot initialize SE_VERSIONINFO structure.");

        // revert default back to state 0
        result = SE_version_get_info (conn, "SDE.DEFAULT", version);
        result = SE_version_change_state (conn, version, 0);

        // clean up the states... may need to be run more than once because of dependancies between states
        tables[0] = "SDE.STATES";
        columns[0] = "STATE_ID";
        sql_construct.tables = tables;
        sql_construct.num_tables = ELEMENTS (tables);
        sql_construct.where = NULL;
        result = SE_stream_create (conn, &stream);
        handle_sde_err<FdoCommandException> (conn, result, ARCSDE_STREAM_ALLOC, "Cannot initialize SE_STREAM structure.");
        result = SE_stream_query (stream, ELEMENTS (columns), columns, &sql_construct);
        handle_sde_err<FdoCommandException> (conn, result, ARCSDE_STREAM_QUERY, "Stream query failed.");
        result = SE_stream_bind_output_column (stream, 1, &row, &row_ind);
        handle_sde_err<FdoCommandException>(stream, result, ARCSDE_BIND_COLUMN_FAILED, "Failed to bind column '%1$ls'.", L"STATE_ID");
        result = SE_stream_execute (stream);
        handle_sde_err<FdoCommandException>(stream, result, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");
        index = 0;
        while (SE_SUCCESS == (result = SE_stream_fetch (stream)))
        {
            if (0 != row)
                states[index] = row;
            index++;
            if (index >= 1000)
                break;
        }
        result = SE_stream_close (stream, true);
        handle_sde_err<FdoCommandException>(conn, result, ARCSDE_STREAM_CLOSE, "Stream close failed.");
        result = SE_stream_free (stream);
        handle_sde_err<FdoCommandException>(conn, result, ARCSDE_STREAM_FREE, "Stream free failed.");

        for (int i = index - 1; i >= 0; i--)
            result = SE_state_delete (conn, states[i]); // ignore error

        SE_versioninfo_free (version);
    }

*/

FdoPtr<FdoIConnection> LongTransactionExternalTests::mConnection;

LongTransactionExternalTests::LongTransactionExternalTests (void)
{
}

LongTransactionExternalTests::~LongTransactionExternalTests (void)
{
}

void LongTransactionExternalTests::setUp ()
{
    if (!CreateSchemaOnly() /* || !bSchemaCreated */ )
    {
    	mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringOzzie());
        mConnection->Open ();
    }
}

void LongTransactionExternalTests::tearDown ()
{
    if (mConnection)
        mConnection->Close ();
}

/* test getting the list of long transactions */
void LongTransactionExternalTests::get ()
{
    bool found;

    if (CreateSchemaOnly())  return;

    try
    {
        FdoPtr <FdoIGetLongTransactions> get = (FdoIGetLongTransactions*)mConnection->CreateCommand (FdoCommandType_GetLongTransactions);
        FdoPtr <FdoILongTransactionReader> reader = get->Execute ();
        found = false;
        while (reader->ReadNext ())
        {
            FdoString* name = reader->GetName ();
            if (0 == wcscmp (NAME(), name))
            {
                found = true;
                FdoString* description = reader->GetDescription ();
                CPPUNIT_ASSERT_MESSAGE ("description incorrect", 0 == wcscmp (DESCRIPTION(), description));

                // get parent long transaction:
                FdoPtr<FdoILongTransactionReader> readerParents = reader->GetParents ();
                CPPUNIT_ASSERT_MESSAGE ("parent long transaction not found", readerParents->ReadNext ());
                CPPUNIT_ASSERT_MESSAGE ("parent long transaction name incorrect", 0 == wcscmp (PARENT(), readerParents->GetName ()));
                CPPUNIT_ASSERT_MESSAGE ("too many parent long transactions found", !readerParents->ReadNext ());

                FdoString* owner = reader->GetOwner ();
                CPPUNIT_ASSERT_MESSAGE ("owner incorrect", 0 == wcscmp (OWNER(), owner));
            }
        }
        reader->Close ();
        if (!found)
            CPPUNIT_FAIL("long transaction not found");
        else
        {
            // try and activate it
            FdoPtr <FdoIActivateLongTransaction> activate = (FdoIActivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_ActivateLongTransaction);
            activate->SetName (NAME());
            activate->Execute ();

            // then deactivate it
            FdoPtr <FdoIDeactivateLongTransaction> deactivate = (FdoIDeactivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_DeactivateLongTransaction);
            deactivate->Execute ();
        }
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test commit operation under long transaction in the face of inserts (no conflicts). */
void LongTransactionExternalTests::commit ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        FdoPtr <FdoITransaction> transaction;

        transaction = mConnection->BeginTransaction ();

        // commit the version
        FdoPtr <FdoICommitLongTransaction> commit = (FdoICommitLongTransaction*)mConnection->CreateCommand (FdoCommandType_CommitLongTransaction);
        commit->SetName (NAME());
        FdoPtr <FdoILongTransactionConflictDirectiveEnumerator> enumerator = commit->Execute ();
        if (0 != enumerator->GetCount ())
        {
            while (enumerator->ReadNext ())
            {
                enumerator->SetResolution (FdoLongTransactionConflictResolution_Child);
            }
            enumerator = commit->Execute ();
        }
        transaction->Commit ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}
