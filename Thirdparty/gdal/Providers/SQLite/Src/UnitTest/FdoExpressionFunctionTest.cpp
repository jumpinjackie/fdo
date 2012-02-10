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

#include "stdafx.h"
#include "FdoExpressionFunctionTest.h"
#include "UnitTestUtil.h"
#include <math.h>
#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>
#include <FdoCommonMiscUtil.h>
#include <FdoExpressionEngine.h>
#include "FdoExpressionEngineIAggregateFunction.h"
#include "FdoExpressionEngineINonAggregateFunction.h"
#include "FdoExpressionEngineFunctionCollection.h"

#ifdef _WIN32
static const wchar_t* EXPFCT_TEST_FILE = L"..\\..\\TestData\\ExpFctTest.sqlite";
static const wchar_t* SC_TEST_FILE = L"..\\..\\TestData\\ExpressionsTest.sqlite";
#else
#include <unistd.h>
static const wchar_t* EXPFCT_TEST_FILE = L"../../TestData/ExpFctTest.sqlite";
static const wchar_t* SC_TEST_FILE = L"../../TestData/ExpressionsTest.sqlite";
#endif

CPPUNIT_TEST_SUITE_REGISTRATION(FdoExpressionFunctionTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FdoExpressionFunctionTest, "FdoExpressionFunctionTest");

// ----------------------------------------------------------------------------
// --                          Test Setup Functions                          --
// ----------------------------------------------------------------------------

FdoExpressionFunctionTest::FdoExpressionFunctionTest ()

// +---------------------------------------------------------------------------
// | The function represents the class constructor.
// +---------------------------------------------------------------------------

{

}  //  FdoExpressionFunctionTest ()

FdoExpressionFunctionTest::~FdoExpressionFunctionTest ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{
}  //  ~FdoExpressionFunctionTest ()

void FdoExpressionFunctionTest::Connect ()

// +---------------------------------------------------------------------------
// | The function establishes the connection to the data store used in the
// | executed test suites.
// +---------------------------------------------------------------------------

{

    // Establish the connection.

    m_connection = UnitTestUtil::OpenConnection( EXPFCT_TEST_FILE, true, true );

}  //  Connect ()

void FdoExpressionFunctionTest::RunAllExpFctTests()
{
    bool error_found = false;

    // TODO: Add more function tests or use base RunAllExpFctTests
    // when SQLite provider supports all standard functions.

    // Executing the geometry functions.

    printf("\n");
    printf("\n");
    printf(" >>> ... Testing Geometry Functions \n");
    printf("\n");

    try {

      TestXYZMFunction();
      TestDateStringConv();
      TestConcat();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    if (error_found)
        throw FdoException::Create(L"Unit tests executed with errors");

}

double FdoExpressionFunctionTest::GetNullOrdinate()
{
    return GetNaNOrdinate();
}

void FdoExpressionFunctionTest::TestDateStringConv()
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
            FdoDateTime dt(2005, 1, 22, 8, 5, 1.0);
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
            FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	        select->SetFeatureClassName(L"TestCompPK");
            FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"PropDT IN ( TIMESTAMP '2005-01-22 08:05:01', TIMESTAMP '2009-07-10 22:57:56')");

            select->SetFilter(filter);
            FdoPtr<FdoIdentifierCollection> selColl = select->GetPropertyNames();
            
            FdoPtr<FdoExpression> exp = FdoExpression::Parse(L"ToString ( PropDT, 'DD [Day] Month-YY' )");
            FdoPtr<FdoIdentifier> idfCalc = FdoComputedIdentifier::Create(L"Calc", exp);
            selColl->Add(idfCalc);
            
            FdoPtr<FdoExpression> exp2 = FdoExpression::Parse(L"ToDate ( Calc, 'DD [Day] Month-YY' )");
            FdoPtr<FdoIdentifier> idfCalc2 = FdoComputedIdentifier::Create(L"Calc2", exp2);
            selColl->Add(idfCalc2);
            
            FdoPtr<FdoIFeatureReader> rdr = select->Execute();
            while(rdr->ReadNext())
            {
                if (!rdr->IsNull(L"Calc"))
                {
                    FdoString* val = rdr->GetString(L"Calc");
                    printf ("Calc=%ls\n", val);
                }
                if (!rdr->IsNull(L"Calc2"))
                {
                    FdoDateTime dt = rdr->GetDateTime(L"Calc2");
                    printf ("Calc2[year] = %d\n", dt.year);
                }
            }
            FdoPtr<FdoClassDefinition> clsrdr = rdr->GetClassDefinition();
            FdoPtr<FdoPropertyDefinitionCollection> rdrProps = clsrdr->GetProperties();
            rdr->Close();

            filter = FdoFilter::Parse(L"PropDT >= TIMESTAMP '2004-01-22 08:05:01'");
            select->SetFilter(filter);
            rdr = select->Execute();
            CPPUNIT_ASSERT(rdr->ReadNext());
            rdr->Close();

            filter = FdoFilter::Parse(L"PropDT >= TIMESTAMP '2009-01-22 08:05:01'");
            select->SetFilter(filter);
            rdr = select->Execute();
            CPPUNIT_ASSERT(!rdr->ReadNext());
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

void FdoExpressionFunctionTest::TestConcat()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
		conn = UnitTestUtil::OpenConnection( EXPFCT_TEST_FILE, false, false);
        try
        {
            FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	        select->SetFeatureClassName(L"exfct_c1");

            FdoPtr<FdoIdentifierCollection> selColl = select->GetPropertyNames();
            
            // Make sure TRUE/FALSE is returned 
            FdoPtr<FdoExpression> exp = FdoExpression::Parse(L"Concat(featid, bool_val)");
            FdoPtr<FdoIdentifier> idfCalc = FdoComputedIdentifier::Create(L"Calc", exp);
            selColl->Add(idfCalc);

            // Concat more than 2 values
            exp = FdoExpression::Parse(L"Concat(bool_val, '=', bool_val)");
            idfCalc = FdoComputedIdentifier::Create(L"Calc1", exp);
            selColl->Add(idfCalc);

            FdoPtr<FdoIFeatureReader> rdr = select->Execute();
            while(rdr->ReadNext())
            {
                if (!rdr->IsNull(L"Calc"))
                {
                    FdoStringP val = rdr->GetString(L"Calc");
                    printf ("Calc=%ls\n", (FdoString*)val);

                    CPPUNIT_ASSERT(val.Contains(L"1") || val.Contains(L"0"));
                }

                if (!rdr->IsNull(L"Calc1"))
                {
                    FdoStringP val = rdr->GetString(L"Calc1");
                    printf ("Calc1=%ls\n", (FdoString*)val);
                    CPPUNIT_ASSERT(val == L"1=1" || val == L"0=0");
                }
            }
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

class FdoCustomUserFunctionTest: public FdoExpressionEngineINonAggregateFunction
{
public:
    static FdoCustomUserFunctionTest* Create()
    {
        return new FdoCustomUserFunctionTest();
    }
    static FdoFunctionDefinition* CreateFunctionDefinition()
    {
        FdoPtr<FdoSignatureDefinitionCollection> signatures = FdoSignatureDefinitionCollection::Create();
	    return FdoFunctionDefinition::Create(
									    L"CustomFunction", //NOXLATE
									    L"Test Function.", //NOXLATE
									    false,
									    signatures,
									    FdoFunctionCategoryType_Unspecified,
									    true);
    }
    virtual FdoFunctionDefinition* GetFunctionDefinition()
    {
        if (!m_functionDefinition)
        {
            m_functionDefinition = FdoCustomUserFunctionTest::CreateFunctionDefinition();
        }
        return FDO_SAFE_ADDREF(m_functionDefinition);
    }
    virtual FdoLiteralValue* Evaluate(FdoLiteralValueCollection *literalValues)
    {
        FdoInt32 count = literalValues->GetCount();
        if (count != 0)
        {
            FdoPtr<FdoLiteralValue> input = literalValues->GetItem(0);
            return FdoStringValue::Create(input->ToString());
        }
        else
            return FdoStringValue::Create(L"test");
    }
    virtual FdoCustomUserFunctionTest* CreateObject()
    {
        return new FdoCustomUserFunctionTest();
    }
private:
    FdoCustomUserFunctionTest()
    {
        m_functionDefinition = NULL;
    }
    ~FdoCustomUserFunctionTest()
    {
        FDO_SAFE_RELEASE(m_functionDefinition);
    }
    virtual void Dispose()
    {
        delete this;
    }
	
    FdoFunctionDefinition* m_functionDefinition;
};

void FdoExpressionFunctionTest::TestCustomFunction()
{
    FdoPtr<FdoIConnection> conn;

	conn = UnitTestUtil::OpenConnection( EXPFCT_TEST_FILE, false, false);
    try
    {
        bool expExc = false;
        try
        {
            FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
            select->SetFeatureClassName(L"exfct_c1");
            FdoPtr<FdoIFeatureReader> rdr = select->Execute();

	        FdoPtr<FdoExpressionEngineFunctionCollection> userDefinedFunctions = FdoExpressionEngineFunctionCollection::Create();

	        FdoPtr<FdoExpressionEngineIFunction> function = FdoCustomUserFunctionTest::Create();
	        userDefinedFunctions->Add(function);
            FdoExpressionEngine::RegisterFunctions(userDefinedFunctions);

            FdoPtr<FdoExpression> exp = FdoExpression::Parse(L"CustomFunction(233)");
            FdoPtr<FdoClassDefinition> cls = rdr->GetClassDefinition();
            {
                FdoPtr<FdoExpressionEngine> eng = FdoExpressionEngine::Create(rdr, cls, NULL);

                FdoPtr<FdoLiteralValue> retVal = eng->Evaluate(exp);
                printf ("\n Result = %ls\n", retVal->ToString());
            }
            FdoExpressionEngine::UnRegisterFunctions(userDefinedFunctions);

            expExc = true;
            {
                FdoPtr<FdoExpressionEngine> eng = FdoExpressionEngine::Create(rdr, cls, NULL);

                FdoPtr<FdoLiteralValue> retVal = eng->Evaluate(exp);
                CPPUNIT_FAIL("\nExpected exception missing!");
            }
        }
        catch(FdoException* exc)
        {
            if (!expExc)
            {
                UnitTestUtil::PrintException(exc);
                exc->Release();
                CPPUNIT_FAIL("\nUnexpected exception: ");
            }
            else
                exc->Release();
        }
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
