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
#include "ThreadingTests.h"
#include "FdoCommonThreadUtil.h"


CPPUNIT_TEST_SUITE_REGISTRATION (ThreadingTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (ThreadingTests, "ThreadingTests");

#define ITERATIONS 25

// error statuses:
#define OK 0
#define NONE_FOUND 1
#define UPDATE_FAILED 2
#define WRONG_VALUE 3
#define CONFLICT 4
#define EXCEPTION 5
#define SELECTALL_FAILED 6

static FdoStringP CONNECT_STRINGS[4];
static const int iFeatureCount = 10;
static int iFeatNums[iFeatureCount];  // feature numbers of inserted rows


static UnitTestData *property0;
static UnitTestData *property1;
static UnitTestClass *theClass;


struct Context
{
    int mId;
    FdoString* mConnectString;
    int* mFault;
    FdoException** mException;
};




/*
  In order to pinpoint a transaction hole between the gathering other users locks
  and actually locking objects, we create an object and try to lock it with two threads
  using FdoLockStrategy_Partial, where the attempt is made to lock objects even if a 
  conflict is detected.
  The object will either show up in the conflicts list or in the feature reader,
  but not both and not in neither, because it's either locked or a conflict.
  So, if it doesn't show up in either, or it shows up in both, there is a fault.
*/
FDOCOMMON_THREAD_FUNCTION_SIGNATURE Worker (void* arg)
{
    Context* context = (Context*)arg;
    *(context->mFault) = OK;

    try
    {
        FdoPtr <FdoIConnection> connection;
        FdoPtr <FdoITransaction> transaction;
        FdoPtr<FdoISelect> select;
        FdoPtr<FdoIUpdate> update;
        FdoPtr <FdoIReleaseLock> release;
        bool conflict;
        int iterations;
        int iFeatNum;
        wchar_t filter[1024];
        const wchar_t* old;
        double stale;
        double fresh;
        wchar_t random[64];

        // seed the random number generator
        srand ((unsigned)time (NULL));

	    connection = ArcSDETests::GetConnection ();
        connection->SetConnectionString (context->mConnectString);
        connection->Open ();
        select = (FdoISelect*)connection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (theClass->mName);
        select->SetLockType (FdoLockType_Exclusive);
        select->SetLockStrategy (FdoLockStrategy_All);
        update = (FdoIUpdate*)connection->CreateCommand (FdoCommandType_Update);
        update->SetFeatureClassName (theClass->mName);
        release = (FdoIReleaseLock*)connection->CreateCommand (FdoCommandType_ReleaseLock);
        release->SetFeatureClassName (theClass->mName);

        iterations = 0;
        do
        {
            // select all rows, the most common operation:
            FdoPtr<FdoISelect> selectAll = (FdoISelect*)connection->CreateCommand (FdoCommandType_Select);
            selectAll->SetFeatureClassName(theClass->mName);
            FdoPtr<FdoIFeatureReader> readerAll = selectAll->Execute();
            int countAll=0;
            while (readerAll->ReadNext())
            {
                countAll++;
            }
            if (countAll != 10)
                if (OK == *(context->mFault))
                    *(context->mFault) = SELECTALL_FAILED;

            // pick a random feature out of the N available:
            int iRandomIndex = rand() % iFeatureCount;
            iFeatNum = iFeatNums[iRandomIndex];

            swprintf (filter, 1023, L"%ls = %d", property0->mPropertyName, iFeatNum);
            //printf ("\nthread %d: id %d", context->mId, iFeatNum);
            //fflush (stdout);

            // get the old value
            old = property1->mPropertyData[iRandomIndex];
            stale = FdoCommonOSUtil::wtof (old);

            // pick a new value
            fresh = (double)rand () / 100.0;
            swprintf (random, 63, L"%f", fresh);

            // lock it
            select->SetFilter (filter);
            conflict = true;
            do
            {
                FdoPtr<FdoIFeatureReader> reader;
                FdoPtr<FdoILockConflictReader> conflicts;

                transaction = connection->BeginTransaction ();
                reader = select->ExecuteWithLock ();
                conflicts = select->GetLockConflicts ();
                if (!reader->ReadNext ())
                    *(context->mFault) = NONE_FOUND;
                else
                {
                    conflict = conflicts->ReadNext ();

                    // check the value
                    if (!conflict)
                        if (fabs (reader->GetDouble (property1->mPropertyName) - stale) > 1e-6)
                            *(context->mFault) = WRONG_VALUE;
                }
                conflicts->Close ();
                reader->Close ();
                transaction->Commit ();
            }
            while ((OK == *(context->mFault)) && conflict);
            //printf ("\nthread %d: id %d locked", context->mId, iFeatNum);
            //fflush (stdout);

            if (OK == *(context->mFault))
            try // guard against permanently locking the feature leading to thread spin
            {
                // change the record
                transaction = connection->BeginTransaction ();
                {
                    FdoPtr<FdoPropertyValueCollection> values;
                    FdoPtr<FdoPropertyValue> value;
                    FdoPtr<FdoValueExpression> expression;

                    values = update->GetPropertyValues ();
                    values->Clear ();
                    expression = (FdoValueExpression*)ArcSDETests::ParseByDataType (random, property1->mPropertyType);
                    value = FdoPropertyValue::Create (property1->mPropertyName, expression);
                    values->Add (value);
                }
                update->SetFilter (filter);
                if (1 != update->Execute ())
                {
                    *(context->mFault) = UPDATE_FAILED;
                    transaction->Commit ();
                }
                else
                {
                    transaction->Commit ();
                    //printf ("\nthread %d: id %d changed to %f", context->mId, iFeatNum, fresh);
                    //fflush (stdout);
                    // check it's changed
                    {
                        FdoPtr<FdoIFeatureReader> reader;

                        reader = select->Execute (); // WithLock?
                        if (!reader->ReadNext ())
                            *(context->mFault) = NONE_FOUND;
                        else
                        {
                            //conflicts = select->GetLockConflicts ();
                            //if (conflicts->ReadNext ())
                            //    *(context->mFault) = CONFLICT; // we already have it locked
                            //else
                            //{
                                // check the value
                                if (fabs (reader->GetDouble (property1->mPropertyName) - fresh) > 1e-6)
                                    *(context->mFault) = WRONG_VALUE;
                            //}
                        }
                        reader->Close ();
                    }
                    //printf ("\nthread %d: id %d checked", context->mId, iFeatNum);
                    //fflush (stdout);

                    // change it back
                    transaction = connection->BeginTransaction ();
                    {
                        FdoPtr<FdoPropertyValueCollection> values;
                        FdoPtr<FdoPropertyValue> value;
                        FdoPtr<FdoValueExpression> expression;

                        values = update->GetPropertyValues ();
                        values->Clear ();
                        expression = (FdoValueExpression*)ArcSDETests::ParseByDataType (old, property1->mPropertyType);
                        value = FdoPropertyValue::Create (property1->mPropertyName, expression);
                        values->Add (value);
                    }
                    update->SetFilter (filter);
                    if (1 != update->Execute ())
                        if (OK == *(context->mFault))
                            *(context->mFault) = UPDATE_FAILED;
                    transaction->Commit ();
                    //printf ("\nthread %d: id %d restored", context->mId, iFeatNum);
                    //fflush (stdout);

                    // unlock it
                    release->SetFilter (filter);
                    transaction = connection->BeginTransaction ();
                    {
                        FdoPtr<FdoILockConflictReader> conflicts;
                        conflicts = release->Execute ();
                        conflicts->Close ();
                    }
                    transaction->Commit ();
                    //printf ("\nthread %d: id %d unlocked", context->mId, iFeatNum);
                    //fflush (stdout);
                }
            }
            catch (FdoException* ge)
            {
                release->SetFilter (L"1=1");
                FdoPtr<FdoILockConflictReader> conflicts = release->Execute ();
                throw ge;
            }

            iterations++;
        }
        while ((OK == *(context->mFault)) && (iterations < ITERATIONS));
        connection->Close ();
    }
    catch (FdoException* ge) 
    {
        *(context->mFault) = EXCEPTION;
        *(context->mException) = ge;
    }

    FDOCOMMON_ENDTHREAD(0);
} 


ThreadingTests::ThreadingTests (void)
{
    theClass = NULL;
    property1 = NULL;
    property0 = NULL;
}

ThreadingTests::~ThreadingTests (void)
{
}

void ThreadingTests::setUp ()
{
    static bool bSchemaCreated = false;

    // Setup test data:
    property0 = new UnitTestData(L"FeatNum", L"Unique identifier", FdoDataType_Int32, 0,  0, 0, false, true, true,
                             (wchar_t*)NULL);
    property1 = new UnitTestData(L"Length", L"The feature length.", FdoDataType_Double, 0,  0, 0, false, false, false,
                             L"4.5", L"7.8", L"9.2", L"1.7", L"2.4", L"6.3", L"9.7", L"1.1", L"0.5", L"4.3", (wchar_t*)NULL);
    theClass = new UnitTestClass(ArcSDETestConfig::QClassNameRoads(), L"Test class definition.", 0, true, false, property0, property1, NULL);
    CONNECT_STRINGS[0] = ArcSDETestConfig::ConnStringFdoTest1();
    CONNECT_STRINGS[1] = ArcSDETestConfig::ConnStringAustralia();
    CONNECT_STRINGS[2] = ArcSDETestConfig::ConnStringMetadcov();
    CONNECT_STRINGS[3] = ArcSDETestConfig::ConnStringWheaton();

    // Create test schema, if requested:
    if (CreateSchemaOnly() && !bSchemaCreated)
    {
        // clean up previous tests and establish a class to play with
        FdoPtr <FdoIConnection> connection;
	    connection = ArcSDETests::GetConnection ();
        connection->SetConnectionString (CONNECT_STRINGS[0]);
        connection->Open ();

        ArcSDETests::CleanUpClass (connection, ArcSDETestConfig::ClassSchemaRoads(), theClass->mName);
        ArcSDETests::CreateSchema (connection, theClass);

        connection->Close ();

        bSchemaCreated = true;
    }
}

void ThreadingTests::tearDown ()
{
    // Cleanup test data:
    delete theClass;  theClass = NULL;
    delete property1;  property1 = NULL;
    delete property0;  property0 = NULL;
}

/* Test running a couple of threads */
void ThreadingTests::a_few_threads ()
{
#ifdef _WIN32
    CPPUNIT_FAIL("ThreadingTests::a_few_threads disabled");
#endif

    FdoPtr <FdoIConnection> connection;
    FDOCOMMON_THREAD_HANDLE* handles;
    bool success;
    int count;
#define THREADS 4
#define SECONDS 1000
    int* faults;
    Context* context;
    FdoException** exceptions;

    if (CreateSchemaOnly()) return;
    if (!ArcSDETestConfig::SupportsTransactionIsolation()) return;

    try
    {
        // clean up previous leftover locks (if any):
        for (int i=0; i<THREADS; i++)
        {
	        connection = ArcSDETests::GetConnection ();
            connection->SetConnectionString (CONNECT_STRINGS[i]);
            connection->Open ();
            FdoPtr<FdoIReleaseLock> rl = (FdoIReleaseLock*)connection->CreateCommand(FdoCommandType_ReleaseLock);
            rl->SetFeatureClassName(theClass->mName);
            FdoPtr<FdoILockConflictReader> conflicts = rl->Execute();  // ignore returned conflicts
            connection->Close();
        }

        // clean up previous tests and establish a class to play with
	    connection = ArcSDETests::GetConnection ();
        connection->SetConnectionString (CONNECT_STRINGS[0]);
        connection->Open ();

        ArcSDETests::CleanUpClass (connection, ArcSDETestConfig::ClassSchemaRoads(), theClass->mName, true);

        for (size_t i = 0; i < property1->mPropertyData.size (); i++)
            iFeatNums[i] = ArcSDETests::Insert (connection, theClass, i);
        connection->Close ();

        // establish a couple of threads
        handles = (FDOCOMMON_THREAD_HANDLE*)alloca (THREADS * sizeof (FDOCOMMON_THREAD_HANDLE));
        faults = (int*)alloca (THREADS * sizeof (int));
        exceptions = (FdoException**) alloca (THREADS * sizeof (FdoException*));
        for (int i = 0; i < THREADS; i++)
        {
            faults[i] = 0;
            exceptions[i] = NULL;
        }
        count = 0;
        for (int i = 0; i < THREADS; i++)
        {
            context = (Context*)alloca (sizeof (Context));
            context->mId = i;
            context->mConnectString = (FdoString*)CONNECT_STRINGS[count];
            context->mFault = &faults[count];
            context->mException = &exceptions[count];

	        if (!FdoCommonThreadUtil::CreateThread(&(handles[count]), (void*)Worker, (void*)context))
            {
                //printf ("error (%d) starting thread #%d", errno, count);
                break;
            }
            else
                count++;
        }

        success = FdoCommonThreadUtil::WaitForThreads(count, handles, SECONDS * 1000);

        for (int i = 0; i < count; i++)
            FdoCommonThreadUtil::CloseThread(handles[i]);

        CPPUNIT_ASSERT_MESSAGE ("thread timeout", success);

        // check for fault
        for (int i = 0; i < THREADS; i++)
            if (0 != faults[i])
                switch (faults[i])
                {
                    case NONE_FOUND:
                        CPPUNIT_FAIL ("execute with lock didn't return an object");
                        break;
                    case UPDATE_FAILED:
                        CPPUNIT_FAIL ("an update operation failed");
                        break;
                    case WRONG_VALUE:
                        CPPUNIT_FAIL ("the object had a wrong value");
                        break;
                    case CONFLICT:
                        CPPUNIT_FAIL ("an unexpected conflict occured");
                        break;
                    case EXCEPTION:
                        wprintf (L"exception: %ls\n", exceptions[i]->GetExceptionMessage ());
                        if (NULL != exceptions[i]->GetRootCause ())
                            wprintf (L"cause: %ls\n", exceptions[i]->GetRootCause ()->GetExceptionMessage ());
                        CPPUNIT_FAIL ("exception");
                        break;
                    case SELECTALL_FAILED:
                        CPPUNIT_FAIL ("a select all operation failed");
                        break;
                }
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }

#undef SECONDS
#undef THREADS
}
