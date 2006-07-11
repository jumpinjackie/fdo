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

#include "Pch.h"
#include "ConnectTests.h"
#include <FdoCommonFile.h>

#ifdef _WIN32
#define LOCATION L"..\\..\\TestData\\Ontario\\"
#define LOCATION_NO_SLASH L"..\\..\\TestData\\Ontario"
#define LOCATION_WRONG_SLASH L"..\\..\\TestData\\Ontario/"
#define LOCATION_DOUBLE_SLASH L"..\\\\..\\\\TestData\\\\Ontario\\\\"
#else
#define LOCATION L"../../TestData/Ontario/"
#define LOCATION_NO_SLASH L"../../TestData/Ontario"
#define LOCATION_WRONG_SLASH L"../../TestData/Ontario\\"
#define LOCATION_DOUBLE_SLASH L"..//..//TestData//Ontario//"
#endif

wchar_t* GConnectStr[] = {NULL, L"", L"DefaultFileLocation= ..\\..\\TestData\\Ontario\\;",
						L"DefaultFileLocation=..\\..\\TestData\\Ontario\\ ; ",
						L"DefaultFileLocation=..\\..\\TestData\\Ontario\\ ;TemporaryFileLocation=",
						L"DefaultFileLocation=..\\..\\TestData\\Ontario\\;TemporaryFileLocation=;",
						L"DefaultFileLocation = ..\\..\\TestData\\Ontario\\;TemporaryFileLocation=",
						L"DefaultFileLocation = ..\\..\\TestData\\Ontario\\;TemporaryFileLocation = ;",
						L"TemporaryFileLocation = ;DefaultFileLocation = ..\\..\\TestData\\Ontario\\",
						L"DefaultFileLocation=",
						L"DefaultFileLocation=;;",
						L"DefaultFileLocation=..\\..\\TestData\\Ontario\\;TemporaryFileLocation = ",
						L"DefaultFileLocation= ;TemporaryFileLocation = ;",
						L"DefaultFileLocation = ;;;;;TemporaryFileLocation   =   ;;;;;;;;;",
						L"DefaultFileLocation = ..\\..\\TestData\\Ontario\\ ;;;;;TemporaryFileLocation   =   ;;;;;;;;;"
						};

CPPUNIT_TEST_SUITE_REGISTRATION (ConnectTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (ConnectTests, "ConnectTests");

FdoPtr<FdoIConnection> ConnectTests::mConnection;

ConnectTests::ConnectTests (void)
{
}

ConnectTests::~ConnectTests (void)
{
}

void ConnectTests::setUp ()
{
    if (FdoCommonFile::FileExists (LOCATION L"schema.xml"))
        FdoCommonFile::Delete (LOCATION L"schema.xml");
}

void ConnectTests::tearDown ()
{
	FDO_SAFE_RELEASE(mConnection.p);
}

void ConnectTests::connect ()
{
    try
    {
        mConnection = ShpTests::GetConnection ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
        mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION);
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
        FdoConnectionState state = mConnection->Open ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == state);
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->GetConnectionState ());
        mConnection->Close ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
    }
    catch (FdoException* ge)
    {
        fail (ge);
    }
    catch (...)
    {
        CPPUNIT_FAIL ("non-FdoException");
    }
}

void ConnectTests::connect2 ()
{
    try
    {
        mConnection = ShpTests::GetConnection ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
        mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION);
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
        FdoConnectionState state = mConnection->Open ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == state);
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->GetConnectionState ());
        mConnection->Close ();
    }
    catch (FdoException* ge)
    {
        fail (ge);
    }
    catch (...)
    {
        CPPUNIT_FAIL ("non-FdoException");
    }
    CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
}


void ConnectTests::connect3 ()
{
    try
    {
        mConnection = ShpTests::GetConnection ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
        mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION L"roads.shp");
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
        FdoConnectionState state = mConnection->Open ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == state);
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->GetConnectionState ());

        // Verify results of DescribeSchema:
        FdoPtr<FdoIDescribeSchema> descSchema = (FdoIDescribeSchema*)mConnection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = descSchema->Execute();
        CPPUNIT_ASSERT_MESSAGE ("Expecting 1 schema", schemas->GetCount()==1);
        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
        CPPUNIT_ASSERT_MESSAGE ("Expecting 'Default' schema", 0 == wcscmp (schema->GetName (), L"Default"));
        CPPUNIT_ASSERT_MESSAGE ("Expecting 'Default schema.' as the schema description", 0 == wcscmp (schema->GetDescription (), L"Default schema."));
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();
        CPPUNIT_ASSERT_MESSAGE ("Expecting 1 class", classes->GetCount()==1);
        FdoPtr<FdoClassDefinition> classDef = classes->GetItem(0);
        CPPUNIT_ASSERT_MESSAGE ("Expecting a specific class, got wrong one", 0==wcscmp(classDef->GetName(), L"roads"));

        mConnection->Close ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
    }
    catch (FdoException* ge)
    {
        fail (ge);
    }
    catch (...)
    {
        CPPUNIT_FAIL ("non-FdoException");
    }
}

void ConnectTests::connect4 ()
{
    try
    {
        mConnection = ShpTests::GetConnection ();
		for (int idx = 0; idx < 15; idx++)
		{
			wchar_t* pConnectStr = GConnectStr[idx];
			CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
			mConnection->SetConnectionString (pConnectStr);
			CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
			FdoConnectionState state = mConnection->Open ();
			CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == state);
			CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->GetConnectionState ());
			mConnection->Close ();
		}
    }
    catch (FdoException* ge)
    {
        fail (ge);
    }
    catch (...)
    {
        CPPUNIT_FAIL ("non-FdoException");
    }
    CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
}

void ConnectTests::dual_connect ()
{
    try
    {
        FdoPtr<FdoIConnection> connection1;
        FdoConnectionState state1;
        FdoPtr<FdoIConnection> connection2;
        FdoConnectionState state2;
        FdoPtr<FdoIDescribeSchema> describe1;
        FdoPtr<FdoFeatureSchemaCollection> schemas1;
        FdoPtr<FdoIDescribeSchema> describe2;
        FdoPtr<FdoFeatureSchemaCollection> schemas2;
        FdoPtr<FdoFeatureSchema> schema1;
        FdoPtr<FdoClassCollection> classes1;
        FdoPtr<FdoFeatureSchema> schema2;
        FdoPtr<FdoClassCollection> classes2;
        FdoPtr<FdoClassDefinition> class1;
        FdoPtr<FdoClassDefinition> class2;

        // create one connection
        connection1 = ShpTests::GetConnection ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == connection1->GetConnectionState ());
        connection1->SetConnectionString (L"DefaultFileLocation=" LOCATION);
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == connection1->GetConnectionState ());
        state1 = connection1->Open ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == state1);
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == connection1->GetConnectionState ());

        // create a second connection for the same default file location
        connection2 = ShpTests::GetConnection ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == connection2->GetConnectionState ());
        connection2->SetConnectionString (L"DefaultFileLocation=" LOCATION);
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == connection2->GetConnectionState ());
        state2 = connection2->Open ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == state2);
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == connection2->GetConnectionState ());

        // try DescribeSchema on the first
        describe1 = (FdoIDescribeSchema*)connection1->CreateCommand (FdoCommandType_DescribeSchema);
        schemas1 = describe1->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting 1 schema", 1 == schemas1->GetCount ());

        // try DescribeSchema on the second...
        // SPR 667272 Cannot make multiple connections to the same SHP datastore
        // An exception will occur.  It will be something similar to this:
        // FdoException: The file '../../../input/SHP\blocks_primary.idx' cannot be opened.
        describe2 = (FdoIDescribeSchema*)connection2->CreateCommand (FdoCommandType_DescribeSchema);
        schemas2 = describe2->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting 1 schema", 1 == schemas2->GetCount ());

        // compare classes
        schema1 = schemas1->GetItem (0);
        classes1 = schema1->GetClasses ();
        schema2 = schemas2->GetItem (0);
        classes2 = schema2->GetClasses ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting same schema name", 0 == wcscmp (schema1->GetName (), schema2->GetName ()));
        for (int i = 0; i < classes1->GetCount (); i++)
        {
            class1 = classes1->GetItem (i);
            class2 = classes2->GetItem (i);
            CPPUNIT_ASSERT_MESSAGE ("schema's differ", 0 == wcscmp (class1->GetName (), class2->GetName ()));
        }

        connection1->Close ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == connection1->GetConnectionState ());
        connection2->Close ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == connection2->GetConnectionState ());
    }
    catch (FdoException* ge)
    {
        fail (ge);
    }
    catch (...)
    {
        CPPUNIT_FAIL ("non-FdoException");
    }
}

void ConnectTests::connect_no_slash ()
{
    try
    {
        // try with just trailing normal slashes
        mConnection = ShpTests::GetConnection ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
        mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION_NO_SLASH);
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
        FdoConnectionState state = mConnection->Open ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == state);
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->GetConnectionState ());

        // Verify results of DescribeSchema:
        FdoPtr<FdoIDescribeSchema> descSchema = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = descSchema->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting 1 schema", 1 == schemas->GetCount ());
        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);
        CPPUNIT_ASSERT_MESSAGE ("Expecting 'Default' schema", 0 == wcscmp (schema->GetName (), L"Default"));
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting more than 3 classes", 3 < classes->GetCount ());

        mConnection->Close ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
    }
    catch (FdoException* ge)
    {
        fail (ge);
    }
}

void ConnectTests::connect_back_slash ()
{
    try
    {
        // try with trailing cross-platform slashes
        mConnection = ShpTests::GetConnection ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
        mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION_WRONG_SLASH);
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
        FdoConnectionState state = mConnection->Open ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == state);
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->GetConnectionState ());

        // Verify results of DescribeSchema:
        FdoPtr<FdoIDescribeSchema> descSchema = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = descSchema->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting 1 schema", 1 == schemas->GetCount ());
        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);
        CPPUNIT_ASSERT_MESSAGE ("Expecting 'Default' schema", 0 == wcscmp (schema->GetName (), L"Default"));
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting more than 3 classes", 3 < classes->GetCount ());

        mConnection->Close ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
    }
    catch (FdoException* ge)
    {
        fail (ge);
    }
}

void ConnectTests::connect_double_slash ()
{
    try
    {
        // try with trailing cross-platform slashes
        mConnection = ShpTests::GetConnection ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
        mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION_DOUBLE_SLASH);
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
        FdoConnectionState state = mConnection->Open ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == state);
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->GetConnectionState ());

        // Verify results of DescribeSchema:
        FdoPtr<FdoIDescribeSchema> descSchema = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = descSchema->Execute ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting 1 schema", 1 == schemas->GetCount ());
        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);
        CPPUNIT_ASSERT_MESSAGE ("Expecting 'Default' schema", 0 == wcscmp (schema->GetName (), L"Default"));
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
        CPPUNIT_ASSERT_MESSAGE ("Expecting 4 classes", 4 == classes->GetCount ());

        mConnection->Close ();
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
    }
    catch (FdoException* ge)
    {
        fail (ge);
    }
}

void ConnectTests::connect_wrong_params ()
{
    mConnection = ShpTests::GetConnection ();
    CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
    try
    {
        mConnection->SetConnectionString (L"DefaultFLocation=" LOCATION);
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
        FdoConnectionState state = mConnection->Open ();
        CPPUNIT_ASSERT_MESSAGE ("open connection should fail", false);
    }
    catch (FdoException*)
	{
	}
    catch (...)
    {
        CPPUNIT_FAIL ("non-FdoException");
    }
	CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
    mConnection->Close ();
    CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
}


void ConnectTests::connect_wrong_params2 ()
{
    mConnection = ShpTests::GetConnection ();
    CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
    try
    {
		mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION L";TemporaryFLocation=C:\\tmp\\");
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
        FdoConnectionState state = mConnection->Open ();
        CPPUNIT_ASSERT_MESSAGE ("open connection should fail", false);
    }
    catch (FdoException*)
	{
	}
    catch (...)
    {
        CPPUNIT_FAIL ("non-FdoException");
    }
	CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
    mConnection->Close ();
    CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
}
void ConnectTests::connect_params1 ()
{
    mConnection = ShpTests::GetConnection ();
    CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
    try
    {
		mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION L";TemporaryFileLocation=");
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
        FdoConnectionState state = mConnection->Open ();
		CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Open == mConnection->GetConnectionState ());
		mConnection->Close ();
		CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
    }
    catch (FdoException* ge)
	{
		fail (ge);
	}
    catch (...)
    {
        CPPUNIT_FAIL ("non-FdoException");
    }
	CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
    mConnection->Close ();
    CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
}

void ConnectTests::connect_params2 ()
{
    mConnection = ShpTests::GetConnection ();
    CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
    try
    {
		mConnection->SetConnectionString (L"DefaultFileLocation=C:\\Shp_NoFolder//" L";TemporaryFileLocation=");
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
        FdoConnectionState state = mConnection->Open ();
        CPPUNIT_ASSERT_MESSAGE ("open connection should fail", false);
    }
    catch (FdoException*)
	{
	}
    catch (...)
    {
        CPPUNIT_FAIL ("non-FdoException");
    }
	CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
    mConnection->Close ();
    CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
}

void ConnectTests::connect_params3 ()
{
    mConnection = ShpTests::GetConnection ();
    CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
    try
    {
		mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION L"..//Ontario\\" L";TemporaryFileLocation=");
        CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
        FdoConnectionState state = mConnection->Open ();
		CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Open == mConnection->GetConnectionState ());
		mConnection->Close ();
		CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
    }
    catch (FdoException* ge)
	{
		fail (ge);
	}
    catch (...)
    {
        CPPUNIT_FAIL ("non-FdoException");
    }
	CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
    mConnection->Close ();
    CPPUNIT_ASSERT_MESSAGE ("connection state not closed", FdoConnectionState_Closed == mConnection->GetConnectionState ());
}
