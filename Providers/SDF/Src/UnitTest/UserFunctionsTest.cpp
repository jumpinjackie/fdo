// Copyright (C) 2004-2007  Autodesk, Inc.
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

// This code is provided just for an example. You should delete it from
// the project once you are ready to compile your test suite.  More information
// about using CppUnit can be found on the CppUnit homepage.
// http://cppunit.sourceforge.net/doc/lastest/index.html

// List of all modules can be found here:
// http://cppunit.sourceforge.net/doc/lastest/modules.html

// Below are the macros for all Assertions available with CppUnit. If you want to create your own assertion
// go to: http://cppunit.sourceforge.net/doc/lastest/group___creating_new_assertions.html

/*
CPPUNIT_ASSERT(condition) 
  Assertions that a condition is true. 

CPPUNIT_ASSERT_MESSAGE(message, condition) 
  Assertion with a user specified message. 

CPPUNIT_FAIL(message) 
  Fails with the specified message. 

CPPUNIT_ASSERT_EQUAL(expected, actual) 
  Asserts that two values are equals. 
 
CPPUNIT_ASSERT_EQUAL_MESSAGE(message, expected, actual) 
  Asserts that two values are equals, provides additional messafe on failure. 
 
CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, delta) 
  Macro for primitive value comparisons. 
 
CPPUNIT_ASSERT_THROW(expression, ExceptionType) 
  Asserts that the given expression throws an exception of the specified type. 
 
CPPUNIT_ASSERT_NO_THROW(expression) 
  Asserts that the given expression does not throw any exceptions. 
 
CPPUNIT_ASSERT_ASSERTION_FAIL(assertion)   CPPUNIT_ASSERT_THROW( assertion, CPPUNIT_NS::Exception ) 
  Asserts that an assertion fail. 
 
CPPUNIT_ASSERT_ASSERTION_PASS(assertion)   CPPUNIT_ASSERT_NO_THROW( assertion ) 
  Asserts that an assertion pass. 
*/

#include "stdafx.h"
#include <ctime>
#include <math.h>
#include "UserFunctionsTest.h"
#include "FdoCommonFile.h"
#include "UnitTestUtil.h"
#include <cppunit/extensions/HelperMacros.h>
#include "FdoExpressionEngine.h"
#include "FdoCommonOSUtil.h"
#include "FdoCommonMiscUtil.h"
#include <FdoExpressionEngineIAggregateFunction.h>
#include <FdoExpressionEngineINonAggregateFunction.h>
#include <FdoExpressionEngineFunctionCollection.h>


CPPUNIT_TEST_SUITE_REGISTRATION(UserFunctionsTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( UserFunctionsTest, "UserFunctionsTest");


UserFunctionsTest::UserFunctionsTest()
{

}


UserFunctionsTest::~UserFunctionsTest()
{
}


// This method is executed at the beginning of each test case.
// Use it to initialize any variables or objects that you will
// need for multiple test cases.
void UserFunctionsTest::setUp()
{

}

// This method is executed at the end of each test case.  It's
// purpose is to deallocate anything that was allocated in the
// setUp() method.
void UserFunctionsTest::tearDown()
{
}

class UserFdoFunctionConcat : public FdoExpressionEngineINonAggregateFunction
	
{
public:

	static UserFdoFunctionConcat* Create() {return new UserFdoFunctionConcat();};

	virtual FdoFunctionDefinition* GetFunctionDefinition()
	{

		if (!func)
		{
			FdoString *desc = NULL;
			FdoPtr<FdoArgumentDefinition>strParm;
			FdoPtr<FdoArgumentDefinitionCollection>strParms = FdoArgumentDefinitionCollection::Create();
		 
			desc = L"Argument that represents a string";
			strParm = FdoArgumentDefinition::Create(L"strValue1", desc, FdoDataType_String);
			strParms->Add(strParm);
			strParm = FdoArgumentDefinition::Create(L"strValue2", desc, FdoDataType_String);
			strParms->Add(strParm);
			desc = L"UserConcat";
			FdoPtr<FdoSignatureDefinition> concatSignatureDef = FdoSignatureDefinition::Create(FdoDataType_String, strParms);
			FdoPtr<FdoSignatureDefinitionCollection> concatSignatureDefCol = FdoSignatureDefinitionCollection::Create();
			concatSignatureDefCol->Add(concatSignatureDef);
			func = FdoFunctionDefinition::Create(L"UserConcat", desc, false, concatSignatureDefCol);
		}

		return FDO_SAFE_ADDREF(func);
	}



	virtual FdoLiteralValue* Evaluate(FdoLiteralValueCollection *literalValues)
	{
		wchar_t res[1000];
		FdoPtr<FdoStringValue> string1 = (FdoStringValue *) literalValues->GetItem(0);
		FdoPtr<FdoStringValue> string2 = (FdoStringValue *) literalValues->GetItem(1);
		wcscpy (res, string1->GetString());
		wcscat (res, string2->GetString());
		string->SetString(res);
        string->AddRef();
		return string;

	}

	virtual UserFdoFunctionConcat* CreateObject() { return new UserFdoFunctionConcat();};
private:
	FdoFunctionDefinition* func;
	FdoStringValue *string;
	UserFdoFunctionConcat()
	{
		string = FdoStringValue::Create();
		func = NULL;
	};

	~UserFdoFunctionConcat()
	{
		string->Release();
		FDO_SAFE_RELEASE(func);
	}

	virtual void Dispose()
    {
        delete this;
    }
};




class FdoFunctionMax2 : public FdoExpressionEngineIAggregateFunction
	
{
public:

	static FdoFunctionMax2* Create() {return new FdoFunctionMax2();};

	virtual FdoFunctionDefinition* GetFunctionDefinition()
	{

		if (!function)
		{
			FdoString *desc = L"Returns the maximum value of an expression";
			function = FdoCommonMiscUtil::CreateFunctionDefinition(L"UserDefinedMax", desc, true, 7,
			//function = FdoCommonMiscUtil::CreateFunctionDefinition(L"Max", desc, true, 7,
				FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Double, 
				FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Single, 
				FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Decimal, 
				FdoPropertyType_DataProperty, FdoDataType_DateTime, 1, FdoPropertyType_DataProperty, FdoDataType_DateTime,
				FdoPropertyType_DataProperty, FdoDataType_String, 1, FdoPropertyType_DataProperty, FdoDataType_String,
				FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Byte, 
				FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int16, 
				FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int32, 
				FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int64);

		}

		return FDO_SAFE_ADDREF(function);
	}

	virtual void Process(FdoLiteralValueCollection *literalValues)
	{
		FdoPtr<FdoInt32Value> int32 = (FdoInt32Value *) literalValues->GetItem(0);
		if (int32->GetInt32() > m_Max)
			m_Max = int32->GetInt32();
	}

	virtual FdoLiteralValue* GetResult()
	{
		return FdoDoubleValue::Create(m_Max);
	}

	virtual FdoFunctionMax2* CreateObject() { return new FdoFunctionMax2();};

private:
	double m_Max;

	FdoFunctionMax2()
	{
		m_Max = -1E100;
		function = NULL;
	};

	~FdoFunctionMax2()
	{
		FDO_SAFE_RELEASE(function);
	}

	FdoFunctionDefinition* function;

	virtual void Dispose()
    {
        delete this;
    }
};

class TestReader: public FdoIReader
{
public:

    TestReader()
    {
        i = 0;
    };

    virtual ~TestReader()
    {
    }


public:

    virtual bool GetBoolean(FdoString* propertyName)
    {
        return true;
    }

    virtual FdoByte GetByte(FdoString* propertyName)
    {
        return 0;
    }

    virtual FdoDateTime GetDateTime(FdoString* propertyName)
    {
        FdoDateTimeValue* dataTime = FdoDateTimeValue::Create();
        dataTime->GetDateTime();
        return dataTime->GetDateTime();
    }

    virtual double GetDouble(FdoString* propertyName)
    {
        return 0;
    }

    virtual FdoInt16 GetInt16(FdoString* propertyName)
    {
        return 0;
    }

    virtual FdoInt32 GetInt32(FdoString* propertyName)
    {
        return i;
    }
    
    virtual FdoInt64 GetInt64(FdoString* propertyName)
    {
        return 0;
    }

    virtual float GetSingle(FdoString* propertyName)
    {
        return 0;
    }

    virtual FdoString* GetString(FdoString* propertyName)
    {
        static wchar_t temp[32];
        swprintf(temp, 32, L"%d", i);


        return temp;
    }

    virtual FdoLOBValue* GetLOB(FdoString* propertyName)
    {
        return NULL;
    }

    FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName )
    {
        return NULL;
    }

    bool IsNull(FdoString* propertyName)
    {
        return false;
    }
    
    virtual FdoByteArray* GetGeometry(FdoString* propertyName)
    {
        return NULL;
    }

    virtual FdoIRaster* GetRaster(FdoString* propertyName)
    {
        return NULL;
    }

    virtual bool ReadNext()
    {
        i++;
        return i < 10;
    }

    virtual void Close()
    {
        
    }

    virtual void Dispose()
    {
        delete this;
    }

private:
    int i;
};


void UserFunctionsTest::selectUserDefinedNonAggregatesTest()
{
    try
    {
        
        FdoPtr<FdoIReader> testReader = new TestReader();

        FdoPtr<FdoExpressionEngineFunctionCollection> userDefinedFunctions = FdoExpressionEngineFunctionCollection::Create();
        userDefinedFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionMax2::Create()));
        userDefinedFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(UserFdoFunctionConcat::Create()));
        FdoPtr<FdoClass> classDef = FdoClass::Create(L"TestClass", L"test class");
        FdoPtr<FdoPropertyDefinitionCollection> properties = classDef->GetProperties();
		FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(L"Name", L"The name of the object");
		dpd->SetDataType(FdoDataType_String);
		properties->Add(dpd);

		dpd = FdoDataPropertyDefinition::Create(L"ID", L"The id");
		dpd->SetDataType(FdoDataType_Int32);
		dpd->SetIsAutoGenerated(false);
		properties->Add(dpd);

        FdoPtr<FdoExpressionEngine> engine = FdoExpressionEngine::Create(testReader, classDef, userDefinedFunctions);

        FdoPtr<FdoExpression> expression = FdoExpression::Parse(L"UserConcat(Name, UserConcat('a', Name))");

        wchar_t buffer[32];
        int i=1;
        while (testReader->ReadNext())
        {
            FdoPtr<FdoLiteralValue> value = engine->Evaluate(expression);
            FdoDataValue* dataValue = static_cast<FdoDataValue *>(value.p);
            FdoStringValue* stringValue = static_cast<FdoStringValue *>(dataValue);
            printf("value = %ls\n", stringValue->GetString());
            swprintf(buffer, 32, L"%da%d", i, i);
            i++;
            CPPUNIT_ASSERT(wcscmp(stringValue->GetString(), buffer) == 0);
        } 
    }
    catch ( FdoException* ex ) {
        TestCommonFail( ex );
    }
}


void UserFunctionsTest::selectUserDefinedAggregatesTest()
{
    try
    {
        
        FdoPtr<FdoIReader> testReader = new TestReader();

        FdoPtr<FdoExpressionEngineFunctionCollection> userDefinedFunctions = FdoExpressionEngineFunctionCollection::Create();
        userDefinedFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(FdoFunctionMax2::Create()));
        userDefinedFunctions->Add(FdoPtr<FdoExpressionEngineIFunction>(UserFdoFunctionConcat::Create()));
        FdoPtr<FdoClass> classDef = FdoClass::Create(L"TestClass", L"test class");
        FdoPtr<FdoPropertyDefinitionCollection> properties = classDef->GetProperties();
		FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(L"Name", L"The name of the object");
		dpd->SetDataType(FdoDataType_String);
		properties->Add(dpd);

		dpd = FdoDataPropertyDefinition::Create(L"ID", L"The id");
		dpd->SetDataType(FdoDataType_Int32);
		dpd->SetIsAutoGenerated(false);
		properties->Add(dpd);

        FdoPtr<FdoExpressionEngine> engine = FdoExpressionEngine::Create(testReader, classDef, userDefinedFunctions);

       FdoPtr<FdoExpression> expression = FdoExpression::Parse(L"UserDefinedMax(ID)+UserDefinedMax(ID)");
//        FdoPtr<FdoExpression> expression = FdoExpression::Parse(L"Max(ID)+Max(ID)");

        int i=1;
        FdoPtr<FdoLiteralValue> value = engine->Evaluate(expression);
        FdoDataValue* dataValue = static_cast<FdoDataValue *>(value.p);
        FdoDoubleValue* doubleValue = static_cast<FdoDoubleValue *>(dataValue);
        printf("value = %f\n", doubleValue->GetDouble());
    }
    catch ( FdoException* ex ) {
        TestCommonFail( ex );
    }
}
