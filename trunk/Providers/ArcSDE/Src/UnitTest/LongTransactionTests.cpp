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
#include "LongTransactionTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION (LongTransactionTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (LongTransactionTests, "LongTransactionTests");


FdoPtr<FdoIConnection> LongTransactionTests::mConnection;

LongTransactionTests::LongTransactionTests (void) :
    theClassB(NULL),
    property1B(NULL),
    theClassA(NULL),
    property1A(NULL),
    theClassModified(NULL),
    property1Modified(NULL),
    theClass(NULL),
    property1(NULL),
    property0(NULL)
{
}

LongTransactionTests::~LongTransactionTests (void)
{
}

void LongTransactionTests::setUp ()
{
    static bool bSchemaCreated = false;

    // Connect:
    if (!CreateSchemaOnly() || !bSchemaCreated)
    {
    	mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcov());
        mConnection->Open ();

        // Setup test data:
        property0 = new UnitTestData(L"Id", L"Unique identifier", FdoDataType_Int32, 0,  0, 0, false, true, true,
                                     NULL, NULL, (wchar_t*)NULL);
        property1 = new UnitTestData(L"LtProperty", L"A string property.", FdoDataType_String, 512,  0, 0, false, false, false,
                                     L"'version 1 data'", L"'version 2 data'", (wchar_t*)NULL);

        theClass = new UnitTestClass(ArcSDETestConfig::QClassNameExample(), L"Example class definition.", 0, false, true, property0, property1, NULL);

        property1Modified = new UnitTestData(L"LtProperty", L"A string property.", FdoDataType_String, 512,  0, 0, false, false, false,
                                     L"'version 3 data'", L"'version 2 data'", (wchar_t*)NULL);

        theClassModified = new UnitTestClass(ArcSDETestConfig::QClassNameExample(), L"Example class definition.", 0, false, true, property0, property1Modified, NULL);

        property1A = new UnitTestData(L"LtProperty", L"A string property.", FdoDataType_String, 512,  0, 0, false, false, false,
                                     L"'version A data'", L"'version 2 data'", (wchar_t*)NULL);

        theClassA = new UnitTestClass(ArcSDETestConfig::QClassNameExample(), L"Example class definition.", 0, false, true, property0, property1A, NULL);

        property1B = new UnitTestData(L"LtProperty", L"A string property.", FdoDataType_String, 512,  0, 0, false, false, false,
                                     L"'version B data'", L"'version 2 data'", (wchar_t*)NULL);

        theClassB = new UnitTestClass(ArcSDETestConfig::QClassNameExample(), L"Example class definition.", 0, false, true, property0, property1B, NULL);
    }

    // Create test classes, if requested:
    if (CreateSchemaOnly() && !bSchemaCreated)
    {
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaExample(), theClass->mName);

        // establish a class to play with
        ArcSDETests::CreateSchema (mConnection, theClass);

        bSchemaCreated = true;
    }
}

void LongTransactionTests::tearDown ()
{
    if (mConnection)
        mConnection->Close ();

    // Cleanup test data:
    delete theClassB;  theClassB = NULL;
    delete property1B;  property1B = NULL;
    delete theClassA;  theClassA = NULL;
    delete property1A;  property1A = NULL;
    delete theClassModified;  theClassModified = NULL;
    delete property1Modified;  property1Modified = NULL;
    delete theClass;  theClass = NULL;
    delete property1;  property1 = NULL;
    delete property0;  property0 = NULL;
}

void LongTransactionTests::CleanUp ()
{
    try
    {
        FdoPtr <FdoIRollbackLongTransaction> rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (NAME());
        rollback->Execute ();
    }
    catch (FdoException* ge)
    {
        ge->Release ();
    }
    catch (...) 
    {
    }

    try
    {
        FdoPtr <FdoIRollbackLongTransaction> rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (NAMEA());
        rollback->Execute ();
    }
    catch (FdoException* ge)
    {
        ge->Release ();
    }
    catch (...) 
    {
    }

    try
    {
        FdoPtr <FdoIRollbackLongTransaction> rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (NAMEB());
        rollback->Execute ();
    }
    catch (FdoException* ge)
    {
        ge->Release ();
    }
    catch (...) 
    {
    }
}

void LongTransactionTests::LongTransactionGone (const wchar_t* name)
{
    FdoPtr <FdoIGetLongTransactions> get = (FdoIGetLongTransactions*)mConnection->CreateCommand (FdoCommandType_GetLongTransactions);
    FdoPtr <FdoILongTransactionReader> reader = get->Execute ();
    FdoString* active_name = NULL;
    while (reader->ReadNext ())
        if (0 == wcscmp (name, reader->GetName ()))
            CPPUNIT_FAIL ("long transaction still present");
    reader->Close ();
}


void LongTransactionTests::MakeTwoObjectsInTwoVersions (int* item1, int* item2)
{
    FdoPtr <FdoITransaction> transaction;
    FdoPtr <FdoICreateLongTransaction> create;
    FdoPtr <FdoIActivateLongTransaction> activate;
    std::vector<int*> map;

    try
    {
        // insert an object
        transaction = mConnection->BeginTransaction ();
        item1[1] = ArcSDETests::Insert (mConnection, theClass, item1[0]);
        transaction->Commit ();

        // make a version
        create = (FdoICreateLongTransaction*)mConnection->CreateCommand (FdoCommandType_CreateLongTransaction);
        create->SetName (NAME());
        create->SetDescription (DESCRIPTION());
        create->Execute ();

        // activate it
        activate = (FdoIActivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_ActivateLongTransaction);
        activate->SetName (NAME());
        activate->Execute ();

        // insert a version 2 object
        transaction = mConnection->BeginTransaction ();
        item2[1] = ArcSDETests::Insert (mConnection, theClass, item2[0]);
        transaction->Commit ();

        // check both are visible
        map.push_back (item1);
        map.push_back (item2);
        ArcSDETests::Expect (mConnection, theClass, &map);
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

void LongTransactionTests::MakeUpdateConflict (int* item1)
{
    FdoPtr <FdoITransaction> transaction;
    FdoPtr <FdoICreateLongTransaction> create;
    FdoPtr <FdoIActivateLongTransaction> activate;
    FdoPtr <FdoIDeactivateLongTransaction> deactivate;
    FdoPtr <FdoICommitLongTransaction> commit;
    FdoPtr <FdoILongTransactionConflictDirectiveEnumerator> enumerator;
    std::vector<int*> map;

    try
    {
        // insert an object
        transaction = mConnection->BeginTransaction ();
        item1[1] = ArcSDETests::Insert (mConnection, theClass, item1[0]);
        transaction->Commit ();

        // make a version
        create = (FdoICreateLongTransaction*)mConnection->CreateCommand (FdoCommandType_CreateLongTransaction);
        create->SetName (NAMEA());
        create->SetDescription (DESCRIPTIONA());
        create->Execute ();

        // activate it
        activate = (FdoIActivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_ActivateLongTransaction);
        activate->SetName (NAMEA());
        activate->Execute ();

        // update the object
        transaction = mConnection->BeginTransaction ();
        map.push_back (item1);
        ArcSDETests::Update (mConnection, theClassA, &map);
        transaction->Commit ();

        // deactivate the active version
        deactivate = (FdoIDeactivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_DeactivateLongTransaction);
        deactivate->Execute ();

        // check original version is visible
        ArcSDETests::Expect (mConnection, theClass, &map);

        // make a second version
        create->SetName (NAMEB());
        create->SetDescription (DESCRIPTIONB());
        create->Execute ();

        // activate it
        activate->SetName (NAMEB());
        activate->Execute ();

        // alter the same object to version B
        transaction = mConnection->BeginTransaction ();
        ArcSDETests::Update (mConnection, theClassB, &map);
        transaction->Commit ();

        // commit the version
        transaction = mConnection->BeginTransaction ();
        commit = (FdoICommitLongTransaction*)mConnection->CreateCommand (FdoCommandType_CommitLongTransaction);
        commit->SetName (NAMEB());
        enumerator = commit->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("conflict detected", 0 == enumerator->GetCount ());
        transaction->Commit ();

        // check that the long transaction is no longer around
        LongTransactionGone (DECORATED_NAMEB());

        // check the new version is visible
        ArcSDETests::Expect (mConnection, theClassB, &map);

        // reactivate the A version and check it
        activate->SetName (NAMEA());
        activate->Execute ();

        // check the new version is visible
        ArcSDETests::Expect (mConnection, theClassA, &map);

        // deactivate the active version
        deactivate->Execute ();

        // check the new version is still visible
        ArcSDETests::Expect (mConnection, theClassB, &map);
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* test creating a long transaction and rolling it back */
void LongTransactionTests::create_then_rollback ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        FdoPtr <FdoICreateLongTransaction> create;
        FdoPtr <FdoIRollbackLongTransaction> rollback;

        CleanUp ();

        create = (FdoICreateLongTransaction*)mConnection->CreateCommand (FdoCommandType_CreateLongTransaction);
        create->SetName (NAME());
        create->SetDescription (DESCRIPTION());
        create->Execute ();

        rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (NAME());
        rollback->Execute ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* test weird names */
void LongTransactionTests::name_test ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        FdoPtr <FdoICreateLongTransaction> create = (FdoICreateLongTransaction*)mConnection->CreateCommand (FdoCommandType_CreateLongTransaction);
        create->SetDescription (NULL);
        FdoPtr <FdoIRollbackLongTransaction> rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);

        try
        {
            create->SetName (L"P.X");
            create->Execute ();

            CPPUNIT_FAIL ("version names containing dots are not allowed");
            rollback->SetName (L"P.X");
            rollback->Execute ();
        }
        catch (FdoException* ge)
        {
            // expected
            ge->Release ();
        }

        try
        {
            create->SetName (L" ");
            create->Execute ();

            CPPUNIT_FAIL ("blank version names are not allowed");
            rollback->SetName (L"P.X");
            rollback->Execute ();
        }
        catch (FdoException* ge)
        {
            // expected
            ge->Release ();
        }
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* test getting the list of long transactions */
void LongTransactionTests::get ()
{
    bool found;
    struct tm systime;


    if (CreateSchemaOnly())  return;

    try
    {
        CleanUp ();

        FdoPtr <FdoICreateLongTransaction> create = (FdoICreateLongTransaction*)mConnection->CreateCommand (FdoCommandType_CreateLongTransaction);
        create->SetName (NAME());
        create->SetDescription (DESCRIPTION());
        create->Execute ();

        FdoPtr <FdoIGetLongTransactions> get = (FdoIGetLongTransactions*)mConnection->CreateCommand (FdoCommandType_GetLongTransactions);
        FdoPtr <FdoILongTransactionReader> reader = get->Execute ();
        found = false;
        while (reader->ReadNext ())
        {
            FdoString* name = reader->GetName ();
            if (0 == wcscmp (DECORATED_NAME(), name))
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
                FdoDateTime fdoDatetime = reader->GetCreationDate ();
                FdoCommonOSUtil::getsystime (&systime);
                CPPUNIT_ASSERT_MESSAGE ("year wrong", fdoDatetime.year == systime.tm_year + 1900);
                CPPUNIT_ASSERT_MESSAGE ("month wrong", fdoDatetime.month == systime.tm_mon + 1);
                CPPUNIT_ASSERT_MESSAGE ("day wrong", fdoDatetime.day == systime.tm_mday);
                //THE FOLLOWING TEST FAILS FREQUENTLY, SINCE THE CLIENT'S TIME AND SERVER'S TIME ARE USUALLY NOT IN SYNC:
		        //CPPUNIT_ASSERT_MESSAGE ("hour wrong", fdoDatetime.hour == systime.tm_hour);
                CPPUNIT_ASSERT_MESSAGE ("active", !reader->IsActive ());
                CPPUNIT_ASSERT_MESSAGE ("frozen", !reader->IsFrozen ());
            }
        }
        reader->Close ();
        if (!found)
            CPPUNIT_FAIL("long transaction not found");

        FdoPtr <FdoIRollbackLongTransaction> rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (NAME());
        rollback->Execute ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* test getting the root version */
void LongTransactionTests::get_root ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        CleanUp ();

        FdoPtr <FdoICreateLongTransaction> create = (FdoICreateLongTransaction*)mConnection->CreateCommand (FdoCommandType_CreateLongTransaction);
        create->SetName (NAME());
        create->SetDescription (DESCRIPTION());
        create->Execute ();

        FdoPtr <FdoIGetLongTransactions> get = (FdoIGetLongTransactions*)mConnection->CreateCommand (FdoCommandType_GetLongTransactions);
        get->SetName (FDO_ROOTLONGTRANSACTION);
        FdoPtr <FdoILongTransactionReader> reader = get->Execute ();
        reader->ReadNext ();
        FdoPtr <FdoILongTransactionReader> parents = reader->GetParents ();
        CPPUNIT_ASSERT_MESSAGE ("root has parents", !parents->ReadNext ());
        parents->Close ();
        reader->Close ();

        FdoPtr <FdoIRollbackLongTransaction> rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (NAME());
        rollback->Execute ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* test getting the active version */
void LongTransactionTests::get_active ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        CleanUp ();

        FdoPtr <FdoICreateLongTransaction> create = (FdoICreateLongTransaction*)mConnection->CreateCommand (FdoCommandType_CreateLongTransaction);
        create->SetName (NAME());
        create->SetDescription (DESCRIPTION());
        create->Execute ();

        FdoPtr <FdoIActivateLongTransaction> activate = (FdoIActivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_ActivateLongTransaction);
        activate->SetName (NAME());
        activate->Execute ();
        FdoPtr <FdoIGetLongTransactions> get = (FdoIGetLongTransactions*)mConnection->CreateCommand (FdoCommandType_GetLongTransactions);
        get->SetName (FDO_ACTIVELONGTRANSACTION);
        FdoPtr <FdoILongTransactionReader> reader = get->Execute ();
        reader->ReadNext ();
        FdoString* name = reader->GetName ();
        if (0 != wcscmp (DECORATED_NAME(), name))
            CPPUNIT_FAIL ("name wrong");
        CPPUNIT_ASSERT_MESSAGE ("not active", reader->IsActive ());
        CPPUNIT_ASSERT_MESSAGE ("not just the active version", !reader->ReadNext ());
        reader->Close ();

        FdoPtr <FdoIRollbackLongTransaction> rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (NAME());
        rollback->Execute ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* test getting a version's children */
void LongTransactionTests::get_children ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        CleanUp ();

        FdoPtr <FdoICreateLongTransaction> create = (FdoICreateLongTransaction*)mConnection->CreateCommand (FdoCommandType_CreateLongTransaction);
        create->SetName (NAME());
        create->SetDescription (DESCRIPTION());
        create->Execute ();

        FdoPtr <FdoIGetLongTransactions> get = (FdoIGetLongTransactions*)mConnection->CreateCommand (FdoCommandType_GetLongTransactions);
        get->SetName (FDO_ROOTLONGTRANSACTION);
        FdoPtr <FdoILongTransactionReader> reader = get->Execute ();
        reader->ReadNext ();
        FdoPtr <FdoILongTransactionReader> children = reader->GetChildren ();
        bool found = false;
        while (children->ReadNext ())
        {
            FdoString* name = children->GetName ();
            if (0 == wcscmp (DECORATED_NAME(), name))
                found = true;
        }
        children->Close ();
        reader->Close ();
        CPPUNIT_ASSERT_MESSAGE ("child not found", found);

        FdoPtr <FdoIRollbackLongTransaction> rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (NAME());
        rollback->Execute ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* test getting a specific version */
void LongTransactionTests::get_specific ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        CleanUp ();

        FdoPtr <FdoICreateLongTransaction> create = (FdoICreateLongTransaction*)mConnection->CreateCommand (FdoCommandType_CreateLongTransaction);
        create->SetName (NAME());
        create->SetDescription (DESCRIPTION());
        create->Execute ();

        FdoPtr <FdoIGetLongTransactions> get = (FdoIGetLongTransactions*)mConnection->CreateCommand (FdoCommandType_GetLongTransactions);
        get->SetName (NAME());
        FdoPtr <FdoILongTransactionReader> reader = get->Execute ();
        reader->ReadNext ();
        FdoString* name = reader->GetName ();
        if (0 != wcscmp (DECORATED_NAME(), name))
            CPPUNIT_FAIL ("wrong name");
        CPPUNIT_ASSERT_MESSAGE ("not just the specific version", !reader->ReadNext ());
        reader->Close ();

        FdoPtr <FdoIRollbackLongTransaction> rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (NAME());
        rollback->Execute ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* test getting a specific version using the qualified name*/
void LongTransactionTests::get_specific_qualified ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        CleanUp ();

        FdoPtr <FdoICreateLongTransaction> create = (FdoICreateLongTransaction*)mConnection->CreateCommand (FdoCommandType_CreateLongTransaction);
        create->SetName (NAME());
        create->SetDescription (DESCRIPTION());
        create->Execute ();

        FdoPtr <FdoIGetLongTransactions> get = (FdoIGetLongTransactions*)mConnection->CreateCommand (FdoCommandType_GetLongTransactions);
        get->SetName (DECORATED_NAME());
        FdoPtr <FdoILongTransactionReader> reader = get->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("reader empty", reader->ReadNext ());
        FdoString* name = reader->GetName ();
        if (0 != wcscmp (DECORATED_NAME(), name))
            CPPUNIT_FAIL ("wrong name");
        CPPUNIT_ASSERT_MESSAGE ("not just the specific version", !reader->ReadNext ());
        reader->Close ();

        FdoPtr <FdoIRollbackLongTransaction> rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (NAME());
        rollback->Execute ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* test activating the root long transaction */
void LongTransactionTests::activate_root ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        CleanUp ();

        FdoPtr <FdoICreateLongTransaction> create = (FdoICreateLongTransaction*)mConnection->CreateCommand (FdoCommandType_CreateLongTransaction);
        create->SetName (NAME());
        create->SetDescription (DESCRIPTION());
        create->Execute ();

        // activate it
        FdoPtr <FdoIActivateLongTransaction> activate = (FdoIActivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_ActivateLongTransaction);
        activate->SetName (NAME());
        activate->Execute ();
        FdoPtr <FdoIGetLongTransactions> get = (FdoIGetLongTransactions*)mConnection->CreateCommand (FdoCommandType_GetLongTransactions);
        FdoPtr <FdoILongTransactionReader> reader = get->Execute ();
        FdoString* active_name = NULL;
        while (reader->ReadNext ())
        {
            if (reader->IsActive ())
            {
                if (NULL != active_name)
                    CPPUNIT_FAIL ("too many active");
                active_name = reader->GetName ();
                if (0 != wcscmp (DECORATED_NAME(), active_name))
                    CPPUNIT_FAIL ("name wrong");
            }
        }
        if (NULL == active_name)
            CPPUNIT_FAIL ("none active");
        reader->Close ();

        // activate the active long transaction
        activate->SetName (FDO_ACTIVELONGTRANSACTION);
        activate->Execute ();

        // check
        reader = get->Execute ();
        while (reader->ReadNext ())
            if (0 == wcscmp (DECORATED_NAME(), reader->GetName ()) && !reader->IsActive ())
                CPPUNIT_FAIL ("not active any more");
        reader->Close ();

        // activate the root long transaction
        activate->SetName (FDO_ROOTLONGTRANSACTION);
        activate->Execute ();

        // check
        reader = get->Execute ();
        while (reader->ReadNext ())
            if (0 == wcscmp (DECORATED_NAME(), reader->GetName ()) && reader->IsActive ())
                CPPUNIT_FAIL ("still active");
        reader->Close ();

        FdoPtr <FdoIRollbackLongTransaction> rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (NAME());
        rollback->Execute ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* test activating all long transactions returned by the GetLongTransactions */
void LongTransactionTests::activate_all ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        CleanUp ();

        FdoPtr <FdoICreateLongTransaction> create = (FdoICreateLongTransaction*)mConnection->CreateCommand (FdoCommandType_CreateLongTransaction);
        create->SetName (NAME());
        create->SetDescription (DESCRIPTION());
        create->Execute ();

        FdoPtr <FdoIGetLongTransactions> get = (FdoIGetLongTransactions*)mConnection->CreateCommand (FdoCommandType_GetLongTransactions);
        FdoPtr <FdoILongTransactionReader> reader = get->Execute ();
        while (reader->ReadNext ())
        {
            FdoString* name = reader->GetName ();
                
            // activate it
            FdoPtr <FdoIActivateLongTransaction> activate = (FdoIActivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_ActivateLongTransaction);
            activate->SetName (name);
            activate->Execute ();

            // check
            FdoPtr <FdoIGetLongTransactions> get2 = (FdoIGetLongTransactions*)mConnection->CreateCommand (FdoCommandType_GetLongTransactions);
            FdoPtr <FdoILongTransactionReader> reader2 = get2->Execute ();
            while (reader2->ReadNext ())
                if (0 == wcscmp (name, reader2->GetName ()) && !reader2->IsActive ())
                    CPPUNIT_FAIL ("not active");
            reader2->Close ();
        }
        FdoPtr <FdoIRollbackLongTransaction> rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (NAME());
        rollback->Execute ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* test deactivating a long transaction */
void LongTransactionTests::deactivate ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        CleanUp ();

        FdoPtr <FdoICreateLongTransaction> create = (FdoICreateLongTransaction*)mConnection->CreateCommand (FdoCommandType_CreateLongTransaction);
        create->SetName (NAME());
        create->SetDescription (DESCRIPTION());
        create->Execute ();

        // activate it
        FdoPtr <FdoIActivateLongTransaction> activate = (FdoIActivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_ActivateLongTransaction);
        activate->SetName (NAME());
        activate->Execute ();
        FdoPtr <FdoIGetLongTransactions> get = (FdoIGetLongTransactions*)mConnection->CreateCommand (FdoCommandType_GetLongTransactions);
        FdoPtr <FdoILongTransactionReader> reader = get->Execute ();
        FdoString* active_name = NULL;
        while (reader->ReadNext ())
        {
            if (reader->IsActive ())
            {
                if (NULL != active_name)
                    CPPUNIT_FAIL ("too many active");
                active_name = reader->GetName ();
                if (0 != wcscmp (DECORATED_NAME(), active_name))
                    CPPUNIT_FAIL ("name wrong");
            }
        }
        if (NULL == active_name)
            CPPUNIT_FAIL ("none active");
        reader->Close ();

        // deactivate it
        FdoPtr <FdoIDeactivateLongTransaction> deactivate = (FdoIDeactivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_DeactivateLongTransaction);
        deactivate->Execute ();

        // check
        reader = get->Execute ();
        while (reader->ReadNext ())
            if (0 == wcscmp (DECORATED_NAME(), reader->GetName ()) && reader->IsActive ())
                CPPUNIT_FAIL ("still active");
        reader->Close ();

        FdoPtr <FdoIRollbackLongTransaction> rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (NAME());
        rollback->Execute ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test basic insert operation under long transaction. */
void LongTransactionTests::insert ()
{
    FdoPtr <FdoITransaction> transaction;
    FdoPtr <FdoIDeactivateLongTransaction> deactivate;
    FdoPtr <FdoIRollbackLongTransaction> rollback;
    int item1[2];
    int item2[2];

    if (CreateSchemaOnly())  return;

    try
    {
        // clean up previous tests
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaExample(), theClass->mName, true);
        CleanUp ();

        // set up
        item1[0] = 0;
        item2[0] = 1;
        MakeTwoObjectsInTwoVersions (item1, item2);

        // deactivate the active version
        deactivate = (FdoIDeactivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_DeactivateLongTransaction);
        deactivate->Execute ();

        // check only version 1 is visible
        std::vector<int*> map2;
        map2.push_back (item1);
        ArcSDETests::Expect (mConnection, theClass, &map2);

        // clean up
        rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (NAME());
        rollback->Execute ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test commit operation under long transaction in the face of inserts (no conflicts). */
void LongTransactionTests::commit_insert ()
{
    FdoPtr <FdoITransaction> transaction;
    FdoPtr <FdoICreateLongTransaction> create;
    FdoPtr <FdoIActivateLongTransaction> activate;
    FdoPtr <FdoIDeactivateLongTransaction> deactivate;
    FdoPtr <FdoICommitLongTransaction> commit;
    FdoPtr <FdoILongTransactionConflictDirectiveEnumerator> enumerator;
    int item1[2];
    int item2[2];
    std::vector<int*> map;

    if (CreateSchemaOnly())  return;

    try
    {
        // clean up previous tests
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaExample(), theClass->mName, true);
        CleanUp ();

        // set up
        item1[0] = 0;
        item2[0] = 1;
        MakeTwoObjectsInTwoVersions (item1, item2);

        // deactivate the active version
        deactivate = (FdoIDeactivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_DeactivateLongTransaction);
        deactivate->Execute ();

        // check only version 1 is visible
        map.push_back (item1);
        ArcSDETests::Expect (mConnection, theClass, &map);

        // commit the version
        transaction = mConnection->BeginTransaction ();
        commit = (FdoICommitLongTransaction*)mConnection->CreateCommand (FdoCommandType_CommitLongTransaction);
        commit->SetName (NAME());
        enumerator = commit->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("unexpected conflict", !enumerator->ReadNext ());
        transaction->Commit ();

        // check both are visible
        map.push_back (item2);
        ArcSDETests::Expect (mConnection, theClass, &map);

        // check that the long transaction is no longer around
        LongTransactionGone (DECORATED_NAME());
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test basic insert operation under long transaction. */
void LongTransactionTests::del ()
{
    FdoPtr <FdoITransaction> transaction;
    FdoPtr <FdoICreateLongTransaction> create;
    FdoPtr <FdoIActivateLongTransaction> activate;
    FdoPtr <FdoIDeactivateLongTransaction> deactivate;
    FdoPtr <FdoIRollbackLongTransaction> rollback;
    int item1[2];
    int item2[2];
    std::vector<int*> map;

    if (CreateSchemaOnly())  return;

    try
    {
        // clean up previous tests
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaExample(), theClass->mName, true);
        CleanUp ();

        // set up
        item1[0] = 0;
        item2[0] = 1;
        MakeTwoObjectsInTwoVersions (item1, item2);

        // delete the first object
        transaction = mConnection->BeginTransaction ();
        ArcSDETests::Delete (mConnection, theClass, item1[1]);
        transaction->Commit ();

        // check only the second is visible
        map.push_back (item2);
        ArcSDETests::Expect (mConnection, theClass, &map);

        // deactivate the active version
        deactivate = (FdoIDeactivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_DeactivateLongTransaction);
        deactivate->Execute ();

        // check only the first is visible
        map.clear ();
        map.push_back (item1);
        ArcSDETests::Expect (mConnection, theClass, &map);

        // clean up
        rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (NAME());
        rollback->Execute ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test commit operation under long transaction in the face of delete (no conflicts). */
void LongTransactionTests::commit_delete ()
{
    FdoPtr <FdoITransaction> transaction;
    FdoPtr <FdoICreateLongTransaction> create;
    FdoPtr <FdoIActivateLongTransaction> activate;
    FdoPtr <FdoIDeactivateLongTransaction> deactivate;
    FdoPtr <FdoICommitLongTransaction> commit;
    FdoPtr <FdoILongTransactionConflictDirectiveEnumerator> enumerator;
    int item1[2];
    int item2[2];
    std::vector<int*> map;

    if (CreateSchemaOnly())  return;

    try
    {
        // clean up previous tests
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaExample(), theClass->mName, true);
        CleanUp ();

        // set up
        item1[0] = 0;
        item2[0] = 1;
        MakeTwoObjectsInTwoVersions (item1, item2);

        // delete the first object
        transaction = mConnection->BeginTransaction ();
        ArcSDETests::Delete (mConnection, theClass, item1[1]);
        transaction->Commit ();

        // check only the second is visible
        map.push_back (item2);
        ArcSDETests::Expect (mConnection, theClass, &map);

        // deactivate the active version
        deactivate = (FdoIDeactivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_DeactivateLongTransaction);
        deactivate->Execute ();

        // check only the first is visible
        map.clear ();
        map.push_back (item1);
        ArcSDETests::Expect (mConnection, theClass, &map);

        // commit the version
        transaction = mConnection->BeginTransaction ();
        commit = (FdoICommitLongTransaction*)mConnection->CreateCommand (FdoCommandType_CommitLongTransaction);
        commit->SetName (NAME());
        enumerator = commit->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("unexpected conflict", !enumerator->ReadNext ());
        transaction->Commit ();

        // check only the second is visible
        map.clear ();
        map.push_back (item2);
        ArcSDETests::Expect (mConnection, theClass, &map);

        // check that the long transaction is no longer around
        LongTransactionGone (DECORATED_NAME());
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test basic update operation under long transaction. */
void LongTransactionTests::update ()
{
    FdoPtr <FdoITransaction> transaction;
    FdoPtr <FdoICreateLongTransaction> create;
    FdoPtr <FdoIActivateLongTransaction> activate;
    FdoPtr<FdoIUpdate> update;
    FdoPtr <FdoIDeactivateLongTransaction> deactivate;
    FdoPtr <FdoIRollbackLongTransaction> rollback;
    int item1[2];
    int item2[2];
    std::vector<int*> map;

    if (CreateSchemaOnly())  return;

    try
    {
        // clean up previous tests
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaExample(), theClass->mName, true);
        CleanUp ();

        // set up
        item1[0] = 0;
        item2[0] = 1;
        MakeTwoObjectsInTwoVersions (item1, item2);

        // update the first object
        transaction = mConnection->BeginTransaction ();
        map.push_back (item1);
        ArcSDETests::Update (mConnection, theClassModified, &map);
        transaction->Commit ();

        // deactivate the active version
        deactivate = (FdoIDeactivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_DeactivateLongTransaction);
        deactivate->Execute ();

        // check only the first is visible with the original values
        ArcSDETests::Expect (mConnection, theClass, &map);

        // clean up
        rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (NAME());
        rollback->Execute ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test commit operation under long transaction in the face of update (no conflicts). */
void LongTransactionTests::commit_update ()
{
    FdoPtr <FdoITransaction> transaction;
    FdoPtr <FdoICreateLongTransaction> create;
    FdoPtr <FdoIActivateLongTransaction> activate;
    FdoPtr<FdoIUpdate> update;
    FdoPtr <FdoIDeactivateLongTransaction> deactivate;
    FdoPtr <FdoICommitLongTransaction> commit;
    FdoPtr <FdoILongTransactionConflictDirectiveEnumerator> enumerator;
    int item1[2];
    int item2[2];
    std::vector<int*> map;

    if (CreateSchemaOnly())  return;

    try
    {
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaExample(), theClass->mName, true);
        CleanUp ();

        // set up
        item1[0] = 0;
        item2[0] = 1;
        MakeTwoObjectsInTwoVersions (item1, item2);

        // update the first object
        transaction = mConnection->BeginTransaction ();
        map.push_back (item1);
        ArcSDETests::Update (mConnection, theClassModified, &map);
        transaction->Commit ();

        // deactivate the active version
        deactivate = (FdoIDeactivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_DeactivateLongTransaction);
        deactivate->Execute ();

        // check only the first is visible with the original values
        ArcSDETests::Expect (mConnection, theClass, &map);

        // commit the version
        transaction = mConnection->BeginTransaction ();
        commit = (FdoICommitLongTransaction*)mConnection->CreateCommand (FdoCommandType_CommitLongTransaction);
        commit->SetName (NAME());
        enumerator = commit->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("unexpected conflict", !enumerator->ReadNext ());
        transaction->Commit ();
        // ToDo: check when the long transaction is active

        // check that the modifications are still in force
        map.push_back (item2);
        ArcSDETests::Expect (mConnection, theClassModified, &map);

        // check that the long transaction is no longer around
        LongTransactionGone (DECORATED_NAME());
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

void LongTransactionTests::CommitUpdateConflict (bool child) 
{
    FdoPtr <FdoITransaction> transaction;
    FdoPtr <FdoICommitLongTransaction> commit;
    FdoPtr <FdoILongTransactionConflictDirectiveEnumerator> enumerator;
    int item1[2];
    std::vector<int*> map;

    try
    {
        // clean up previous tests
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaExample(), theClass->mName, true);
        CleanUp ();

        // set up
        item1[0] = 0;
        MakeUpdateConflict (item1);

        transaction = mConnection->BeginTransaction ();
        commit = (FdoICommitLongTransaction*)mConnection->CreateCommand (FdoCommandType_CommitLongTransaction);
        commit->SetName (NAMEA());
        enumerator = commit->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("no conflict detected", 0 != enumerator->GetCount ());
        while (enumerator->ReadNext ())
            if (child)
                enumerator->SetResolution (FdoLongTransactionConflictResolution_Child); // A wins
            else
                enumerator->SetResolution (FdoLongTransactionConflictResolution_Parent); // B wins
        enumerator = commit->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("conflict still detected", 0 == enumerator->GetCount ());
        transaction->Commit ();

        // check it
        map.push_back (item1);
        if (child)
            ArcSDETests::Expect (mConnection, theClassA, &map);
        else
            ArcSDETests::Expect (mConnection, theClassB, &map);
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test commit operation under long transaction in the face of update with a conflict. */
void LongTransactionTests::commit_update_conflict_child_wins ()
{
    if (CreateSchemaOnly())  return;

    // handle the conflict with the A version superior
    CommitUpdateConflict (true);
}

/* Test commit operation under long transaction in the face of update with a conflict. */
void LongTransactionTests::commit_update_conflict_parent_wins ()
{
    if (CreateSchemaOnly())  return;

    // handle the conflict with the B version superior
    CommitUpdateConflict (false);
}

/* Test committing a non-leaf version */
void LongTransactionTests::commit_leaf ()
{
    FdoPtr <FdoITransaction> transaction;
    FdoPtr <FdoICreateLongTransaction> create;
    FdoPtr <FdoIActivateLongTransaction> activate;
    FdoPtr <FdoIDeactivateLongTransaction> deactivate;
    FdoPtr <FdoICommitLongTransaction> commit;
    FdoPtr <FdoILongTransactionConflictDirectiveEnumerator> enumerator;
    FdoPtr <FdoIRollbackLongTransaction> rollback;
    bool failed;

    if (CreateSchemaOnly())  return;

    try
    {
        // clean up previous tests
        rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        try
        {
            rollback->SetName (NAMEB());
            rollback->Execute ();
        }
        catch (FdoException* ge) 
        {
            ge->Release ();
        }
        try
        {
            rollback->SetName (NAMEA());
            rollback->Execute ();
        }
        catch (FdoException* ge) 
        {
            ge->Release ();
        }
        try
        {
            rollback->SetName (NAME());
            rollback->Execute ();
        }
        catch (FdoException* ge) 
        {
            ge->Release ();
        }

        create = (FdoICreateLongTransaction*)mConnection->CreateCommand (FdoCommandType_CreateLongTransaction);
        create->SetName (NAME());
        create->SetDescription (DESCRIPTION());
        create->Execute ();

        // make a version
        create = (FdoICreateLongTransaction*)mConnection->CreateCommand (FdoCommandType_CreateLongTransaction);
        create->SetName (NAMEA());
        create->SetDescription (DESCRIPTIONA());
        create->Execute ();

        // activate it
        activate = (FdoIActivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_ActivateLongTransaction);
        activate->SetName (NAMEA());
        activate->Execute ();

        // make a second version based on the first version
        create = (FdoICreateLongTransaction*)mConnection->CreateCommand (FdoCommandType_CreateLongTransaction);
        create->SetName (NAMEB());
        create->SetDescription (DESCRIPTIONB());
        create->Execute ();

        // deactivate the active version
        deactivate = (FdoIDeactivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_DeactivateLongTransaction);
        deactivate->Execute ();

        // try to commit the first version
        failed = true;
        try
        {
            transaction = mConnection->BeginTransaction ();
            commit = (FdoICommitLongTransaction*)mConnection->CreateCommand (FdoCommandType_CommitLongTransaction);
            commit->SetName (NAMEA());
            enumerator = commit->Execute ();
            CPPUNIT_ASSERT_MESSAGE ("conflict detected", 0 == enumerator->GetCount ());
            transaction->Commit ();
            failed = false;
        }
        catch (FdoException* ge) 
        {
            // expected outcome
            ge->Release ();
        }

        // clean up
        try
        {
            rollback->SetName (NAMEB());
            rollback->Execute ();
        }
        catch (FdoException* ge) 
        {
            ge->Release ();
        }
        try
        {
            rollback->SetName (NAMEA());
            rollback->Execute ();
        }
        catch (FdoException* ge) 
        {
            ge->Release ();
        }
        try
        {
            rollback->SetName (NAME());
            rollback->Execute ();
        }
        catch (FdoException* ge) 
        {
            ge->Release ();
        }

        CPPUNIT_ASSERT_MESSAGE ("non-leaf version committed", failed);
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test updating a version that has children */
void LongTransactionTests::update_parent ()
{
    FdoPtr <FdoITransaction> transaction;
    FdoPtr <FdoICreateLongTransaction> create;
    FdoPtr <FdoIActivateLongTransaction> activate;
    FdoPtr <FdoIDeactivateLongTransaction> deactivate;
    FdoPtr <FdoICommitLongTransaction> commit;
    FdoPtr <FdoILongTransactionConflictDirectiveEnumerator> enumerator;
    FdoPtr <FdoIRollbackLongTransaction> rollback;
    int item1[2];
    int item2[2];

    if (CreateSchemaOnly())  return;

    try
    {
        // clean up previous tests
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaExample(), theClass->mName, true);
        rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        try
        {
            rollback->SetName (NAMEB());
            rollback->Execute ();
        }
        catch (FdoException* ge) 
        {
            ge->Release ();
        }
        try
        {
            rollback->SetName (NAMEA());
            rollback->Execute ();
        }
        catch (FdoException* ge) 
        {
            ge->Release ();
        }

        // make a version
        create = (FdoICreateLongTransaction*)mConnection->CreateCommand (FdoCommandType_CreateLongTransaction);
        create->SetName (NAMEA());
        create->SetDescription (DESCRIPTIONA());
        create->Execute ();

        // activate it
        activate = (FdoIActivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_ActivateLongTransaction);
        activate->SetName (NAMEA());
        activate->Execute ();

        // make a second version based on the first version
        create = (FdoICreateLongTransaction*)mConnection->CreateCommand (FdoCommandType_CreateLongTransaction);
        create->SetName (NAMEB());
        create->SetDescription (DESCRIPTIONB());
        create->Execute ();

        // activate it
        activate = (FdoIActivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_ActivateLongTransaction);
        activate->SetName (NAMEB());
        activate->Execute ();

        // insert an object in the child version
        item1[0] = 0;
        transaction = mConnection->BeginTransaction ();
        item1[1] = ArcSDETests::Insert (mConnection, theClass, item1[0]);
        transaction->Commit ();

        // activate the parent version again
        activate = (FdoIActivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_ActivateLongTransaction);
        activate->SetName (NAMEA());
        activate->Execute ();

        // insert an object in the parent version
        item2[0] = 1;
        transaction = mConnection->BeginTransaction ();
        item2[1] = ArcSDETests::Insert (mConnection, theClass, item2[0]);
        transaction->Commit ();

        // clean up
        try
        {
            rollback->SetName (NAMEB());
            rollback->Execute ();
        }
        catch (FdoException* ge) 
        {
            ge->Release ();
        }
        try
        {
            rollback->SetName (NAMEA());
            rollback->Execute ();
        }
        catch (FdoException* ge) 
        {
            ge->Release ();
        }
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}


void LongTransactionTests::rollback_arccatalog_version ()
{
    if (CreateSchemaOnly())  return;

    FdoPtr <FdoIRollbackLongTransaction> rollback;

    try
    {
        // Create an ArcCatalog-like version:
        FdoStringP wNewVersionName = CreateArcCatalogLikeVersion(SE_VERSION_ACCESS_PRIVATE);

        // Rollback the new version using our ArcSDE Provider:
        rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (wNewVersionName);
        rollback->Execute ();

        // Make sure the version no longer exists:
        FdoPtr<FdoIGetLongTransactions> getLTs = (FdoIGetLongTransactions*)mConnection->CreateCommand(FdoCommandType_GetLongTransactions);
        FdoPtr<FdoILongTransactionReader> ltReader = getLTs->Execute();
        bool bFound = false;
        while (ltReader->ReadNext())
            if (0==wcscmp(ltReader->GetName(),(FdoString*)wNewVersionName))
                bFound = true;
        ltReader->Close();
        CPPUNIT_ASSERT_MESSAGE("Version was found", !bFound);
    }
    catch (FdoException* ge)
    {
        fail (ge);
    }
}


void LongTransactionTests::activate_arccatalog_version ()
{
    if (CreateSchemaOnly())  return;

    FdoPtr <FdoIActivateLongTransaction> activate;
    FdoPtr <FdoIDeactivateLongTransaction> deactivate;
    FdoPtr <FdoILongTransactionConflictDirectiveEnumerator> enumerator;
    FdoPtr <FdoIRollbackLongTransaction> rollback;

    try
    {
        // Create an ArcCatalog-like version:
        FdoStringP wNewVersionName = CreateArcCatalogLikeVersion(SE_VERSION_ACCESS_PRIVATE);

        // Activate this new version:
        activate = (FdoIActivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_ActivateLongTransaction);
        activate->SetName (wNewVersionName);
        activate->Execute ();

        // Verify that it is now active:
        FdoPtr<FdoIGetLongTransactions> getLTs = (FdoIGetLongTransactions*)mConnection->CreateCommand(FdoCommandType_GetLongTransactions);
        FdoPtr<FdoILongTransactionReader> ltReader = getLTs->Execute();
        bool bFound = false;
        while (ltReader->ReadNext())
        {
            if (0==wcscmp(ltReader->GetName(),(FdoString*)wNewVersionName))
            {
                bFound = true;
                CPPUNIT_ASSERT_MESSAGE("Version should be active", ltReader->IsActive());
            }
        }
        ltReader->Close();
        CPPUNIT_ASSERT_MESSAGE("Version was not found", bFound);

        // Deactivate this new version:
        deactivate = (FdoIDeactivateLongTransaction*)mConnection->CreateCommand (FdoCommandType_DeactivateLongTransaction);
        deactivate->Execute();

        // Verify that it is now not active:
        ltReader = getLTs->Execute();
        while (ltReader->ReadNext())
        {
            if (0==wcscmp(ltReader->GetName(),(FdoString*)wNewVersionName))
            {
                CPPUNIT_ASSERT_MESSAGE("Version should NOT be active", !ltReader->IsActive());
            }
        }
        ltReader->Close();

        // Rollback the new version:
        rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (wNewVersionName);
        rollback->Execute ();

        // Make sure the version no longer exists:
        getLTs = (FdoIGetLongTransactions*)mConnection->CreateCommand(FdoCommandType_GetLongTransactions);
        ltReader = getLTs->Execute();
        bFound = false;
        while (ltReader->ReadNext())
            if (0==wcscmp(ltReader->GetName(),(FdoString*)wNewVersionName))
                bFound = true;
        ltReader->Close();
        CPPUNIT_ASSERT_MESSAGE("Version was found", !bFound);
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}


void LongTransactionTests::commit_arccatalog_version ()
{
    if (CreateSchemaOnly())  return;

    FdoPtr <FdoICommitLongTransaction> commit;

    try
    {
        // Create an ArcCatalog-like version:
        FdoStringP wNewVersionName = CreateArcCatalogLikeVersion(SE_VERSION_ACCESS_PRIVATE);

        // Commit the new version:
        commit = (FdoICommitLongTransaction*)mConnection->CreateCommand (FdoCommandType_CommitLongTransaction);
        commit->SetName (wNewVersionName);
        FdoPtr<FdoILongTransactionConflictDirectiveEnumerator> conflictReader = commit->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("Didnt expect any version commit conflicts", !conflictReader->ReadNext());

        // Make sure the version no longer exists:
        FdoPtr<FdoIGetLongTransactions> getLTs = (FdoIGetLongTransactions*)mConnection->CreateCommand(FdoCommandType_GetLongTransactions);
        FdoPtr<FdoILongTransactionReader> ltReader = getLTs->Execute();
        bool bFound = false;
        while (ltReader->ReadNext())
            if (0==wcscmp(ltReader->GetName(),(FdoString*)wNewVersionName))
                bFound = true;
        ltReader->Close();
        CPPUNIT_ASSERT_MESSAGE("Version was found", !bFound);
    }
    catch (FdoException* ge)
    {
        fail (ge);
    }
}



FdoStringP LongTransactionTests::CreateArcCatalogLikeVersion(long accessLevel)
{
    // Establish a low-level connection with ArcSDE server:
    SE_ERROR error;
    SE_CONNECTION connection;
    LONG result;
    FdoStringP server(ArcSDETestConfig::Server());
    FdoStringP instance(ArcSDETestConfig::Instance());
    FdoStringP username(ArcSDETestConfig::UserNameMetadcov());
    FdoStringP password(L"test");
    result = SE_connection_create (
        server,
        instance,
        NULL,
        username,
        password,
        &error,
        &connection);
    CPPUNIT_ASSERT(result == SE_SUCCESS);

    // Get the default version's current state id:
    LONG state_id = -1;
    SE_VERSIONINFO versioninfo;
    char newVersionName[SE_MAX_VERSION_LEN+1];
    result = SE_versioninfo_create(&versioninfo);
    CPPUNIT_ASSERT(result == SE_SUCCESS);
    result = SE_version_get_info (connection, SE_QUALIFIED_DEFAULT_VERSION_NAME, versioninfo);
    CPPUNIT_ASSERT(result == SE_SUCCESS);
    result = SE_versioninfo_get_state_id (versioninfo, &state_id);
    CPPUNIT_ASSERT(result == SE_SUCCESS);
    SE_versioninfo_free(versioninfo);

    // Create a new version like ArcCatalog does it: pointing to the same state as the default version is pointing to,
    // and with access PRIVATE instead of PUBLIC:
    result = SE_versioninfo_create(&versioninfo);
    CPPUNIT_ASSERT(result == SE_SUCCESS);
    result = SE_versioninfo_set_name(versioninfo, "ARCSDE_TEST_ROLLBACK");
    CPPUNIT_ASSERT(result == SE_SUCCESS);
    result = SE_versioninfo_set_parent_name(versioninfo, SE_QUALIFIED_DEFAULT_VERSION_NAME);
    CPPUNIT_ASSERT(result == SE_SUCCESS);
    result = SE_versioninfo_set_state_id (versioninfo, state_id);
    CPPUNIT_ASSERT(result == SE_SUCCESS);
    result = SE_versioninfo_set_access (versioninfo, accessLevel);
    CPPUNIT_ASSERT(result == SE_SUCCESS);
    result = SE_version_create(connection, versioninfo, TRUE, versioninfo);
    CPPUNIT_ASSERT(result == SE_SUCCESS);
    result = SE_versioninfo_get_name(versioninfo, newVersionName);
    CPPUNIT_ASSERT(result == SE_SUCCESS);

    // Close our connection:
    SE_connection_free(connection);

    FdoStringP wNewVersionName(newVersionName);
    return wNewVersionName;
}


void LongTransactionTests::read_protected_version()
{
    if (CreateSchemaOnly())  return;

    FdoStringP wNewVersionName;
    FdoPtr <FdoIActivateLongTransaction> activate;

    try
    {
        // Create an ArcCatalog-like *protected* version:
        wNewVersionName = CreateArcCatalogLikeVersion(SE_VERSION_ACCESS_PROTECTED);

        // Establish a second connection as a different user:
    	FdoPtr<FdoIConnection> conn2 = ArcSDETests::GetConnection ();
        conn2->SetConnectionString (ArcSDETestConfig::ConnStringOzzie());
        conn2->Open ();

        // Activate this new version, as different user:
        activate = (FdoIActivateLongTransaction*)conn2->CreateCommand (FdoCommandType_ActivateLongTransaction);
        activate->SetName (wNewVersionName);
        activate->Execute ();

        // Try selecting data from versioned table, as different user:
        FdoPtr<FdoISelect> select = (FdoISelect*)conn2->CreateCommand(FdoCommandType_Select);
        select->SetFeatureClassName(ArcSDETestConfig::QClassNameTrees());
        FdoPtr<FdoIFeatureReader> rdr = select->Execute();
        while (rdr->ReadNext())
        {
            // do nothing
        }
    }
    catch (FdoException* ge)
    {
        fail (ge);
    }

    // clean up this new version:
    try
    {
        FdoPtr <FdoIRollbackLongTransaction> rollback = (FdoIRollbackLongTransaction*)mConnection->CreateCommand (FdoCommandType_RollbackLongTransaction);
        rollback->SetName (wNewVersionName);
        rollback->Execute ();
    }
    catch (FdoException* ge)
    {
        // ignore error here
    }
}
