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
#include "InsertTest.h"
#include "UnitTestUtil.h"
#include <ctime>
#include <cppunit/extensions/HelperMacros.h>
#ifdef _WIN32
static const wchar_t* SC_TEST_FILE = L"..\\..\\TestData\\InsertTest.sqlite";
#else
#include <unistd.h>
static const wchar_t* SC_TEST_FILE = L"../../TestData/InsertTest.sqlite";
#endif


CPPUNIT_TEST_SUITE_REGISTRATION( InsertTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( InsertTest, "InsertTest");

InsertTest::InsertTest(void)
{
    
}

InsertTest::~InsertTest(void)
{
    
}

void InsertTest::setUp ()
{

}

void InsertTest::tearDown ()
{
}

void InsertTest::TestInsInvalidGeomTypes ()
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

        UnitTestUtil::CreateData(false, conn, 100);
        conn->Close();
        conn->Open();

        FdoPtr<FdoIDescribeSchema> descschema = static_cast<FdoIDescribeSchema*>(conn->CreateCommand(FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schColl = descschema->Execute();
        CPPUNIT_ASSERT(schColl->GetCount() != 0);
        FdoPtr<FdoFeatureSchema> sch = schColl->GetItem(0);
        FdoPtr<FdoClassCollection> classColl = sch->GetClasses();
        FdoPtr<FdoClassDefinition> clsDef = classColl->GetItem(L"Parcel");
        CPPUNIT_ASSERT(clsDef->GetClassType() == FdoClassType_FeatureClass);
        FdoFeatureClass* fclsDef = static_cast<FdoFeatureClass*>(clsDef.p);
        FdoPtr<FdoGeometricPropertyDefinition> geomProp = fclsDef->GetGeometryProperty();
        CPPUNIT_ASSERT(geomProp->GetGeometryTypes() == FdoGeometricType_Point);

        FdoInt32 length = 0;
        FdoGeometryType* types = geomProp->GetSpecificGeometryTypes(length);
        CPPUNIT_ASSERT(length == 1);
        CPPUNIT_ASSERT(*types == FdoGeometryType_Point);        
	}
	catch(FdoException *exp )
	{
		UnitTestUtil::PrintException( exp, stdout, false);
        FDO_SAFE_RELEASE(exp);
        CPPUNIT_FAIL("Insert failed");
	}
}

bool InsertTest::TestForDateValue(FdoIConnection* conn, FdoString* clsName, FdoString* propName, FdoInt32 id, FdoDateTime* expVal)
{
	FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	select->SetFeatureClassName(clsName);
    FdoStringP filterTxt = FdoStringP::Format(L"FeatId=%d", id);
    FdoPtr<FdoFilter> filter = FdoFilter::Parse(filterTxt);
    select->SetFilter(filter);
	FdoPtr<FdoIFeatureReader> rdr = select->Execute();
	if (rdr->ReadNext())
	{
        if (rdr->IsNull(propName))
            return (expVal == NULL);

        if (expVal == NULL)
            return false;

        FdoDateTime dt = rdr->GetDateTime(propName);
        return (dt.day == expVal->day) && (dt.hour == expVal->hour) && (dt.minute == expVal->minute) && (dt.month == expVal->month)
            && (dt.seconds == expVal->seconds) && (dt.year == expVal->year);
	}
    return false;
}

void InsertTest::TestConstraints1WithOption (FdoBoolean inMemConn)
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (inMemConn)
            conn = UnitTestUtil::OpenMemoryConnection();
        else
		    conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, true, true);
		 
        //apply schema
		FdoPtr<FdoIApplySchema> applyschema = static_cast<FdoIApplySchema*>(conn->CreateCommand(FdoCommandType_ApplySchema));
        FdoPtr<FdoFeatureSchemaCollection> schColl = FdoFeatureSchemaCollection::Create(NULL);
        schColl->ReadXml(L"SchConstraintsTest1.xml");
        CPPUNIT_ASSERT(schColl->GetCount() == 1);
        
        FdoPtr<FdoFeatureSchema> schema = schColl->GetItem(0);
		applyschema->SetFeatureSchema(schema);
		applyschema->Execute();

        FdoPtr<FdoIInsert> insCmd = static_cast<FdoIInsert*>(conn->CreateCommand(FdoCommandType_Insert));
        FdoPtr<FdoPropertyValueCollection> vals = insCmd->GetPropertyValues();
        FdoPtr<FdoPropertyValue> propIns;
        
		FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
		double coords[] = { 7.2068, 43.7556, 
							7.2088, 43.7556, 
							7.2088, 43.7574, 
							7.2068, 43.7574, 
							7.2068, 43.7556 }; 
		FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, 10, coords);
		FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);
		FdoPtr<FdoByteArray> polyfgf = gf->GetFgf(poly);
		FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(polyfgf);

        FdoPtr<FdoPropertyValue> propGeomIns = FdoPropertyValue::Create(L"Geometry", gv);
        vals->Add(propGeomIns);
        bool excRec = false;
        
        // Range test
        try
        {
            // check the default value
            insCmd->SetFeatureClassName(L"TestRange");
            vals->Clear();
            vals->Add(propGeomIns);
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            
            FdoDateTime expVal(2009, 7, 7, 0, 0, 0);
            CPPUNIT_ASSERT(TestForDateValue(conn, L"TestRange", L"PropDT", rdr->GetInt32(L"FeatId"), &expVal));
            rdr->Close();
        }
        catch(FdoException* exc)
        {
            UnitTestUtil::PrintException(exc);
            exc->Release();
            CPPUNIT_FAIL("\nUnexpected exception: ");
        }

        try
        {
            // try insert invalid date
            FdoDateTime dt(2005, 1, 22, 0, 0, 0.0);
            FdoPtr<FdoDateTimeValue> dtValue = FdoDateTimeValue::Create(dt);
            propIns = FdoPropertyValue::Create(L"PropDT", dtValue);
            vals->Clear();
            vals->Add(propIns);
            vals->Add(propGeomIns);
            
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            rdr->Close();
        }
        catch(FdoException* exc)
        {
            // expected exception
            printf( "\nExpected exception: " );
            UnitTestUtil::PrintException(exc);
            exc->Release();
            excRec = true;
        }
        if (!excRec)
        {
            CPPUNIT_FAIL ("Expected exception not found");
        }
        try
        {
            // try insert valid date
            FdoDateTime dt(2009, 7, 9, 0, 0, 0.0);
            FdoPtr<FdoDateTimeValue> dtValue = FdoDateTimeValue::Create(dt);
            propIns = FdoPropertyValue::Create(L"PropDT", dtValue);
            vals->Clear();
            vals->Add(propIns);
            vals->Add(propGeomIns);
            
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());

            CPPUNIT_ASSERT(TestForDateValue(conn, L"TestRange", L"PropDT", rdr->GetInt32(L"FeatId"), &dt));
            rdr->Close();
        }
        catch(FdoException* exc)
        {
            UnitTestUtil::PrintException(exc);
            exc->Release();
            CPPUNIT_FAIL("\nUnexpected exception: ");
        }

        // List test
        excRec = false;
        try
        {
            // check the default value
            insCmd->SetFeatureClassName(L"TestList");
            vals->Clear();
            vals->Add(propGeomIns);
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());

            FdoDateTime expVal(2009, 7, 3, 0, 0, 0);
            CPPUNIT_ASSERT(TestForDateValue(conn, L"TestList", L"PropDT", rdr->GetInt32(L"FeatId"), &expVal));
            rdr->Close();
        }
        catch(FdoException* exc)
        {
            UnitTestUtil::PrintException(exc);
            exc->Release();
            CPPUNIT_FAIL("\nUnexpected exception: ");
        }

        try
        {
            // try insert invalid date
            FdoDateTime dt(2005, 1, 22, 0, 0, 0.0);
            FdoPtr<FdoDateTimeValue> dtValue = FdoDateTimeValue::Create(dt);
            propIns = FdoPropertyValue::Create(L"PropDT", dtValue);
            vals->Clear();
            vals->Add(propIns);
            vals->Add(propGeomIns);
            
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            rdr->Close();
        }
        catch(FdoException* exc)
        {
            // expected exception
            printf( "\nExpected exception: " );
            UnitTestUtil::PrintException(exc);
            exc->Release();
            excRec = true;
        }
        if (!excRec)
        {
            CPPUNIT_FAIL ("Expected exception not found");
        }
        try
        {
            // try insert valid date
            FdoDateTime dt(2009, 7, 7, 0, 0, 0.0);
            FdoPtr<FdoDateTimeValue> dtValue = FdoDateTimeValue::Create(dt);
            propIns = FdoPropertyValue::Create(L"PropDT", dtValue);
            vals->Clear();
            vals->Add(propIns);
            vals->Add(propGeomIns);
            
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            CPPUNIT_ASSERT(TestForDateValue(conn, L"TestList", L"PropDT", rdr->GetInt32(L"FeatId"), &dt));
            rdr->Close();
        }
        catch(FdoException* exc)
        {
            UnitTestUtil::PrintException(exc);
            exc->Release();
            CPPUNIT_FAIL("\nUnexpected exception: ");
        }
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

void InsertTest::TestConstraints1 ()
{
    TestConstraints1WithOption ();
}

void InsertTest::TestConstraints2 ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
		conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, true, true);
		 
        //apply schema
		FdoPtr<FdoIApplySchema> applyschema = static_cast<FdoIApplySchema*>(conn->CreateCommand(FdoCommandType_ApplySchema));
        FdoPtr<FdoFeatureSchemaCollection> schColl = FdoFeatureSchemaCollection::Create(NULL);
        schColl->ReadXml(L"SchConstraintsTest2.xml");
        CPPUNIT_ASSERT(schColl->GetCount() == 1);
        
        FdoPtr<FdoFeatureSchema> schema = schColl->GetItem(0);
		applyschema->SetFeatureSchema(schema);
		applyschema->Execute();

        FdoPtr<FdoIInsert> insCmd = static_cast<FdoIInsert*>(conn->CreateCommand(FdoCommandType_Insert));
        FdoPtr<FdoPropertyValueCollection> vals = insCmd->GetPropertyValues();
        FdoPtr<FdoPropertyValue> propIns;
        
		FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
		double coords[] = { 7.2068, 43.7556, 
							7.2088, 43.7556, 
							7.2088, 43.7574, 
							7.2068, 43.7574, 
							7.2068, 43.7556 }; 
		FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, 10, coords);
		FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);
		FdoPtr<FdoByteArray> polyfgf = gf->GetFgf(poly);
		FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(polyfgf);

        FdoPtr<FdoPropertyValue> propGeomIns = FdoPropertyValue::Create(L"Geometry", gv);
        vals->Add(propGeomIns);
        bool excRec = false;
        
        // Range test
        try
        {
            // check the default value
            insCmd->SetFeatureClassName(L"TestRange");
            vals->Clear();
            vals->Add(propGeomIns);
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            
            CPPUNIT_ASSERT(TestForDateValue(conn, L"TestRange", L"PropDT", rdr->GetInt32(L"FeatId"), NULL));
            rdr->Close();
        }
        catch(FdoException* exc)
        {
            UnitTestUtil::PrintException(exc);
            exc->Release();
            CPPUNIT_FAIL("\nUnexpected exception: ");
        }

        try
        {
            // try insert invalid date
            FdoDateTime dt(2010, 1, 22, 0, 0, 0.0);
            FdoPtr<FdoDateTimeValue> dtValue = FdoDateTimeValue::Create(dt);
            propIns = FdoPropertyValue::Create(L"PropDT", dtValue);
            vals->Clear();
            vals->Add(propIns);
            vals->Add(propGeomIns);
            
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            rdr->Close();
        }
        catch(FdoException* exc)
        {
            // expected exception
            printf( "\nExpected exception: " );
            UnitTestUtil::PrintException(exc);
            exc->Release();
            excRec = true;
        }
        if (!excRec)
        {
            CPPUNIT_FAIL ("Expected exception not found");
        }
        try
        {
            // try insert valid date
            FdoDateTime dt(2005, 7, 9, 0, 0, 0.0);
            FdoPtr<FdoDateTimeValue> dtValue = FdoDateTimeValue::Create(dt);
            propIns = FdoPropertyValue::Create(L"PropDT", dtValue);
            vals->Clear();
            vals->Add(propIns);
            vals->Add(propGeomIns);
            
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());

            CPPUNIT_ASSERT(TestForDateValue(conn, L"TestRange", L"PropDT", rdr->GetInt32(L"FeatId"), &dt));
            rdr->Close();
        }
        catch(FdoException* exc)
        {
            UnitTestUtil::PrintException(exc);
            exc->Release();
            CPPUNIT_FAIL("\nUnexpected exception: ");
        }

        // List test
        excRec = false;
        try
        {
            // check the default value
            insCmd->SetFeatureClassName(L"TestList");
            vals->Clear();
            vals->Add(propGeomIns);
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());

            CPPUNIT_ASSERT(TestForDateValue(conn, L"TestList", L"PropDT", rdr->GetInt32(L"FeatId"), NULL));
            rdr->Close();
        }
        catch(FdoException* exc)
        {
            UnitTestUtil::PrintException(exc);
            exc->Release();
            CPPUNIT_FAIL("\nUnexpected exception: ");
        }

        try
        {
            // try insert invalid date
            FdoDateTime dt(2005, 1, 22, 0, 0, 0.0);
            FdoPtr<FdoDateTimeValue> dtValue = FdoDateTimeValue::Create(dt);
            propIns = FdoPropertyValue::Create(L"PropDT", dtValue);
            vals->Clear();
            vals->Add(propIns);
            vals->Add(propGeomIns);
            
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            rdr->Close();
        }
        catch(FdoException* exc)
        {
            // expected exception
            printf( "\nExpected exception: " );
            UnitTestUtil::PrintException(exc);
            exc->Release();
            excRec = true;
        }
        if (!excRec)
        {
            CPPUNIT_FAIL ("Expected exception not found");
        }
        try
        {
            // try insert valid date
            FdoDateTime dt(2009, 7, 7, 0, 0, 0.0);
            FdoPtr<FdoDateTimeValue> dtValue = FdoDateTimeValue::Create(dt);
            propIns = FdoPropertyValue::Create(L"PropDT", dtValue);
            vals->Clear();
            vals->Add(propIns);
            vals->Add(propGeomIns);
            
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            CPPUNIT_ASSERT(TestForDateValue(conn, L"TestList", L"PropDT", rdr->GetInt32(L"FeatId"), &dt));
            rdr->Close();
        }
        catch(FdoException* exc)
        {
            UnitTestUtil::PrintException(exc);
            exc->Release();
            CPPUNIT_FAIL("\nUnexpected exception: ");
        }
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

void InsertTest::TestInsertData(FdoIConnection* conn, FdoString* className, FdoInt32 expectedProps, FdoIdentifierCollection* propNames)
{
	FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	select->SetFeatureClassName(className);
    if (propNames != NULL && propNames->GetCount() != 0)
    {
        FdoPtr<FdoIdentifierCollection> selColl = select->GetPropertyNames();
        for(int idx = 0; idx < propNames->GetCount(); idx++)
        {
            FdoPtr<FdoIdentifier> idf = propNames->GetItem(idx);
            selColl->Add(idf);
        }
    }
    FdoPtr<FdoIFeatureReader> rdr = select->Execute();
    CPPUNIT_ASSERT(rdr->ReadNext());
    FdoPtr<FdoClassDefinition> clsrdr = rdr->GetClassDefinition();
    FdoPtr<FdoPropertyDefinitionCollection> rdrProps = clsrdr->GetProperties();
    if (expectedProps != -1)
    {
        CPPUNIT_ASSERT(expectedProps == rdrProps->GetCount());
    }
}

void InsertTest::TestCompPK ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
		conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, true, true);
		 
        //apply schema
		FdoPtr<FdoIApplySchema> applyschema = static_cast<FdoIApplySchema*>(conn->CreateCommand(FdoCommandType_ApplySchema));
        FdoPtr<FdoFeatureSchemaCollection> schColl = FdoFeatureSchemaCollection::Create(NULL);
        schColl->ReadXml(L"SchPKTest.xml");
        CPPUNIT_ASSERT(schColl->GetCount() == 1);
        
        FdoPtr<FdoFeatureSchema> schema = schColl->GetItem(0);
		applyschema->SetFeatureSchema(schema);
		applyschema->Execute();

        FdoPtr<FdoIInsert> insCmd = static_cast<FdoIInsert*>(conn->CreateCommand(FdoCommandType_Insert));
        FdoPtr<FdoPropertyValueCollection> vals = insCmd->GetPropertyValues();
        FdoPtr<FdoPropertyValue> propIns;
        
		FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
		double coords[] = { 7.2068, 43.7556, 
							77.2088, 43.7556, 
							77.2088, 143.7574, 
							7.2068, 143.7574, 
							7.2068, 43.7556 }; 
		FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, 10, coords);
		FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);
		FdoPtr<FdoByteArray> polyfgf = gf->GetFgf(poly);
		FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(polyfgf);

        FdoPtr<FdoPropertyValue> propGeomIns = FdoPropertyValue::Create(L"Geometry", gv);
        // insert test
        insCmd->SetFeatureClassName(L"TestCompPK");
        try
        {
            FdoDateTime dt(2005, 1, 22, 0, 0, 0.0);
            FdoPtr<FdoDateTimeValue> dtValue = FdoDateTimeValue::Create(dt);
            propIns = FdoPropertyValue::Create(L"PropDT", dtValue);
            
            FdoPtr<FdoInt64Value> id64Value = FdoInt64Value::Create(1);
            FdoPtr<FdoPropertyValue> propInsId64 = FdoPropertyValue::Create(L"FeatId", id64Value);
            
            FdoPtr<FdoStringValue> idStrValue = FdoStringValue::Create(L"city");
            FdoPtr<FdoPropertyValue> propInsIdStr = FdoPropertyValue::Create(L"Name", idStrValue);
            
            vals->Clear();
            vals->Add(propIns);
            vals->Add(propInsId64);
            vals->Add(propInsIdStr);
            vals->Add(propGeomIns);
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            FdoPtr<FdoClassDefinition> clsrdr = rdr->GetClassDefinition();
            CPPUNIT_ASSERT(rdr->GetInt64(L"FeatId") == 1);
            CPPUNIT_ASSERT(((FdoStringP)rdr->GetString(L"Name")) == L"city");
            
            rdr->Close();
        }
        catch(FdoException* exc)
        {
            UnitTestUtil::PrintException(exc);
            exc->Release();
            CPPUNIT_FAIL("\nUnexpected exception: ");
        }
        try
        {
            FdoDateTime dt(2005, 1, 22, 0, 0, 0.0);
            FdoPtr<FdoDateTimeValue> dtValue = FdoDateTimeValue::Create(dt);
            propIns = FdoPropertyValue::Create(L"PropDT", dtValue);
            
            FdoPtr<FdoInt64Value> id64Value = FdoInt64Value::Create(1);
            FdoPtr<FdoPropertyValue> propInsId64 = FdoPropertyValue::Create(L"FeatId", id64Value);
            
            FdoPtr<FdoStringValue> idStrValue = FdoStringValue::Create(L"town");
            FdoPtr<FdoPropertyValue> propInsIdStr = FdoPropertyValue::Create(L"Name", idStrValue);
            
            vals->Clear();
            vals->Add(propIns);
            vals->Add(propInsId64);
            vals->Add(propInsIdStr);
            vals->Add(propGeomIns);
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            FdoPtr<FdoClassDefinition> clsrdr = rdr->GetClassDefinition();
            CPPUNIT_ASSERT(rdr->GetInt64(L"FeatId") == 1);
            CPPUNIT_ASSERT(((FdoStringP)rdr->GetString(L"Name")) == L"town");
            
            rdr->Close();
        }
        catch(FdoException* exc)
        {
            UnitTestUtil::PrintException(exc);
            exc->Release();
            CPPUNIT_FAIL("\nUnexpected exception: ");
        }
        
        try
        {
            FdoDateTime dt(2005, 1, 22, 0, 0, 0.0);
            FdoPtr<FdoDateTimeValue> dtValue = FdoDateTimeValue::Create(dt);
            propIns = FdoPropertyValue::Create(L"PropDT", dtValue);
            
            FdoPtr<FdoInt64Value> id64Value = FdoInt64Value::Create(1);
            FdoPtr<FdoPropertyValue> propInsId64 = FdoPropertyValue::Create(L"FeatId", id64Value);
            
            FdoPtr<FdoStringValue> idStrValue = FdoStringValue::Create(L"town");
            FdoPtr<FdoPropertyValue> propInsIdStr = FdoPropertyValue::Create(L"Name", idStrValue);
            
            vals->Clear();
            vals->Add(propIns);
            vals->Add(propInsId64);
            vals->Add(propInsIdStr);
            vals->Add(propGeomIns);
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            FdoPtr<FdoClassDefinition> clsrdr = rdr->GetClassDefinition();
            CPPUNIT_ASSERT(rdr->GetInt64(L"FeatId") == 1);
            CPPUNIT_ASSERT(((FdoStringP)rdr->GetString(L"Name")) == L"town");
            
            rdr->Close();
            CPPUNIT_FAIL ("Expected exception not found");
        }
        catch(FdoException* exc)
        {
            // expected exception
            printf( "\nExpected exception: " );
            UnitTestUtil::PrintException(exc);
            exc->Release();
        }

        FdoPtr<FdoIdentifierCollection> reqColl = FdoIdentifierCollection::Create();
        FdoPtr<FdoIdentifier> idfName = FdoIdentifier::Create(L"Name");
        reqColl->Add(idfName);
        FdoPtr<FdoIdentifier> idfGeom = FdoIdentifier::Create(L"Geometry");
        reqColl->Add(idfGeom);
        TestInsertData(conn, L"TestCompPK", 2, reqColl);

        FdoPtr<FdoIdentifier> idfFeatId = FdoIdentifier::Create(L"FeatId");
        reqColl->Add(idfFeatId);
        TestInsertData(conn, L"TestCompPK", 3, reqColl);

        FdoPtr<FdoExpression> exp = FdoExpression::Parse(L"22.44*FeatId - 33.44");
        FdoPtr<FdoIdentifier> idfCalc = FdoComputedIdentifier::Create(L"Calc", exp);
        reqColl->Add(idfCalc);
        TestInsertData(conn, L"TestCompPK", 4, reqColl);

        reqColl->Remove(idfFeatId);
        TestInsertData(conn, L"TestCompPK", 3, reqColl);

        conn->Close();
        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false );

		FdoPtr<FdoIGetSpatialContexts> gscCmd = (FdoIGetSpatialContexts *)conn->CreateCommand( FdoCommandType_GetSpatialContexts );

        // Get a SC reader
        FdoPtr<FdoISpatialContextReader> reader = gscCmd->Execute();

        // Iterate ...
        while ( reader->ReadNext() )
        {
			FdoPtr<FdoByteArray> ext = reader->GetExtent();
		}
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

void InsertTest::TestNoPK ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
		conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, true, true);
		 
        //apply schema
		FdoPtr<FdoIApplySchema> applyschema = static_cast<FdoIApplySchema*>(conn->CreateCommand(FdoCommandType_ApplySchema));
        FdoPtr<FdoFeatureSchemaCollection> schColl = FdoFeatureSchemaCollection::Create(NULL);
        schColl->ReadXml(L"SchPKTest.xml");
        CPPUNIT_ASSERT(schColl->GetCount() == 1);
        
        FdoPtr<FdoFeatureSchema> schema = schColl->GetItem(0);
		applyschema->SetFeatureSchema(schema);
		applyschema->Execute();

        FdoPtr<FdoIInsert> insCmd = static_cast<FdoIInsert*>(conn->CreateCommand(FdoCommandType_Insert));
        FdoPtr<FdoPropertyValueCollection> vals = insCmd->GetPropertyValues();
        FdoPtr<FdoPropertyValue> propIns;
        
		FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
		double coords[] = { 7.2068, 43.7556, 
							77.2088, 43.7556, 
							77.2088, 143.7574, 
							7.2068, 143.7574, 
							7.2068, 43.7556 }; 
		FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, 10, coords);
		FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);
		FdoPtr<FdoByteArray> polyfgf = gf->GetFgf(poly);
		FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(polyfgf);

        FdoPtr<FdoPropertyValue> propGeomIns = FdoPropertyValue::Create(L"Geometry", gv);
        // insert test
        insCmd->SetFeatureClassName(L"TestNoPK");
        try
        {
            FdoDateTime dt(2005, 1, 22, 0, 0, 0.0);
            FdoPtr<FdoDateTimeValue> dtValue = FdoDateTimeValue::Create(dt);
            propIns = FdoPropertyValue::Create(L"PropDT", dtValue);
            
            FdoPtr<FdoInt64Value> id64Value = FdoInt64Value::Create(1);
            FdoPtr<FdoPropertyValue> propInsId64 = FdoPropertyValue::Create(L"FeatId", id64Value);
            
            FdoPtr<FdoStringValue> idStrValue = FdoStringValue::Create(L"city");
            FdoPtr<FdoPropertyValue> propInsIdStr = FdoPropertyValue::Create(L"Name", idStrValue);
            
            vals->Clear();
            vals->Add(propIns);
            vals->Add(propInsId64);
            vals->Add(propInsIdStr);
            vals->Add(propGeomIns);
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            FdoPtr<FdoClassDefinition> clsrdr = rdr->GetClassDefinition();
            
            rdr->Close();
        }
        catch(FdoException* exc)
        {
            UnitTestUtil::PrintException(exc);
            exc->Release();
            CPPUNIT_FAIL("\nUnexpected exception: ");
        }
        try
        {
            FdoDateTime dt(2005, 1, 22, 0, 0, 0.0);
            FdoPtr<FdoDateTimeValue> dtValue = FdoDateTimeValue::Create(dt);
            propIns = FdoPropertyValue::Create(L"PropDT", dtValue);
            
            FdoPtr<FdoInt64Value> id64Value = FdoInt64Value::Create(1);
            FdoPtr<FdoPropertyValue> propInsId64 = FdoPropertyValue::Create(L"FeatId", id64Value);
            
            FdoPtr<FdoStringValue> idStrValue = FdoStringValue::Create(L"town");
            FdoPtr<FdoPropertyValue> propInsIdStr = FdoPropertyValue::Create(L"Name", idStrValue);
            
            vals->Clear();
            vals->Add(propIns);
            vals->Add(propInsId64);
            vals->Add(propInsIdStr);
            vals->Add(propGeomIns);
            FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            FdoPtr<FdoClassDefinition> clsrdr = rdr->GetClassDefinition();
            
            rdr->Close();
        }
        catch(FdoException* exc)
        {
            UnitTestUtil::PrintException(exc);
            exc->Release();
            CPPUNIT_FAIL("\nUnexpected exception: ");
        }

        // select values
	    FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    select->SetFeatureClassName(L"TestNoPK");
        FdoPtr<FdoIdentifierCollection> collProps = select->GetPropertyNames();
        
        FdoPtr<FdoIdentifier> idfName = FdoIdentifier::Create(L"Name");
        collProps->Add(idfName);
        FdoPtr<FdoIdentifier> idfGeom = FdoIdentifier::Create(L"Geometry");
        collProps->Add(idfGeom);
        FdoPtr<FdoIdentifier> idfFeatId = FdoIdentifier::Create(L"FeatId");
        collProps->Add(idfFeatId);
        FdoPtr<FdoExpression> exp = FdoExpression::Parse(L"22.44*FeatId - 33.44");
        FdoPtr<FdoIdentifier> idfCalc = FdoComputedIdentifier::Create(L"Calc", exp);
        collProps->Add(idfCalc);

		FdoPtr<FdoSpatialCondition> filter = FdoSpatialCondition::Create(L"Geometry", FdoSpatialOperations_EnvelopeIntersects, gv);
        select->SetFilter(filter);
	    FdoPtr<FdoIFeatureReader> rdr = select->Execute();
        int cnt = 0;
	    while (rdr->ReadNext())
	    {
            cnt++;
            if (!rdr->IsNull(L"Name"))
            {
                FdoString* name = rdr->GetString(L"Name");
                printf("Name=%ls\n", name);
            }
            if (!rdr->IsNull(L"FeatId"))
            {
                FdoInt64 featId = rdr->GetInt64(L"FeatId");
                printf("FeatId=%ld\n", featId);
            }
            if (!rdr->IsNull(L"Calc"))
            {
                double calc = rdr->GetDouble(L"Calc");
                printf("Calc=%g\n", calc);
            }
	    }
        CPPUNIT_ASSERT(cnt == 2);

        rdr->Close();
        conn->Close();
        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false );

		FdoPtr<FdoIGetSpatialContexts> gscCmd = (FdoIGetSpatialContexts *)conn->CreateCommand( FdoCommandType_GetSpatialContexts );

        // Get a SC reader
        FdoPtr<FdoISpatialContextReader> reader = gscCmd->Execute();

        // Iterate ...
        while ( reader->ReadNext() )
        {
			FdoPtr<FdoByteArray> ext = reader->GetExtent();
		}
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

FdoString* InsertTest::GetDefaultSchemaName(void)
{
    return L"Default";
}

void InsertTest::CreateConnection( Context& context, FdoBoolean recreateDb )
{
    context.connection = UnitTestUtil::OpenConnection( SC_TEST_FILE, recreateDb, true );
}

bool InsertTest::InsertsDefaultValues()
{
    return false;
}

FdoDataType InsertTest::GetAutoIncrementType()
{
    return FdoDataType_Int32;
}

#define SZ_TXT_BLOB (27*sizeof(wchar_t))
#define VAL_TXT_BLOB L"My Blob String\0Second Value"

void InsertTest::TestInsBLOBTypes()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
		conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, true, true);
		 
        //apply schema
		FdoPtr<FdoIApplySchema> applyschema = static_cast<FdoIApplySchema*>(conn->CreateCommand(FdoCommandType_ApplySchema));
        FdoPtr<FdoFeatureSchemaCollection> schColl = FdoFeatureSchemaCollection::Create(NULL);
        schColl->ReadXml(L"SchConstraintsTest1.xml");
        CPPUNIT_ASSERT(schColl->GetCount() == 1);
        
        FdoPtr<FdoFeatureSchema> schema = schColl->GetItem(0);
        FdoPtr<FdoClassCollection> clsColl = schema->GetClasses();
        FdoPtr<FdoClassDefinition> clsChg = clsColl->GetItem(L"TestRange");
        FdoPtr<FdoPropertyDefinitionCollection> propsToAdd = clsChg->GetProperties();
        FdoPtr<FdoDataPropertyDefinition> blobProp = FdoDataPropertyDefinition::Create(L"BlobTest", L"");
        blobProp->SetDataType(FdoDataType_BLOB);
        propsToAdd->Add(blobProp);

		applyschema->SetFeatureSchema(schema);
		applyschema->Execute();

        {
        FdoPtr<FdoIInsert> insCmd = static_cast<FdoIInsert*>(conn->CreateCommand(FdoCommandType_Insert));
        FdoPtr<FdoPropertyValueCollection> vals = insCmd->GetPropertyValues();
        FdoPtr<FdoPropertyValue> propIns;
        
		FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
		double coords[] = { 7.2068, 43.7556, 
							7.2088, 43.7556, 
							7.2088, 43.7574, 
							7.2068, 43.7574, 
							7.2068, 43.7556 }; 
		FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, 10, coords);
		FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);
		FdoPtr<FdoByteArray> polyfgf = gf->GetFgf(poly);
		FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(polyfgf);

        FdoPtr<FdoPropertyValue> propGeomIns = FdoPropertyValue::Create(L"Geometry", gv);
        vals->Add(propGeomIns);
        
        FdoString* testValBlob = VAL_TXT_BLOB;
        FdoPtr<FdoByteArray> bArray = FdoByteArray::Create((FdoByte*)testValBlob, SZ_TXT_BLOB);
        FdoPtr<FdoBLOBValue> blobv = FdoBLOBValue::Create(bArray);
        FdoPtr<FdoPropertyValue> propBlobIns = FdoPropertyValue::Create(L"BlobTest", blobv);
        vals->Add(propBlobIns);

        insCmd->SetFeatureClassName(L"TestRange");        
        FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
        CPPUNIT_ASSERT(rdr->ReadNext());
        
        FdoDateTime expVal(2009, 7, 7, 0, 0, 0);
        CPPUNIT_ASSERT(TestForDateValue(conn, L"TestRange", L"PropDT", rdr->GetInt32(L"FeatId"), &expVal));
        rdr->Close();
        }

	    FdoPtr<FdoISelect> selectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    selectCmd->SetFeatureClassName(L"TestRange");
        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
        CPPUNIT_ASSERT(reader->ReadNext());

        FdoPtr<FdoLOBValue> bValRet = reader->GetLOB(L"BlobTest");
        if (bValRet)
        {
            FdoPtr<FdoByteArray> arrDataRet = bValRet->GetData();
            CPPUNIT_ASSERT(arrDataRet->GetCount() == SZ_TXT_BLOB);
        }
        else
            CPPUNIT_FAIL("\nInvalid BLOB value [NULL] ");

        FdoPtr<FdoIStreamReader> fdoStream = reader->GetLOBStreamReader(L"BlobTest");
        if (fdoStream.p)
        {
            CPPUNIT_ASSERT(FdoStreamReaderType_Byte == fdoStream->GetType());
            FdoBLOBStreamReader* stream = static_cast<FdoBLOBStreamReader*>(fdoStream.p);
            CPPUNIT_ASSERT(stream->GetLength() == SZ_TXT_BLOB);
            wchar_t buff[21];
            buff[20] = L'\0';
            int sz = stream->ReadNext((FdoByte*)buff, SZ_TXT_BLOB);
            CPPUNIT_ASSERT(sz == SZ_TXT_BLOB);
        }
        else
            CPPUNIT_FAIL("\nInvalid BLOB stream value [NULL] ");

        reader->Close();
    }
    catch(FdoException* exc)
    {
        UnitTestUtil::PrintException(exc);
        exc->Release();
        CPPUNIT_FAIL("\nUnexpected exception: ");
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


void InsertTest::TestConstraints1InMemConn ()
{
    TestConstraints1WithOption (true);
}