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

#include "Pch.h"
#include "ExpressionTest.h"
#ifdef _DEBUG
//#define DEBUG_DETAIL	1
#endif
#include "TestProcessor.h"

CPPUNIT_TEST_SUITE_REGISTRATION (ExpressionTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (ExpressionTest, "ExpressionTest");

ExpressionTest::ExpressionTest(void)
{
}

ExpressionTest::~ExpressionTest(void)
{
}

void ExpressionTest::setUp()
{
    // nothing to do at setup time... yet
}

void ExpressionTest::testExpressionTree()
{
    // create a tree of Expression Classes
    FdoDataValue*           pDVal = FdoStringValue::Create(L"abc");
    FdoFunction*            pFunc = FdoFunction::Create(L"FunctionName", NULL, 0);
    FdoBinaryExpression*    pExprMult = FdoBinaryExpression::Create(pDVal, FdoBinaryOperations_Multiply, pFunc);

    FdoIdentifier*          pIdent = FdoIdentifier::Create(L"this.is.a.test");
    FdoParameter*           pParam = FdoParameter::Create(L"AndSoIsThis");
    FdoBinaryExpression*    pExprDiv = FdoBinaryExpression::Create(pIdent, FdoBinaryOperations_Divide, pParam);

    FdoBinaryExpression*    pExprAdd = FdoBinaryExpression::Create(pExprDiv, FdoBinaryOperations_Add, pExprMult);
    
	FdoByteArray*			pFGF = FdoByteArray::Create((FdoByte*)"SomeGeometryData", 17);
    FdoGeometryValue*       pGVal = FdoGeometryValue::Create(pFGF);
    FdoBinaryExpression*    pExprSub = FdoBinaryExpression::Create(pGVal, FdoBinaryOperations_Subtract, pExprAdd);

    FdoUnaryExpression*     pExprNeg = FdoUnaryExpression::Create(FdoUnaryOperations_Negate, pExprSub);

    // test ToString
#ifdef _WIN32
    _putws(pExprNeg->ToString());
#else
    printf("%ls\n", pExprNeg->ToString());
#endif

    // test Processor
    UnitTestProcessor    testProc;
    pExprNeg->Process(&testProc);

    // release Expression classes
    pIdent->Release();
    pExprMult->Release();
    pExprDiv->Release();
    pExprAdd->Release();
    pExprSub->Release();
    pExprNeg->Release();
    pParam->Release();
    pFunc->Release();
    pDVal->Release();
    pGVal->Release();
    pFGF->Release();
}

void ExpressionTest::testBinaryExpression()
{
    // create some data for the test.
    FdoDataValue*           pDVal = FdoStringValue::Create(L"abc");
    FdoParameter*           pParam = FdoParameter::Create(L"AndSoIsThis");

    // test the constructor
    FdoBinaryExpression*    pBExpr = FdoBinaryExpression::Create(pDVal, FdoBinaryOperations_Multiply, pParam);
    FdoExpression*  pExpr = pBExpr->GetLeftExpression();
    FDO_CPPUNIT_ASSERT(pExpr == pDVal);
    pExpr->Release();

    FDO_CPPUNIT_ASSERT(pBExpr->GetOperation() == FdoBinaryOperations_Multiply);

    pExpr = pBExpr->GetRightExpression();
    FDO_CPPUNIT_ASSERT(pExpr == pParam);
    pExpr->Release();

    // test the setters and getters with null values
    pBExpr->SetLeftExpression(NULL);
    pBExpr->SetRightExpression(NULL);
    FDO_CPPUNIT_ASSERT(pBExpr->GetLeftExpression() == NULL);
    FDO_CPPUNIT_ASSERT(pBExpr->GetRightExpression() == NULL);

    // test the setters and getters
    pBExpr->SetLeftExpression(pParam);
    pBExpr->SetOperation(FdoBinaryOperations_Add);
    pBExpr->SetRightExpression(pDVal);
    pExpr = pBExpr->GetLeftExpression();
    FDO_CPPUNIT_ASSERT(pExpr == pParam);
    pExpr->Release();
    FDO_CPPUNIT_ASSERT(pBExpr->GetOperation() == FdoBinaryOperations_Add);
    pExpr = pBExpr->GetRightExpression();
    FDO_CPPUNIT_ASSERT(pExpr == pDVal);
    pExpr->Release();

    // release everything
    pDVal->Release();
    pParam->Release();
    pBExpr->Release();
}

void ExpressionTest::testDataValue()
{
    // test the default constructor
    FdoBooleanValue*    pDVal1 = FdoBooleanValue::Create();
    FDO_CPPUNIT_ASSERT(pDVal1->IsNull());
    FdoString*  pStr = pDVal1->ToString();
    FDO_CPPUNIT_ASSERT(wcscmp(pStr, L"NULL") == 0);
    pDVal1->SetBoolean(true);
    FDO_CPPUNIT_ASSERT(!pDVal1->IsNull());
    FDO_CPPUNIT_ASSERT(pDVal1->GetDataType() == FdoDataType_Boolean);
    FDO_CPPUNIT_ASSERT(pDVal1->GetBoolean() == true);
    pDVal1->Release();

    FdoByteValue*    pDVal2 = FdoByteValue::Create();
    FDO_CPPUNIT_ASSERT(pDVal2->IsNull());
    pStr = pDVal2->ToString();
    FDO_CPPUNIT_ASSERT(wcscmp(pStr, L"NULL") == 0);
    pDVal2->SetByte(0x03);
    FDO_CPPUNIT_ASSERT(!pDVal2->IsNull());
    FDO_CPPUNIT_ASSERT(pDVal2->GetDataType() == FdoDataType_Byte);
    FDO_CPPUNIT_ASSERT(pDVal2->GetByte() == 0x03);
    pDVal2->Release();

    FdoDateTimeValue*    pDVal3 = FdoDateTimeValue::Create();
    FDO_CPPUNIT_ASSERT(pDVal3->IsNull());
    pStr = pDVal3->ToString();
    FDO_CPPUNIT_ASSERT(wcscmp(pStr, L"NULL") == 0);
    FdoDateTime timeVal(1962, 1, 15, 17, 11, 0.0);
    pDVal3->SetDateTime(timeVal);
    FDO_CPPUNIT_ASSERT(!pDVal3->IsNull());
    FDO_CPPUNIT_ASSERT(pDVal3->GetDataType() == FdoDataType_DateTime);
    FdoDateTime timeVal1 = pDVal3->GetDateTime();
    FDO_CPPUNIT_ASSERT(timeVal1.year == timeVal.year);
    FDO_CPPUNIT_ASSERT(timeVal1.month == timeVal.month);
    FDO_CPPUNIT_ASSERT(timeVal1.day == timeVal.day);
    FDO_CPPUNIT_ASSERT(timeVal1.hour == timeVal.hour);
    FDO_CPPUNIT_ASSERT(timeVal1.minute == timeVal.minute);
    FDO_CPPUNIT_ASSERT(timeVal1.seconds == timeVal.seconds);
    pDVal3->ToString();
    pDVal3->Release();

    FdoDecimalValue*    pDVal4 = FdoDecimalValue::Create();
    FDO_CPPUNIT_ASSERT(pDVal4->IsNull());
    pStr = pDVal4->ToString();
    FDO_CPPUNIT_ASSERT(wcscmp(pStr, L"NULL") == 0);
    pDVal4->SetDecimal(3.14);
    FDO_CPPUNIT_ASSERT(!pDVal4->IsNull());
    FDO_CPPUNIT_ASSERT(pDVal4->GetDataType() == FdoDataType_Decimal);
    FDO_CPPUNIT_ASSERT(pDVal4->GetDecimal() == 3.14);
    pDVal4->Release();

    FdoDoubleValue*    pDVal5 = FdoDoubleValue::Create();
    FDO_CPPUNIT_ASSERT(pDVal5->IsNull());
    pStr = pDVal5->ToString();
    FDO_CPPUNIT_ASSERT(wcscmp(pStr, L"NULL") == 0);
    pDVal5->SetDouble(2.234);
    FDO_CPPUNIT_ASSERT(!pDVal5->IsNull());
    FDO_CPPUNIT_ASSERT(pDVal5->GetDataType() == FdoDataType_Double);
    FDO_CPPUNIT_ASSERT(pDVal5->GetDouble() == 2.234);
    pDVal5->Release();

    FdoInt16Value*    pDVal6 = FdoInt16Value::Create();
    FDO_CPPUNIT_ASSERT(pDVal6->IsNull());
    pStr = pDVal6->ToString();
    FDO_CPPUNIT_ASSERT(wcscmp(pStr, L"NULL") == 0);
    pDVal6->SetInt16(32700);
    FDO_CPPUNIT_ASSERT(!pDVal6->IsNull());
    FDO_CPPUNIT_ASSERT(pDVal6->GetDataType() == FdoDataType_Int16);
    FDO_CPPUNIT_ASSERT(pDVal6->GetInt16() == 32700);
    pDVal6->Release();

    FdoInt32Value*    pDVal7 = FdoInt32Value::Create();
    FDO_CPPUNIT_ASSERT(pDVal7->IsNull());
    pStr = pDVal7->ToString();
    FDO_CPPUNIT_ASSERT(wcscmp(pStr, L"NULL") == 0);
    pDVal7->SetInt32(93000000);
    FDO_CPPUNIT_ASSERT(!pDVal7->IsNull());
    FDO_CPPUNIT_ASSERT(pDVal7->GetDataType() == FdoDataType_Int32);
    FDO_CPPUNIT_ASSERT(pDVal7->GetInt32() == 93000000);
    pDVal7->Release();

    FdoInt64Value*    pDVal8 = FdoInt64Value::Create();
    FDO_CPPUNIT_ASSERT(pDVal8->IsNull());
    pStr = pDVal8->ToString();
    FDO_CPPUNIT_ASSERT(wcscmp(pStr, L"NULL") == 0);

    pDVal8->SetInt64(12345678901234LL);
    FDO_CPPUNIT_ASSERT(!pDVal8->IsNull());
    FDO_CPPUNIT_ASSERT(pDVal8->GetDataType() == FdoDataType_Int64);
    FDO_CPPUNIT_ASSERT(pDVal8->GetInt64() == 12345678901234LL);
    pDVal8->Release();

    FdoSingleValue*    pDVal9 = FdoSingleValue::Create();
    FDO_CPPUNIT_ASSERT(pDVal9->IsNull());
    pStr = pDVal9->ToString();
    FDO_CPPUNIT_ASSERT(wcscmp(pStr, L"NULL") == 0);
    pDVal9->SetSingle(1.21f);
    FDO_CPPUNIT_ASSERT(!pDVal9->IsNull());
    FDO_CPPUNIT_ASSERT(pDVal9->GetDataType() == FdoDataType_Single);
    FDO_CPPUNIT_ASSERT(pDVal9->GetSingle() == 1.21f);
    pDVal9->Release();

    FdoStringValue*    pDVal10 = FdoStringValue::Create();
    FDO_CPPUNIT_ASSERT(pDVal10->IsNull());
    pStr = pDVal10->ToString();
    FDO_CPPUNIT_ASSERT(wcscmp(pStr, L"NULL") == 0);   // this was throwing an exception.  Make sure it returns a string.
    pDVal10->SetString(L"TestValue");
    FDO_CPPUNIT_ASSERT(!pDVal10->IsNull());
    FDO_CPPUNIT_ASSERT(pDVal10->GetDataType() == FdoDataType_String);
    FDO_CPPUNIT_ASSERT(wcscmp(pDVal10->GetString(), L"TestValue") == 0);
    pDVal10->Release();

    FdoBLOBValue*    pDVal11 = FdoBLOBValue::Create();
    FDO_CPPUNIT_ASSERT(pDVal11->IsNull());
    pStr = pDVal11->ToString();
	FDO_CPPUNIT_ASSERT(wcscmp(pStr, L"{NULL}") == 0);
	FdoByteArray*    pByteSet = FdoByteArray::Create((FdoByte*)"123456", 7);
    pDVal11->SetData(pByteSet);
    FDO_CPPUNIT_ASSERT(!pDVal11->IsNull());
    FDO_CPPUNIT_ASSERT(pDVal11->GetDataType() == FdoDataType_BLOB);
    FdoByteArray*    pByteGet = pDVal11->GetData();
    FdoInt32    iLen = pByteGet->GetCount();
    FDO_CPPUNIT_ASSERT(iLen == 7);
    FDO_CPPUNIT_ASSERT(memcmp(pByteGet->GetData(), "123456", iLen) == 0);
	pByteGet->Release();
	pByteSet->Release();
    pDVal11->Release();

    FdoCLOBValue*    pDVal12 = FdoCLOBValue::Create();
    FDO_CPPUNIT_ASSERT(pDVal12->IsNull());
    pStr = pDVal12->ToString();
	FDO_CPPUNIT_ASSERT(wcscmp(pStr, L"{NULL}") == 0);
	pByteSet = FdoByteArray::Create((FdoByte*)"98765", 6);
    pDVal12->SetData(pByteSet);
    FDO_CPPUNIT_ASSERT(!pDVal12->IsNull());
    FDO_CPPUNIT_ASSERT(pDVal12->GetDataType() == FdoDataType_CLOB);
    pByteGet = pDVal12->GetData();
    iLen = pByteGet->GetCount();
    FDO_CPPUNIT_ASSERT(iLen == 6);
    FDO_CPPUNIT_ASSERT(memcmp(pByteGet->GetData(), "98765", iLen) == 0);
	pByteSet->Release();
	pByteGet->Release();
    pDVal12->Release();

    // test the setters and getters for a null value
    FdoStringValue*    pDVal13 = FdoStringValue::Create();
    FDO_CPPUNIT_ASSERT(pDVal13->IsNull());
    pStr = pDVal13->ToString();
    FDO_CPPUNIT_ASSERT(wcscmp(pStr, L"NULL") == 0);
    pDVal13->SetNull();
    pStr = pDVal13->ToString();
    FDO_CPPUNIT_ASSERT(wcscmp(pStr, L"NULL") == 0);
    FDO_CPPUNIT_ASSERT(pDVal13->IsNull());
    FDO_CPPUNIT_ASSERT(pDVal13->GetDataType() == FdoDataType_String);
    pDVal13->Release();
}

void ExpressionTest::testFunction()
{
    FdoExpression*  apExp[3];

    // create some data for the test.
    apExp[0] = FdoParameter::Create(L"TestParam");
    apExp[1] = FdoIdentifier::Create(L"TestIdent");
    apExp[2] = FdoDoubleValue::Create(1.234);

    // test the constructor
    FdoFunction*    pFunc = FdoFunction::Create(L"Name", apExp, 3);
    FDO_CPPUNIT_ASSERT(wcscmp(pFunc->GetName(), L"Name") == 0);
    FdoExpressionCollection*    pColl = pFunc->GetArguments();
    FDO_CPPUNIT_ASSERT(pColl->GetCount() == 3);
    pColl->Release();

    // test the setters and getters
    pFunc->SetName(L"Name2");
    FDO_CPPUNIT_ASSERT(wcscmp(pFunc->GetName(), L"Name2") == 0);

    // release everything
    apExp[0]->Release();
    apExp[1]->Release();
    apExp[2]->Release();
    pFunc->Release();
}

void ExpressionTest::testGeometryValue()
{
    // test the constructor
	FdoByteArray*		pFGF1 = FdoByteArray::Create((FdoByte*)"testing123", 11);
    FdoGeometryValue*   pGeom = FdoGeometryValue::Create(pFGF1);

    FdoByteArray*   pByte = pGeom->GetGeometry();
	FdoInt32		iLen = pByte->GetCount();

    FDO_CPPUNIT_ASSERT(iLen == 11);
    FDO_CPPUNIT_ASSERT(strcmp((const char*)pByte->GetData(), "testing123") == 0);
    pByte->Release();
    FDO_CPPUNIT_ASSERT(!pGeom->IsNull());

    // test the setters and getters with null values
    pGeom->SetNullValue();
    FDO_CPPUNIT_ASSERT(pGeom->IsNull());

    // test the setters and getters
	FdoByteArray*	pFGF2 = FdoByteArray::Create((FdoByte*)"Test456", 8);
    pGeom->SetGeometry(pFGF2);

    pByte = pGeom->GetGeometry();
    FDO_CPPUNIT_ASSERT(pByte->GetCount() == 8);
    FDO_CPPUNIT_ASSERT(strcmp((const char*)pByte->GetData(), "Test456") == 0);
    pByte->Release();
    FDO_CPPUNIT_ASSERT(!pGeom->IsNull());

    // release everything
	pFGF1->Release();
	pFGF2->Release();
    pGeom->Release();
}

void ExpressionTest::testIdentifier()
{
    // test the constructor
    FdoIdentifier*  pIdent = FdoIdentifier::Create(L"This.Is.A.Test");
    FDO_CPPUNIT_ASSERT(wcscmp(pIdent->GetName(), L"Test") == 0);
    FdoInt32    iLen;
    FdoString**   apScope = pIdent->GetScope(iLen);
    FDO_CPPUNIT_ASSERT(iLen == 3);
    FDO_CPPUNIT_ASSERT(wcscmp(apScope[0], L"This") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(apScope[1], L"Is") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(apScope[2], L"A") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(pIdent->GetText(), L"This.Is.A.Test") == 0);

    // test the setters and getters
    pIdent->SetText(L"Testing");
    FDO_CPPUNIT_ASSERT(wcscmp(pIdent->GetName(), L"Testing") == 0);
    apScope = pIdent->GetScope(iLen);
    FDO_CPPUNIT_ASSERT(iLen == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(pIdent->GetText(), L"Testing") == 0);

    // test the setters and getters with null values
    pIdent->SetText(NULL);
    FDO_CPPUNIT_ASSERT(pIdent->GetName() == NULL);
    FDO_CPPUNIT_ASSERT(pIdent->GetScope(iLen) == NULL);
    FDO_CPPUNIT_ASSERT(iLen == 0);
    FDO_CPPUNIT_ASSERT(pIdent->GetText() == NULL);

    // release everything
    pIdent->Release();

    // Test the schema part
    pIdent = FdoIdentifier::Create(L"Electric:Pole.Make.Id");
    FDO_CPPUNIT_ASSERT(wcscmp(pIdent->GetSchemaName(), L"Electric") == 0);
    pIdent->SetText(L"Pole.Make.Id");
    FDO_CPPUNIT_ASSERT(pIdent->GetSchemaName()[0] == '\0');

    pIdent->SetText(L"Telco:Cable.Make.Id");
    FDO_CPPUNIT_ASSERT(wcscmp(pIdent->GetSchemaName(), L"Telco") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(pIdent->GetName(), L"Id") == 0);
    apScope = pIdent->GetScope(iLen);
    FDO_CPPUNIT_ASSERT(iLen == 2);
    FDO_CPPUNIT_ASSERT(wcscmp(apScope[0], L"Cable") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(apScope[1], L"Make") == 0);

    pIdent->SetText(L"Telco:Cable");
    FDO_CPPUNIT_ASSERT(wcscmp(pIdent->GetSchemaName(), L"Telco") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(pIdent->GetName(), L"Cable") == 0);

    pIdent->SetText(NULL);
    FDO_CPPUNIT_ASSERT(pIdent->GetSchemaName()[0] == '\0');

    pIdent->Release();
}

void ExpressionTest::testComputedIdentifier()
{
    FdoPtr<FdoIdentifier>pWidth = FdoIdentifier::Create(L"Width");
    FdoPtr<FdoIdentifier>pLength = FdoIdentifier::Create(L"Length");

    FdoPtr<FdoBinaryExpression>pArea = FdoBinaryExpression::Create(pWidth, FdoBinaryOperations_Multiply, pLength);

    FdoPtr<FdoComputedIdentifier>pCompIdent = FdoComputedIdentifier::Create(L"Area", pArea);
    FDO_CPPUNIT_ASSERT(wcscmp(pCompIdent->GetName(), L"Area") == 0);
    FDO_CPPUNIT_ASSERT(wcscmp(pCompIdent->ToString(), L"( Width*Length ) AS Area") == 0 );
}

void ExpressionTest::testParameter()
{
    // test the constructor
    FdoParameter*   pParam = FdoParameter::Create(L"ParamTest");
    FDO_CPPUNIT_ASSERT(wcscmp(pParam->GetName(), L"ParamTest") == 0);

    // test the setters and getters with null values
    pParam->SetName(NULL);
    FDO_CPPUNIT_ASSERT(pParam->GetName() == NULL);

    // test the setters and getters
    pParam->SetName(L"TestAgain");
    FDO_CPPUNIT_ASSERT(wcscmp(pParam->GetName(), L"TestAgain") == 0);

    // release everything
    pParam->Release();
}

void ExpressionTest::testUnaryExpression()
{
    // create some data for the test.
    FdoDataValue*       pDVal1 = FdoBooleanValue::Create(true);
    FdoDataValue*       pDVal2 = FdoInt16Value::Create(123);

    // test the constructor
    FdoUnaryExpression* pExpr = FdoUnaryExpression::Create(FdoUnaryOperations_Negate, pDVal1);
    FDO_CPPUNIT_ASSERT(pExpr->GetOperation() == FdoUnaryOperations_Negate);
    FdoExpression*  pExprTest = pExpr->GetExpression();
    FDO_CPPUNIT_ASSERT(pExprTest == pDVal1);
    pExprTest->Release();

    // test the setters and getters with nulls
    pExpr->SetExpression(NULL);
    FDO_CPPUNIT_ASSERT(pExpr->GetExpression() == NULL);

    // test the setters and getters
    pExpr->SetExpression(pDVal2);
    pExprTest = pExpr->GetExpression();
    FDO_CPPUNIT_ASSERT(pExprTest == pDVal2);
    pExprTest->Release();

    // release everything
    pExpr->Release();
    pDVal1->Release();
    pDVal2->Release();
}


