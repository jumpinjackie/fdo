// Copyright (C) 2004-2009  Autodesk, Inc.
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
#include "SQLCmdTest.h"
#include "UnitTestUtil.h"
#include <ctime>
#include <cppunit/extensions/HelperMacros.h>
#include "FdoCommonFile.h"
#ifndef _WIN32
#include <unistd.h>
#endif
static const wchar_t* SC_TEST_FILE = L"../../TestData/SQLCmdTest.sqlite";
static const wchar_t* SOURCE_FILE = L"../../TestData/PARCEL_Source.sqlite";

CPPUNIT_TEST_SUITE_REGISTRATION( SQLCmdTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SQLCmdTest, "SQLCmdTest");

SQLCmdTest::SQLCmdTest(void)
{
    
}

SQLCmdTest::~SQLCmdTest(void)
{
    
}

void SQLCmdTest::setUp ()
{

}

void SQLCmdTest::tearDown ()
{
}

void SQLCmdTest::TestSimpleBind ()
{
	try
	{
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateConnection();

		UnitTestUtil::OpenConnection( SC_TEST_FILE, true, true, conn );
		 //apply schema
		FdoPtr<FdoIApplySchema> applyschema = static_cast<FdoIApplySchema*>(conn->CreateCommand(FdoCommandType_ApplySchema));
        FdoPtr<FdoFeatureSchema> schema = UnitTestUtil::CreateSLTSchema(FdoGeometryType_Point);
		applyschema->SetFeatureSchema(schema);
		applyschema->Execute();
        conn->Close();
        conn->Open();

        FdoPtr<FdoISQLCommand> sqlCmd = static_cast<FdoISQLCommand*>(conn->CreateCommand(FdoCommandType_SQLCommand));
        sqlCmd->SetSQLStatement(L"insert into parcelchild (Name,Url,Key,Numb) values (?,?,?,?)");

        FdoPtr<FdoParameterValueCollection> parmVals = sqlCmd->GetParameterValues();

        FdoPtr<FdoStringValue>name = FdoStringValue::Create(L"Fred");
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"Name", name )));

        FdoPtr<FdoStringValue>url = FdoStringValue::Create(L"http://www.youtube.com/watch?v=zX53PVe8Rck");
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"Url", url)));

        FdoPtr<FdoStringValue>key = FdoStringValue::Create(L"Key1");
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"Key", key)));

        FdoPtr<FdoDoubleValue>dblVal = FdoDoubleValue::Create(234.234);
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"Numb", dblVal)));

        CPPUNIT_ASSERT(sqlCmd->ExecuteNonQuery() == 1 );

        name->SetString(L"Wilma");
        key->SetString(L"Key2");
        dblVal->SetDouble(1234.005);
        CPPUNIT_ASSERT(sqlCmd->ExecuteNonQuery() == 1 );

        name->SetString(L"Barney");
        key->SetString(L"Key3");
        dblVal->SetDouble(2345.005);
        CPPUNIT_ASSERT(sqlCmd->ExecuteNonQuery() == 1 );

        name->SetString(L"Betty");
        key->SetString(L"Key4");
        dblVal->SetDouble(4325.009);
        CPPUNIT_ASSERT(sqlCmd->ExecuteNonQuery() == 1 );               
	}
	catch(FdoException *exp )
	{
		UnitTestUtil::PrintException( exp, stdout, false);
        FDO_SAFE_RELEASE(exp);
        CPPUNIT_FAIL("TestBindBasicTypes failed");
	}
}

void SQLCmdTest::TestErrorMessageSimpleBind ()
{
	try
	{
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateConnection();

		UnitTestUtil::OpenConnection( SC_TEST_FILE, true, true, conn );
		 //apply schema
		FdoPtr<FdoIApplySchema> applyschema = static_cast<FdoIApplySchema*>(conn->CreateCommand(FdoCommandType_ApplySchema));
        FdoPtr<FdoFeatureSchema> schema = UnitTestUtil::CreateSLTSchema(FdoGeometryType_Point);
		applyschema->SetFeatureSchema(schema);
		applyschema->Execute();
        conn->Close();
        conn->Open();

        FdoPtr<FdoISQLCommand> sqlCmd = static_cast<FdoISQLCommand*>(conn->CreateCommand(FdoCommandType_SQLCommand));
        sqlCmd->SetSQLStatement(L"insert into parcelchild2 (Name,Url,Key,Numb) values (?,?,?,?)");

        FdoPtr<FdoParameterValueCollection> parmVals = sqlCmd->GetParameterValues();

        FdoPtr<FdoStringValue>name = FdoStringValue::Create(L"Fred");
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"Name", name )));

        FdoPtr<FdoStringValue>url = FdoStringValue::Create(L"http://www.youtube.com/watch?v=zX53PVe8Rck");
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"Url", url)));

        FdoPtr<FdoStringValue>key = FdoStringValue::Create(L"Key1");
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"Key", key)));

        FdoPtr<FdoDoubleValue>dblVal = FdoDoubleValue::Create(234.234);
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"Numb", dblVal)));

        CPPUNIT_ASSERT(sqlCmd->ExecuteNonQuery() == 1 );

        CPPUNIT_FAIL("TestErrorMessageSimpleBind: Expected exception not found, test should fail!");
	}
	catch(FdoException *exp )
	{
        FdoStringP expErr = L"no such table: parcelchild2";
        CPPUNIT_ASSERT(expErr == exp->GetExceptionMessage());
        printf("\nExpected exception:\n");
		UnitTestUtil::PrintException( exp, stdout, false);
        FDO_SAFE_RELEASE(exp);
	}
}

void SQLCmdTest::TestUpdateWithBind ()
{
	try
	{
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateConnection();

		UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false, conn );
		 

        FdoPtr<FdoISQLCommand> sqlCmd = static_cast<FdoISQLCommand*>(conn->CreateCommand(FdoCommandType_SQLCommand));
        sqlCmd->SetSQLStatement(L"update parcelchild set key=? where Name=?");

        FdoPtr<FdoParameterValueCollection> parmVals = sqlCmd->GetParameterValues();      

        FdoPtr<FdoStringValue>key = FdoStringValue::Create(L"Key99");
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"Key", key)));

        FdoPtr<FdoStringValue>name = FdoStringValue::Create(L"Barney");
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"Name", name )));

        CPPUNIT_ASSERT(sqlCmd->ExecuteNonQuery() == 1 );

	}
	catch(FdoException *exp )
	{
		UnitTestUtil::PrintException( exp, stdout, false);
        FDO_SAFE_RELEASE(exp);
        CPPUNIT_FAIL("TestBindBasicTypes failed");
	}
}

void SQLCmdTest::TestDeleteWithBind ()
{
	try
	{
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateConnection();

		UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false, conn );
		 

        FdoPtr<FdoISQLCommand> sqlCmd = static_cast<FdoISQLCommand*>(conn->CreateCommand(FdoCommandType_SQLCommand));
        sqlCmd->SetSQLStatement(L"delete from parcelchild where key=?");

        FdoPtr<FdoParameterValueCollection> parmVals = sqlCmd->GetParameterValues();      

        FdoPtr<FdoStringValue>key = FdoStringValue::Create(L"Key2");
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"Key", key)));

        CPPUNIT_ASSERT(sqlCmd->ExecuteNonQuery() == 1 );

	}
	catch(FdoException *exp )
	{
		UnitTestUtil::PrintException( exp, stdout, false);
        FDO_SAFE_RELEASE(exp);
        CPPUNIT_FAIL("TestBindBasicTypes failed");
	}
}

void SQLCmdTest::TestSelectWithBind ()
{
	try
	{
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateConnection();

		UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false, conn );		 

        FdoPtr<FdoISQLCommand> sqlCmd = static_cast<FdoISQLCommand*>(conn->CreateCommand(FdoCommandType_SQLCommand));
        sqlCmd->SetSQLStatement(L"select f.Name from parcelchild f where f.Key=:parm1 and f.id>=:parm2");

        FdoPtr<FdoParameterValueCollection> parmVals = sqlCmd->GetParameterValues();

        FdoPtr<FdoStringValue>key = FdoStringValue::Create(L"Key1");
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"parm1", key)));
        FdoPtr<FdoInt32Value>id = FdoInt32Value::Create(1);
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"parm2", id)));

        FdoPtr<FdoISQLDataReader> reader = sqlCmd->ExecuteReader();
        CPPUNIT_ASSERT(reader->ReadNext() );
        CPPUNIT_ASSERT(wcscmp(reader->GetString(L"Name"),L"Fred") == 0 );

        key->SetString(L"Key2");
        reader = sqlCmd->ExecuteReader();
        CPPUNIT_ASSERT( ! reader->ReadNext() );

        key->SetString(L"Key99");
        reader = sqlCmd->ExecuteReader();
        CPPUNIT_ASSERT( reader->ReadNext() );
        CPPUNIT_ASSERT(wcscmp(reader->GetString(L"Name"),L"Barney") == 0 );

        key->SetString(L"Key4");
        reader = sqlCmd->ExecuteReader();
        CPPUNIT_ASSERT(reader->ReadNext() );
        CPPUNIT_ASSERT(wcscmp(reader->GetString(L"Name"),L"Betty") == 0 );

	}
	catch(FdoException *exp )
	{
		UnitTestUtil::PrintException( exp, stdout, false);
        FDO_SAFE_RELEASE(exp);
        CPPUNIT_FAIL("TestBindBasicTypes failed");
	}
}

void SQLCmdTest::TestExpressionWithBind ()
{
	try
	{
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateConnection();

		UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false, conn );		 

        FdoPtr<FdoISQLCommand> sqlCmd = static_cast<FdoISQLCommand*>(conn->CreateCommand(FdoCommandType_SQLCommand));
        sqlCmd->SetSQLStatement(L"select (max(f.id))+1,null,Name from parcelchild f where f.Key=:key and f.id>=:id");

        FdoPtr<FdoParameterValueCollection> parmVals = sqlCmd->GetParameterValues();

        FdoPtr<FdoStringValue>key = FdoStringValue::Create(L"Key1");
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"Key", key)));
        FdoPtr<FdoInt32Value>id = FdoInt32Value::Create(1);
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"id", id)));

        FdoPtr<FdoISQLDataReader> reader = sqlCmd->ExecuteReader();
        CPPUNIT_ASSERT(reader->ReadNext() );
        CPPUNIT_ASSERT(reader->GetColumnType(0)==FdoDataType_Int64);
        CPPUNIT_ASSERT(reader->GetString(1)==NULL);
        CPPUNIT_ASSERT(wcscmp(reader->GetString(2),L"Fred")==0);

        int idxColName = reader->GetColumnIndex(L"Name");
        FdoStringP nameCol = reader->GetColumnName(idxColName);
        CPPUNIT_ASSERT(wcscmp(nameCol,L"Name")==0);
	}
	catch(FdoException *exp )
	{
		UnitTestUtil::PrintException( exp, stdout, false);
        FDO_SAFE_RELEASE(exp);
        CPPUNIT_FAIL("TestExpressionWithBind failed");
	}
}


void SQLCmdTest::TestComplexBind ()
{
	try
	{
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateConnection();

		UnitTestUtil::OpenConnection( SC_TEST_FILE, true, true, conn );
		 //apply schema
		FdoPtr<FdoIApplySchema> applyschema = static_cast<FdoIApplySchema*>(conn->CreateCommand(FdoCommandType_ApplySchema));
        FdoPtr<FdoFeatureSchema> schema = UnitTestUtil::CreateSLTSchema(FdoGeometryType_Point);
		applyschema->SetFeatureSchema(schema);
		applyschema->Execute();
        conn->Close();
        conn->Open();

        FdoPtr<FdoISQLCommand> sqlCmd = static_cast<FdoISQLCommand*>(conn->CreateCommand(FdoCommandType_SQLCommand));

        sqlCmd->SetSQLStatement(L"insert into parcelchild (Name,Url,Key,Numb,Data,Data2,datetime) values (?,?,?,?,?,?,?)");

        FdoPtr<FdoParameterValueCollection> parmVals = sqlCmd->GetParameterValues();

        FdoPtr<FdoStringValue>name = FdoStringValue::Create();
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"Name", name )));

        FdoPtr<FdoStringValue>url = FdoStringValue::Create();
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"Url", url)));

        FdoPtr<FdoStringValue>key = FdoStringValue::Create();
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"Key", key)));

        FdoPtr<FdoDoubleValue>dblVal = FdoDoubleValue::Create();
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"Numb", dblVal)));

        FdoPtr<FdoGeometryValue> gvgeom = FdoGeometryValue::Create(NULL);
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"Data", gvgeom)));

        FdoPtr<FdoGeometryValue> gvgeom2 = FdoGeometryValue::Create(NULL);
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"Data2", gvgeom2)));

        FdoDateTime dt(2006,4,21,19,40,(float)10.0001);
	    FdoPtr<FdoDateTimeValue> dtv = FdoDateTimeValue::Create(dt);
        parmVals->Add(FdoPtr<FdoParameterValue>(FdoParameterValue::Create(L"datetime", dtv)));

        FdoPtr<FdoIConnection> shpConn = UnitTestUtil::OpenConnection( SOURCE_FILE, false);
	    FdoPtr<FdoISelect> select = (FdoISelect*)shpConn->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"DaKlass");
	    FdoPtr<FdoIFeatureReader> shpReader = select->Execute();

        clock_t start;
        clock_t finish;
		start = clock ();

        char tmp[32];
	    tmp[0] = '\0';

	    int numbProp = 1;

        FdoPtr<FdoITransaction> trans = conn->BeginTransaction();

	    while( shpReader->ReadNext() )
	    {
		    if( ! shpReader->IsNull(L"Data") )
		    {
			    gvgeom->SetGeometry( FdoPtr<FdoByteArray>( shpReader->GetGeometry(L"Data") ) );
			    gvgeom2->SetGeometry( FdoPtr<FdoByteArray>( shpReader->GetGeometry(L"Data") ) ); 
		    }
		    else
		    {
			    gvgeom->SetGeometry( NULL );
			    gvgeom2->SetGeometry( NULL );
		    }
		    if( ! shpReader->IsNull(L"Name") )
			    name->SetString( shpReader->GetString(L"Name") );
		    else
			    name->SetString( L"" );

		    if( ! shpReader->IsNull(L"Key") )
			    key->SetString( shpReader->GetString(L"Key") );
		    else
			    key->SetString( L"" );

		    if( ! shpReader->IsNull(L"Url") )
			    url->SetString( shpReader->GetString(L"Url") );
		    else
			    url->SetString( L"" );

		    dt.minute = numbProp%60;
		    dtv->SetDateTime( dt );	    

            dblVal->SetDouble(numbProp*0.00009);

            numbProp++;

            CPPUNIT_ASSERT(sqlCmd->ExecuteNonQuery() == 1 );
            if ( (numbProp % 1000) == 0 )
            {
                for(unsigned int i=0;i<strlen(tmp);i++)putchar(8);
                sprintf(tmp,"Count = %d",numbProp);
                printf(tmp);
            }
        }
        trans->Commit();

        for(unsigned int i=0;i<strlen(tmp);i++)putchar(8);
		sprintf(tmp,"Count = %d",numbProp);
		printf(tmp);
		finish = clock ();
		printf ("  time: %2.3f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
	}
	catch(FdoException *exp )
	{
		UnitTestUtil::PrintException( exp, stdout, false);
        FDO_SAFE_RELEASE(exp);
        CPPUNIT_FAIL("TestComplexBind failed");
	}
}

void SQLCmdTest::TestSelectWithTrans()
{

    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SOURCE_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
        FdoPtr<FdoISQLCommand> selectCmd = (FdoISQLCommand*)conn->CreateCommand(FdoCommandType_SQLCommand); 
        selectCmd->SetSQLStatement(L"select (max(FeatId)+1) from DaKlass");
        
        FdoPtr<FdoISQLDataReader> rdr = selectCmd->ExecuteReader();
        FdoIDataReader* reader = dynamic_cast<FdoIDataReader*>(rdr.p);

        if (reader == NULL)
        {
            // This should not happen but it’s happening on RH 5
            printf( "Known issue: dynamic_cast to FdoIDataReader* failed\n" );
            return;
        }
        
        if ( reader->ReadNext() )
        {
            FdoString* prop = reader->GetPropertyName(0);
            FdoPropertyType tp = rdr->GetPropertyType(prop);
		}
        // added just to force dispose transaction
        if (reader)
        {
            FdoPtr<FdoITransaction> tr1 = conn->BeginTransaction();
            // Rollback will not work is a SQLite limitation which needs to be avoided
            // Rollback with readers opened and transaction opened during opened reader will not work
            tr1->Commit();
        }

        // test if we can open a new transaction
        FdoPtr<FdoITransaction> tr2 = conn->BeginTransaction();
    }
    catch ( FdoException* e )
	{
		TestCommonFail( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		throw;
	}
   	catch (...)
   	{
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
	printf( "Done\n" );
}

void SQLCmdTest::TestSelectWithIdtNames()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SOURCE_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );

        FdoPtr<FdoISQLCommand> sqlCmd = static_cast<FdoISQLCommand*>(conn->CreateCommand(FdoCommandType_SQLCommand));
        sqlCmd->SetSQLStatement(L"select FeatId ID, Name Name1, 'test' Name1, FeatId*33, FeatId*33, FeatId*33 from DaKlass;");

        FdoPtr<FdoISQLDataReader> rdr = sqlCmd->ExecuteReader();
        bool val = rdr->ReadNext();
        FdoPropertyType pt = rdr->GetPropertyType(L"ID");
        pt = rdr->GetPropertyType(L"Name1");
        pt = rdr->GetPropertyType(L"Name1$1");
        pt = rdr->GetPropertyType(L"FeatId*33");
        pt = rdr->GetPropertyType(L"FeatId*33$1");
        pt = rdr->GetPropertyType(L"FeatId*33$2");
	}
	catch(FdoException *exp )
	{
		UnitTestUtil::PrintException( exp, stdout, false);
        FDO_SAFE_RELEASE(exp);
        CPPUNIT_FAIL("TestBindBasicTypes failed");
	}
}
