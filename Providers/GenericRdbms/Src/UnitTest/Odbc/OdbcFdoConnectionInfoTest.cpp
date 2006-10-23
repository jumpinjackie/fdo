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
#include "OdbcFdoConnectionInfoTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION(OdbcMySqlFdoConnectionInfoTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OdbcMySqlFdoConnectionInfoTest, "FdoConnectionInfoTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OdbcMySqlFdoConnectionInfoTest, "OdbcMySqlFdoConnectionInfoTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OdbcMySqlFdoConnectionInfoTest, "OdbcMySqlTests");

CPPUNIT_TEST_SUITE_REGISTRATION(OdbcOracleFdoConnectionInfoTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OdbcOracleFdoConnectionInfoTest, "FdoConnectionInfoTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OdbcOracleFdoConnectionInfoTest, "OdbcOracleFdoConnectionInfoTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OdbcOracleFdoConnectionInfoTest, "OdbcOracleTests");

#ifdef _WIN32

CPPUNIT_TEST_SUITE_REGISTRATION(OdbcSqlServerFdoConnectionInfoTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OdbcSqlServerFdoConnectionInfoTest, "FdoConnectionInfoTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OdbcSqlServerFdoConnectionInfoTest, "OdbcSqlServerFdoConnectionInfoTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OdbcSqlServerFdoConnectionInfoTest, "OdbcSqlServerTests");

CPPUNIT_TEST_SUITE_REGISTRATION(OdbcAccessFdoConnectionInfoTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OdbcAccessFdoConnectionInfoTest, "FdoConnectionInfoTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OdbcAccessFdoConnectionInfoTest, "OdbcAccessFdoConnectionInfoTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OdbcAccessFdoConnectionInfoTest, "OdbcSqlServerTests");

CPPUNIT_TEST_SUITE_REGISTRATION(OdbcExcelFdoConnectionInfoTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OdbcExcelFdoConnectionInfoTest, "FdoConnectionInfoTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OdbcExcelFdoConnectionInfoTest, "OdbcExcelFdoConnectionInfoTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OdbcExcelFdoConnectionInfoTest, "OdbcExcelTests");

CPPUNIT_TEST_SUITE_REGISTRATION(OdbcTextFdoConnectionInfoTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OdbcTextFdoConnectionInfoTest, "FdoConnectionInfoTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OdbcTextFdoConnectionInfoTest, "OdbcTextFdoConnectionInfoTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OdbcTextFdoConnectionInfoTest, "OdbcTextTests");

#endif


// ------------------------- ODBC MYSQL TEST CASE -------------------------

void OdbcMySqlFdoConnectionInfoTest::set_provider ()
{

    UnitTestUtil::SetProvider("OdbcMySql");

}  //  set_provider ()

void OdbcMySqlFdoConnectionInfoTest::SetExpectedFileList ()
{

    // This is a database provider and hence does not have a list of
    // dependent files.

}  //  SetExpectedFileList ()

void OdbcMySqlFdoConnectionInfoTest::TestProviderInfo ()
{

    try {

        // Establish the connection and execute the test. 

		mConnection = UnitTestUtil::GetProviderConnectionObject();
        FdoStringP userConnectionString =
                UnitTestUtil::GetConnectionString(Connection_WithDatastore);
        mConnection->SetConnectionString(userConnectionString);
        mConnection->Open();

        CheckDatabaseBasedProviderInfo();

        mConnection->Close();

    }  //  try ...

	catch (FdoException *e) {

        mConnection->Close();
        FdoStringP errMsg = e->GetExceptionMessage();
        const char *failMsg = (const char*) errMsg;
        CPPUNIT_FAIL(failMsg);

	}  //  catch ...

}  //  TestProviderInfo ()


// ------------------------- ODBC ORACLE TEST CASE -------------------------

void OdbcOracleFdoConnectionInfoTest::set_provider ()
{

    UnitTestUtil::SetProvider("OdbcOracle");

}  //  set_provider ()

void OdbcOracleFdoConnectionInfoTest::SetExpectedFileList ()
{

    // This is a database provider and hence does not have a list of
    // dependent files.

}  //  SetExpectedFileList ()

void OdbcOracleFdoConnectionInfoTest::TestProviderInfo ()
{

    try {

        // Establish the connection and execute the test. 

		mConnection = UnitTestUtil::GetProviderConnectionObject();
        FdoStringP userConnectionString =
                UnitTestUtil::GetConnectionString(Connection_WithDatastore);
        mConnection->SetConnectionString(userConnectionString);
        mConnection->Open();

        CheckDatabaseBasedProviderInfo();

        mConnection->Close();

    }  //  try ...

	catch (FdoException *e) {

        mConnection->Close();
        FdoStringP errMsg = e->GetExceptionMessage();
        const char *failMsg = (const char*) errMsg;
        CPPUNIT_FAIL(failMsg);

	}  //  catch ...

}  //  TestProviderInfo ()


// ------------------------- ODBC SQL SERVER TEST CASE -------------------------

#ifdef _WIN32

void OdbcSqlServerFdoConnectionInfoTest::set_provider ()
{

    UnitTestUtil::SetProvider("OdbcSqlServer");

}  //  set_provider ()

void OdbcSqlServerFdoConnectionInfoTest::SetExpectedFileList ()
{

    // This is a database provider and hence does not have a list of
    // dependent files.

}  //  SetExpectedFileList ()

void OdbcSqlServerFdoConnectionInfoTest::TestProviderInfo ()
{

    try {

        // Establish the connection and execute the test. 

		mConnection = UnitTestUtil::GetProviderConnectionObject();
        FdoStringP userConnectionString =
                UnitTestUtil::GetConnectionString(Connection_WithDatastore);
        mConnection->SetConnectionString(userConnectionString);
        mConnection->Open();

        CheckDatabaseBasedProviderInfo();

        mConnection->Close();

    }  //  try ...

	catch (FdoException *e) {

        mConnection->Close();
        FdoStringP errMsg = e->GetExceptionMessage();
        const char *failMsg = (const char*) errMsg;
        CPPUNIT_FAIL(failMsg);

	}  //  catch ...

}  //  TestProviderInfo ()

#endif


// ------------------------- ODBC ACCESS TEST CASE -------------------------

#ifdef _WIN32

void OdbcAccessFdoConnectionInfoTest::set_provider ()
{

    UnitTestUtil::SetProvider("OdbcAccess");

}  //  set_provider ()

void OdbcAccessFdoConnectionInfoTest::SetExpectedFileList ()
{

    // As this is a file-based provider, there should be a list of dependent
    // files. The list should include at least the file "MSTest.mdb". This
    // one is added to the list of expected files for cross-checking.

    ClearFileNameList();
    AddFileNameToList(L"MSTest.mdb");

}  //  SetExpectedFileList ()

void OdbcAccessFdoConnectionInfoTest::TestProviderInfo ()
{

    FdoStringP connectionStringDriver =
                                L"Driver={Microsoft Access Driver (*.mdb)}";
    FdoStringP connectionStringDbq = L"DBQ=MsTest.mdb";
    FdoStringP userConnectionString;

    try {

        // For cross checking, set the list of expected dependent files.

        SetExpectedFileList();

        // Establish the connection and execute the test. 

        printf("ODBC Access Test: Case 1: \n");
		mConnection = UnitTestUtil::GetProviderConnectionObject();
        userConnectionString =
                UnitTestUtil::GetConnectionString(Connection_WithDatastore);
        ExecuteTest(userConnectionString);
        mConnection->Release();
        mConnection = NULL;

        printf("\nODBC Access Test: Case 2: \n");
		mConnection = UnitTestUtil::GetProviderConnectionObject();
        userConnectionString =
                     FdoStringP::Format(L"ConnectionString=\"%ls; %ls\"",
                                        (FdoString *)connectionStringDriver,
                                        (FdoString *)connectionStringDbq);
        ExecuteTest(userConnectionString);
        mConnection->Release();
        mConnection = NULL;

        // Clear the list of expected dependent files.

        ClearFileNameList ();

    }  //  try ...

	catch (FdoException *e) {

        mConnection->Close();
        FdoStringP errMsg = e->GetExceptionMessage();
        const char *failMsg = (const char*) errMsg;
        CPPUNIT_FAIL(failMsg);

	}  //  catch ...

}  //  TestProviderInfo ()

void OdbcAccessFdoConnectionInfoTest::ExecuteTest (FdoStringP connectionString)
{

    mConnection->SetConnectionString(connectionString);
    mConnection->Open();

    CheckFileBasedProviderInfo();

    mConnection->Close();

}  //  ExecuteTest ()

#endif


// ------------------------- ODBC EXCEL TEST CASE -------------------------

#ifdef _WIN32

void OdbcExcelFdoConnectionInfoTest::set_provider ()
{

    UnitTestUtil::SetProvider("OdbcExcel");

}  //  set_provider ()

void OdbcExcelFdoConnectionInfoTest::SetExpectedFileList ()
{

    // As this is a file-based provider, there should be a list of dependent
    // files. The list should include at least the file "MSTest.xls". This
    // one is added to the list of expected files for cross-checking.

    ClearFileNameList();
    AddFileNameToList(L"MSTest.xls");

}  //  SetExpectedFileList ()

void OdbcExcelFdoConnectionInfoTest::TestProviderInfo ()
{

    try {

        // For cross checking, set the list of expected dependent files.

        SetExpectedFileList();

        // Establish the connection and execute the test. 

		mConnection = UnitTestUtil::GetProviderConnectionObject();
        FdoStringP userConnectionString =
                UnitTestUtil::GetConnectionString(Connection_WithDatastore);
        ExecuteTest(userConnectionString);

        // Clear the list of expected dependent files.

        ClearFileNameList ();

    }  //  try ...

	catch (FdoException *e) {

        mConnection->Close();
        FdoStringP errMsg = e->GetExceptionMessage();
        const char *failMsg = (const char*) errMsg;
        CPPUNIT_FAIL(failMsg);

	}  //  catch ...

}  //  TestProviderInfo ()

void OdbcExcelFdoConnectionInfoTest::ExecuteTest (FdoStringP connectionString)
{

    mConnection->SetConnectionString(connectionString);
    mConnection->Open();

    CheckFileBasedProviderInfo();

    mConnection->Close();

}  //  ExecuteTest ()

#endif


// ------------------------- ODBC TEXT TEST CASE -------------------------

#ifdef _WIN32

void OdbcTextFdoConnectionInfoTest::set_provider ()
{

    UnitTestUtil::SetProvider("OdbcText");

}  //  set_provider ()

void OdbcTextFdoConnectionInfoTest::SetExpectedFileList ()
{

    // As this is a file-based provider, there should be a list of dependent
    // files. The list should include at least the file "Country-Text.txt".
    // This one is added to the list of expected files for cross-checking.

    ClearFileNameList();
    AddFileNameToList(L"Country-Text.txt");

}  //  SetExpectedFileList ()

void OdbcTextFdoConnectionInfoTest::TestProviderInfo ()
{

    try {

        // For cross checking, set the list of expected dependent files.

        SetExpectedFileList();

        // Establish the connection and execute the test. 

		mConnection = UnitTestUtil::GetProviderConnectionObject();
        FdoStringP userConnectionString =
                UnitTestUtil::GetConnectionString(Connection_WithDatastore);
        ExecuteTest(userConnectionString);

        // Clear the list of expected dependent files.

        ClearFileNameList ();

    }  //  try ...

	catch (FdoException *e) {

        mConnection->Close();
        FdoStringP errMsg = e->GetExceptionMessage();
        const char *failMsg = (const char*) errMsg;
        CPPUNIT_FAIL(failMsg);

	}  //  catch ...

}  //  TestProviderInfo ()

void OdbcTextFdoConnectionInfoTest::ExecuteTest (FdoStringP connectionString)
{

    mConnection->SetConnectionString(connectionString);
    mConnection->Open();

    CheckFileBasedProviderInfo();

    mConnection->Close();

}  //  ExecuteTest ()

#endif
