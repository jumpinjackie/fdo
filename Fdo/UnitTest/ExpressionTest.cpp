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
#include <math.h>
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

void ExpressionTest::testDataValueConvert()
{
/* From Boolean Tests */
    FdoBoolean normBool[] = { true, false };
    FdoString* expNormBool[] = { L"TRUE", L"FALSE" };
    FdoString* expNormBoolNum[] = { L"1", L"0" };
    FdoString* expNormBoolStr[] = { L"'TRUE'", L"'FALSE'" };

    NormalCase<FdoBooleanValue, FdoBoolean>(2, normBool, expNormBool, FdoDataType_Boolean);
    IncompatibleCase<FdoBooleanValue, FdoBoolean>(normBool[0], FdoDataType_DateTime);
    NormalCase<FdoBooleanValue, FdoBoolean>(2, normBool, expNormBoolNum, FdoDataType_Byte);
    NormalCase<FdoBooleanValue, FdoBoolean>(2, normBool, expNormBoolNum, FdoDataType_Decimal);
    NormalCase<FdoBooleanValue, FdoBoolean>(2, normBool, expNormBoolNum, FdoDataType_Double);
    NormalCase<FdoBooleanValue, FdoBoolean>(2, normBool, expNormBoolNum, FdoDataType_Int16);
    NormalCase<FdoBooleanValue, FdoBoolean>(2, normBool, expNormBoolNum, FdoDataType_Int32);
    NormalCase<FdoBooleanValue, FdoBoolean>(2, normBool, expNormBoolNum, FdoDataType_Int64);
    NormalCase<FdoBooleanValue, FdoBoolean>(2, normBool, expNormBoolNum, FdoDataType_Single);
    NormalCase<FdoBooleanValue, FdoBoolean>(2, normBool, expNormBoolStr, FdoDataType_String);
    NullCases<FdoBooleanValue>();

/* From Byte Tests */
    FdoByte normByte[] = { 5, 0, 128, 255, 254 };
    FdoString* expNormByte[] = { L"5", L"0", L"128", L"255", L"254" };
    FdoString* expNormByteStr[] = { L"'5'", L"'0'", L"'128'", L"'255'", L"'254'" };
    FdoByte truncByte[] = { 2, 100, 255 };
    FdoString* expTruncByte[] = { L"TRUE", L"TRUE", L"TRUE" };


    NormalToBoolCase<FdoByteValue, FdoByte>();
    IncompatibleCase<FdoByteValue, FdoByte>(normBool[0], FdoDataType_DateTime);
    NormalCase<FdoByteValue, FdoByte>(5, normByte, expNormByte, FdoDataType_Byte);
    NormalCase<FdoByteValue, FdoByte>(5, normByte, expNormByte, FdoDataType_Decimal);
    NormalCase<FdoByteValue, FdoByte>(5, normByte, expNormByte, FdoDataType_Double);
    NormalCase<FdoByteValue, FdoByte>(5, normByte, expNormByte, FdoDataType_Int16);
    NormalCase<FdoByteValue, FdoByte>(5, normByte, expNormByte, FdoDataType_Int32);
    NormalCase<FdoByteValue, FdoByte>(5, normByte, expNormByte, FdoDataType_Int64);
    NormalCase<FdoByteValue, FdoByte>(5, normByte, expNormByte, FdoDataType_Single);
    NormalCase<FdoByteValue, FdoByte>(5, normByte, expNormByteStr, FdoDataType_String);
    NullCases<FdoByteValue>();
    TruncateCase<FdoByteValue, FdoByte>(3, truncByte, expTruncByte, FdoDataType_Boolean);

/* From DateTime Tests */
    FdoDateTime normDateTime[] = { FdoDateTime(2008, 10, 30, 16, 0, 30) };
    FdoString* expNormDateTime[] = { L"TIMESTAMP '2008-10-30 16:00:30'" };
    FdoString* expNormDateTimeStr[] = { L"'TIMESTAMP ''2008-10-30 16:00:30'''" };

    IncompatibleCase<FdoDateTimeValue, FdoDateTime>(normDateTime[0], FdoDataType_Boolean);
    IncompatibleCase<FdoDateTimeValue, FdoDateTime>(normDateTime[0], FdoDataType_Byte);
    NormalCase<FdoDateTimeValue, FdoDateTime>(1, normDateTime, expNormDateTime, FdoDataType_DateTime);
    IncompatibleCase<FdoDateTimeValue, FdoDateTime>(normDateTime[0], FdoDataType_Decimal);
    IncompatibleCase<FdoDateTimeValue, FdoDateTime>(normDateTime[0], FdoDataType_Double);
    IncompatibleCase<FdoDateTimeValue, FdoDateTime>(normDateTime[0], FdoDataType_Int16);
    IncompatibleCase<FdoDateTimeValue, FdoDateTime>(normDateTime[0], FdoDataType_Int32);
    IncompatibleCase<FdoDateTimeValue, FdoDateTime>(normDateTime[0], FdoDataType_Int64);
    IncompatibleCase<FdoDateTimeValue, FdoDateTime>(normDateTime[0], FdoDataType_Single);
    NormalCase<FdoDateTimeValue, FdoDateTime>(1, normDateTime, expNormDateTimeStr, FdoDataType_String);
    NullCases<FdoDateTimeValue>();

/* From Decimal Tests */

    FdoDouble doubleVals[] = { 1, 0, 128, 255, 254, -1, -32768, 2000, 256, 32767, -32769, 32768, 100000, -2147483648LL, 2147483647, -2147483649LL, 2147483648LL, -1000000000000LL, -pow((FdoDouble)2,(FdoDouble) 63), (pow((FdoDouble)2, (FdoDouble)52) - 1) * pow((FdoDouble)2, (FdoDouble)11), -pow((FdoDouble)2, (FdoDouble)63) - pow((FdoDouble)2, (FdoDouble)11), pow((FdoDouble)2,(FdoDouble) 63), 1.892e101, -1.7e308, 1.7e308 };
    FdoString* expDoubleVals[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767", L"-32769", L"32768", L"100000", L"-2147483648", L"2147483647", L"-2147483649", L"2147483648", L"-1000000000000", L"-9223372036854775808", L"9223372036854773760", L"-9.22337203685478e+018", L"9.22337203685478e+018", L"1.892e101", L"-1.7e308", L"1.7e308" };
    FdoString* expDoubleValsStr[] = { L"'1'", L"'0'", L"'128'", L"'255'", L"'254'", L"'-1'", L"'-32768'", L"'2000'", L"'256'", L"'32767'", L"'-32769'", L"'32768'", L"'100000'", L"'-2147483648'", L"'2147483647'", L"'-2147483649'", L"'2147483648'", L"'-1000000000000'", L"'-9.22337203685478e+018'", L"'9.22337203685477e+018'", L"'-9.22337203685478e+018'", L"'9.22337203685478e+018'", L"'1.892e101'", L"'-1.7e308'", L"'1.7e308'" };
    FdoString* expDoubleValsBool[] = { L"TRUE", L"FALSE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE" };
    FdoString* expDoubleValsByte[] = { L"1", L"0", L"128", L"255", L"254", L"0", L"0", L"255", L"255", L"255", L"0", L"255", L"255", L"0", L"255", L"0", L"255", L"0", L"0", L"255", L"0", L"255", L"255", L"0", L"255" };
    FdoString* expDoubleValsInt16[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767", L"-32768", L"32767", L"32767", L"-32768", L"32767", L"-32768", L"32767", L"-32768", L"-32768", L"32767", L"-32768", L"32767", L"32767", L"-32768", L"32767" };
    FdoString* expDoubleValsInt32[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767", L"-32769", L"32768", L"100000", L"-2147483648", L"2147483647", L"-2147483648", L"2147483647", L"-2147483648", L"-2147483648", L"2147483647", L"-2147483648", L"2147483647", L"2147483647", L"-2147483648", L"2147483647" };
    FdoString* expDoubleValsInt64[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767", L"-32769", L"32768", L"100000", L"-2147483648", L"2147483647", L"-2147483648", L"2147483647", L"-2147483648", L"-2147483648", L"2147483647", L"-9223372036854775808", L"9223372036854775807", L"9223372036854775807", L"-9223372036854775808", L"9223372036854775807" };
    FdoString* expDoubleValsSingle[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767", L"-32769", L"32768", L"100000", L"-2147483648", L"2147483647", L"-2147483648", L"2147483647", L"-2147483648", L"-2147483648", L"2147483647", L"-9223372036854775808", L"9223372036854775807", L"3.4e+038", L"-3.4e+038", L"3.4e+038" };
    FdoDouble doubleFractVals[] = { -5.8, -5.5, -5.3, -0.51, -0.5, -0.1, 0.2, 0.5, 0.51, 100.4, 101.5, 105.9 };
    FdoString* expDoubleFractVals[] = { L"-5.8", L"-5.5", L"-5.3", L"-0.51", L"-0.5", L"-0.1", L"0.2", L"0.5", L"0.51", L"100.4", L"101.5", L"105.9" };
    FdoString* expDoubleRoundVals[] = { L"-6", L"-6", L"-5", L"-1", L"-1", L"0", L"0", L"1", L"1", L"100", L"102", L"106" };
    FdoString* expDoubleFractValsStr[] = { L"'-5.8'", L"'-5.5'", L"'-5.3'", L"'-0.51'", L"'-0.5'", L"'-0.1'", L"'0.2'", L"'0.5'", L"'0.51'", L"'100.4'", L"'101.5'", L"'105.9'" };
    FdoString* expDoubleRoundValsBool[] = { L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE" };
    FdoDouble doubleFractValsByte[] = { -0.5, -0.1, 255.2, 255.5 };
    FdoString* expDoubleFractValsByte[] = { L"0", L"0", L"255", L"255" };
    FdoDouble doubleFractValsInt16[] = { -32768.5, -32768.1, 32767.2, 32767.5 };
    FdoString* expDoubleFractValsInt16[] = { L"-32768", L"-32768", L"32767", L"32767" };
    FdoDouble doubleFractValsInt32[] = { -2147483648.5, -2147483648.1, 2147483647.2, 2147483647.5 };
    FdoString* expDoubleFractValsInt32[] = { L"-2147483648", L"-2147483648", L"2147483647", L"2147483647" };

    NormalToBoolCase<FdoDecimalValue, FdoDouble>();
    NormalCase<FdoDecimalValue, FdoDouble>(5, doubleVals, expDoubleVals, FdoDataType_Byte);
    IncompatibleCase<FdoDecimalValue, FdoDouble>(doubleVals[7], FdoDataType_DateTime);
    NormalCase<FdoDecimalValue, FdoDouble>(25, doubleVals, expDoubleVals, FdoDataType_Decimal);
    NormalCase<FdoDecimalValue, FdoDouble>(25, doubleVals, expDoubleVals, FdoDataType_Double);
    NormalCase<FdoDecimalValue, FdoDouble>(10, doubleVals, expDoubleVals, FdoDataType_Int16);
    NormalCase<FdoDecimalValue, FdoDouble>(15, doubleVals, expDoubleVals, FdoDataType_Int32);
    NormalCase<FdoDecimalValue, FdoDouble>(20, doubleVals, expDoubleVals, FdoDataType_Int64);
    NormalCase<FdoDecimalValue, FdoDouble>(13, doubleVals, expDoubleVals, FdoDataType_Single);
    NormalCase<FdoDecimalValue, FdoDouble>(18, doubleVals, expDoubleValsStr, FdoDataType_String);
    NullCases<FdoDecimalValue>();
    TruncateCase<FdoDecimalValue, FdoDouble>(23, &doubleVals[2], &expDoubleValsBool[2], FdoDataType_Boolean);
    TruncateCase<FdoDecimalValue, FdoDouble>(20, &doubleVals[5], &expDoubleValsByte[5], FdoDataType_Byte);
    TruncateCase<FdoDecimalValue, FdoDouble>(15, &doubleVals[10], &expDoubleValsInt16[10], FdoDataType_Int16);
    TruncateCase<FdoDecimalValue, FdoDouble>(10, &doubleVals[15], &expDoubleValsInt32[15], FdoDataType_Int32);
    TruncateCase<FdoDecimalValue, FdoDouble>(5, &doubleVals[20], &expDoubleValsInt64[20], FdoDataType_Int64);
    TruncateCase<FdoDecimalValue, FdoDouble>(3, &doubleVals[22], &expDoubleValsSingle[22], FdoDataType_Single);
    TruncateCase<FdoDecimalValue, FdoDouble>(12, doubleFractVals, expDoubleRoundValsBool, FdoDataType_Boolean);
    NormalCase<FdoDecimalValue, FdoDouble>(12, doubleFractVals, expDoubleFractVals, FdoDataType_Decimal);
    NormalCase<FdoDecimalValue, FdoDouble>(12, doubleFractVals, expDoubleFractVals, FdoDataType_Double);
    ShiftCase<FdoDecimalValue, FdoDouble>(12, doubleFractVals, expDoubleRoundVals, FdoDataType_Int16);
    ShiftCase<FdoDecimalValue, FdoDouble>(12, doubleFractVals, expDoubleRoundVals, FdoDataType_Int32);
    ShiftCase<FdoDecimalValue, FdoDouble>(12, doubleFractVals, expDoubleRoundVals, FdoDataType_Int64);
    ShiftCase<FdoDecimalValue, FdoDouble>(1, doubleFractVals, expDoubleFractVals, FdoDataType_Single);
    NormalCase<FdoDecimalValue, FdoDouble>(12, doubleFractVals, expDoubleFractValsStr, FdoDataType_String);

/* From Double Tests */

    NormalToBoolCase<FdoDoubleValue, FdoDouble>();
    NormalCase<FdoDoubleValue, FdoDouble>(5, doubleVals, expDoubleVals, FdoDataType_Byte);
    IncompatibleCase<FdoDoubleValue, FdoDouble>(doubleVals[7], FdoDataType_DateTime);
    NormalCase<FdoDoubleValue, FdoDouble>(25, doubleVals, expDoubleVals, FdoDataType_Decimal);
    NormalCase<FdoDoubleValue, FdoDouble>(25, doubleVals, expDoubleVals, FdoDataType_Double);
    NormalCase<FdoDoubleValue, FdoDouble>(10, doubleVals, expDoubleVals, FdoDataType_Int16);
    NormalCase<FdoDoubleValue, FdoDouble>(15, doubleVals, expDoubleVals, FdoDataType_Int32);
    NormalCase<FdoDoubleValue, FdoDouble>(20, doubleVals, expDoubleVals, FdoDataType_Int64);
    NormalCase<FdoDoubleValue, FdoDouble>(13, doubleVals, expDoubleVals, FdoDataType_Single);
    NormalCase<FdoDoubleValue, FdoDouble>(18, doubleVals, expDoubleValsStr, FdoDataType_String);
    NullCases<FdoDoubleValue>();
    TruncateCase<FdoDoubleValue, FdoDouble>(23, &doubleVals[2], &expDoubleValsBool[2], FdoDataType_Boolean);
    TruncateCase<FdoDoubleValue, FdoDouble>(20, &doubleVals[5], &expDoubleValsByte[5], FdoDataType_Byte);
    TruncateCase<FdoDoubleValue, FdoDouble>(15, &doubleVals[10], &expDoubleValsInt16[10], FdoDataType_Int16);
    TruncateCase<FdoDoubleValue, FdoDouble>(10, &doubleVals[15], &expDoubleValsInt32[15], FdoDataType_Int32);
    TruncateCase<FdoDoubleValue, FdoDouble>(5, &doubleVals[20], &expDoubleValsInt64[20], FdoDataType_Int64);
    TruncateCase<FdoDoubleValue, FdoDouble>(23, &doubleVals[2], &expDoubleValsBool[2], FdoDataType_Boolean);
    TruncateCase<FdoDoubleValue, FdoDouble>(20, &doubleVals[5], &expDoubleValsByte[5], FdoDataType_Byte);
    TruncateCase<FdoDoubleValue, FdoDouble>(15, &doubleVals[10], &expDoubleValsInt16[10], FdoDataType_Int16);
    TruncateCase<FdoDoubleValue, FdoDouble>(10, &doubleVals[15], &expDoubleValsInt32[15], FdoDataType_Int32);
    TruncateCase<FdoDoubleValue, FdoDouble>(5, &doubleVals[20], &expDoubleValsInt64[20], FdoDataType_Int64);
    TruncateCase<FdoDoubleValue, FdoDouble>(3, &doubleVals[22], &expDoubleValsSingle[22], FdoDataType_Single);
    TruncateCase<FdoDoubleValue, FdoDouble>(12, doubleFractVals, expDoubleRoundValsBool, FdoDataType_Boolean);
    NormalCase<FdoDoubleValue, FdoDouble>(12, doubleFractVals, expDoubleFractVals, FdoDataType_Decimal);
    NormalCase<FdoDoubleValue, FdoDouble>(12, doubleFractVals, expDoubleFractVals, FdoDataType_Double);
    ShiftCase<FdoDoubleValue, FdoDouble>(12, doubleFractVals, expDoubleRoundVals, FdoDataType_Int16);
    ShiftCase<FdoDoubleValue, FdoDouble>(12, doubleFractVals, expDoubleRoundVals, FdoDataType_Int32);
    ShiftCase<FdoDoubleValue, FdoDouble>(12, doubleFractVals, expDoubleRoundVals, FdoDataType_Int64);
    ShiftCase<FdoDoubleValue, FdoDouble>(1, doubleFractVals, expDoubleFractVals, FdoDataType_Single);
    NormalCase<FdoDoubleValue, FdoDouble>(12, doubleFractVals, expDoubleFractValsStr, FdoDataType_String);
    ShiftCase<FdoDoubleValue, FdoDouble>(4, &doubleVals[18], &expDoubleValsStr[18], FdoDataType_String);

    TruncateCase<FdoDecimalValue, FdoDouble>(4, doubleFractValsByte, expDoubleFractValsByte, FdoDataType_Byte);
    TruncateCase<FdoDecimalValue, FdoDouble>(4, doubleFractValsInt16, expDoubleFractValsInt16, FdoDataType_Int16);
    TruncateCase<FdoDecimalValue, FdoDouble>(4, doubleFractValsInt32, expDoubleFractValsInt32, FdoDataType_Int32);


/* From Int16 Tests */
    FdoInt16 int16Vals[] = { 1, 0, 128, 255, 254, -1, -32768, 2000, 256, 32767 };
    FdoString* expInt16Vals[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767" };
    FdoString* expInt16ValsStr[] = { L"'1'", L"'0'", L"'128'", L"'255'", L"'254'", L"'-1'", L"'-32768'", L"'2000'", L"'256'", L"'32767'" };
    FdoString* expInt16ValsBool[] = { L"TRUE", L"FALSE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE" };
    FdoString* expInt16ValsByte[] = { L"1", L"0", L"128", L"255", L"254", L"0", L"0", L"255", L"255", L"255" };

    NormalToBoolCase<FdoInt16Value, FdoInt16>();
    NormalCase<FdoInt16Value, FdoInt16>(5, int16Vals, expInt16Vals, FdoDataType_Byte);
    IncompatibleCase<FdoInt16Value, FdoInt16>(int16Vals[7], FdoDataType_DateTime);
    NormalCase<FdoInt16Value, FdoInt16>(10, int16Vals, expInt16Vals, FdoDataType_Decimal);
    NormalCase<FdoInt16Value, FdoInt16>(10, int16Vals, expInt16Vals, FdoDataType_Double);
    NormalCase<FdoInt16Value, FdoInt16>(10, int16Vals, expInt16Vals, FdoDataType_Int16);
    NormalCase<FdoInt16Value, FdoInt16>(10, int16Vals, expInt16Vals, FdoDataType_Int32);
    NormalCase<FdoInt16Value, FdoInt16>(10, int16Vals, expInt16Vals, FdoDataType_Int64);
    NormalCase<FdoInt16Value, FdoInt16>(10, int16Vals, expInt16Vals, FdoDataType_Single);
    NormalCase<FdoInt16Value, FdoInt16>(10, int16Vals, expInt16ValsStr, FdoDataType_String);
    NullCases<FdoInt16Value>();
    TruncateCase<FdoInt16Value, FdoInt16>(8, &int16Vals[2], &expInt16ValsBool[2], FdoDataType_Boolean);
    TruncateCase<FdoInt16Value, FdoInt16>(5, &int16Vals[5], &expInt16ValsByte[5], FdoDataType_Byte);

/* From Int32 Tests */
    FdoInt32 int32Vals[] = { 1, 0, 128, 255, 254, -1, -32768, 2000, 256, 32767, -32769, 32768, 100000, -2147483647L - 1L, 2147483647 };
    FdoString* expInt32Vals[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767", L"-32769", L"32768", L"100000", L"-2147483648", L"2147483647" };
    FdoString* expInt32ValsStr[] = { L"'1'", L"'0'", L"'128'", L"'255'", L"'254'", L"'-1'", L"'-32768'", L"'2000'", L"'256'", L"'32767'", L"'-32769'", L"'32768'", L"'100000'", L"'-2147483648'", L"'2147483647'" };
    FdoString* expInt32ValsBool[] = { L"TRUE", L"FALSE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE" };
    FdoString* expInt32ValsByte[] = { L"1", L"0", L"128", L"255", L"254", L"0", L"0", L"255", L"255", L"255", L"0", L"255", L"255", L"0", L"255" };
    FdoString* expInt32ValsInt16[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767", L"-32768", L"32767", L"32767", L"-32768", L"32767" };
    FdoInt32 int32ShiftVals[] = { -2147483647, 2147483391, 2147483647 };
    FdoString* expInt32ShiftValsSingle[] = { L"-2.14748e+009", L"2.14748e+009", L"2.14748e+009" };

    NormalToBoolCase<FdoInt32Value, FdoInt32>();
    NormalCase<FdoInt32Value, FdoInt32>(5, int32Vals, expInt32Vals, FdoDataType_Byte);
    IncompatibleCase<FdoInt32Value, FdoInt32>(int32Vals[7], FdoDataType_DateTime);
    NormalCase<FdoInt32Value, FdoInt32>(15, int32Vals, expInt32Vals, FdoDataType_Decimal);
    NormalCase<FdoInt32Value, FdoInt32>(15, int32Vals, expInt32Vals, FdoDataType_Double);
    NormalCase<FdoInt32Value, FdoInt32>(10, int32Vals, expInt32Vals, FdoDataType_Int16);
    NormalCase<FdoInt32Value, FdoInt32>(15, int32Vals, expInt32Vals, FdoDataType_Int32);
    NormalCase<FdoInt32Value, FdoInt32>(15, int32Vals, expInt32Vals, FdoDataType_Int64);
    NormalCase<FdoInt32Value, FdoInt32>(13, int32Vals, expInt32Vals, FdoDataType_Single);
    NormalCase<FdoInt32Value, FdoInt32>(15, int32Vals, expInt32ValsStr, FdoDataType_String);
    NullCases<FdoInt32Value>();
    TruncateCase<FdoInt32Value, FdoInt32>(13, &int32Vals[2], &expInt32ValsBool[2], FdoDataType_Boolean);
    TruncateCase<FdoInt32Value, FdoInt32>(10, &int32Vals[5], &expInt32ValsByte[5], FdoDataType_Byte);
    TruncateCase<FdoInt32Value, FdoInt32>(5, &int32Vals[10], &expInt32ValsInt16[10], FdoDataType_Int16);
    ShiftCase<FdoInt32Value, FdoInt32>(3, int32ShiftVals, expInt32ShiftValsSingle, FdoDataType_Single);

/* From Int64 Tests */
    FdoInt64 int64Vals[] = { 1, 0, 128, 255, 254, -1, -32768, 2000, 256, 32767, -32769, 32768, 100000, -2147483648LL, 2147483647, -2147483649LL, 2147483648LL, -1000000000000LL, -9223372036854775807LL - 1LL, 9223372036854775807LL };
    FdoString* expInt64Vals[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767", L"-32769", L"32768", L"100000", L"-2147483648", L"2147483647", L"-2147483649", L"2147483648", L"-1000000000000", L"-9223372036854775808", L"9223372036854775807" };
    FdoString* expInt64ValsStr[] = { L"'1'", L"'0'", L"'128'", L"'255'", L"'254'", L"'-1'", L"'-32768'", L"'2000'", L"'256'", L"'32767'", L"'-32769'", L"'32768'", L"'100000'", L"'-2147483648'", L"'2147483647'", L"'-2147483649'", L"'2147483648'", L"'-1000000000000'", L"'-9223372036854775808'", L"'9223372036854775807'" };
    FdoString* expInt64ValsBool[] = { L"TRUE", L"FALSE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE" };
    FdoString* expInt64ValsByte[] = { L"1", L"0", L"128", L"255", L"254", L"0", L"0", L"255", L"255", L"255", L"0", L"255", L"255", L"0", L"255", L"0", L"255", L"0", L"0", L"255" };
    FdoString* expInt64ValsInt16[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767", L"-32768", L"32767", L"32767", L"-32768", L"32767", L"-32768", L"32767", L"-32768", L"-32768", L"32767" };
    FdoString* expInt64ValsInt32[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767", L"-32769", L"32768", L"100000", L"-2147483648", L"2147483647", L"-2147483648", L"2147483647", L"-2147483648", L"-2147483648", L"2147483647" };
    FdoInt64 int64ShiftVals[] = { - 9223372036854775807LL, 9223372036854775807LL - 4096, 9223372036854775807LL };
    FdoString* expInt64ShiftValsSingle[] = { L"-9.22337e+018", L"9.22337e+018", L"9.22337e+018" };
    FdoString* expInt64ShiftValsDouble[] = { L"-9.22337203685478e+018", L"9.22337203685477e+018", L"9.22337203685478e+018" };

    NormalToBoolCase<FdoInt64Value, FdoInt64>();
    NormalCase<FdoInt64Value, FdoInt64>(5, int64Vals, expInt64Vals, FdoDataType_Byte);
    IncompatibleCase<FdoInt64Value, FdoInt64>(int64Vals[7], FdoDataType_DateTime);
    NormalCase<FdoInt64Value, FdoInt64>(18, int64Vals, expInt64Vals, FdoDataType_Decimal);
    NormalCase<FdoInt64Value, FdoInt64>(18, int64Vals, expInt64Vals, FdoDataType_Double);
    NormalCase<FdoInt64Value, FdoInt64>(10, int64Vals, expInt64Vals, FdoDataType_Int16);
    NormalCase<FdoInt64Value, FdoInt64>(15, int64Vals, expInt64Vals, FdoDataType_Int32);
    NormalCase<FdoInt64Value, FdoInt64>(20, int64Vals, expInt64Vals, FdoDataType_Int64);
    NormalCase<FdoInt64Value, FdoInt64>(13, int64Vals, expInt64Vals, FdoDataType_Single);
    NormalCase<FdoInt64Value, FdoInt64>(20, int64Vals, expInt64ValsStr, FdoDataType_String);
    NullCases<FdoInt64Value>();
    TruncateCase<FdoInt64Value, FdoInt64>(18, &int64Vals[2], &expInt64ValsBool[2], FdoDataType_Boolean);
    TruncateCase<FdoInt64Value, FdoInt64>(15, &int64Vals[5], &expInt64ValsByte[5], FdoDataType_Byte);
    TruncateCase<FdoInt64Value, FdoInt64>(10, &int64Vals[10], &expInt64ValsInt16[10], FdoDataType_Int16);
    TruncateCase<FdoInt64Value, FdoInt64>(5, &int64Vals[15], &expInt64ValsInt32[15], FdoDataType_Int32);
    ShiftCase<FdoInt64Value, FdoInt64>(3, int64ShiftVals, expInt64ShiftValsSingle, FdoDataType_Single);
    ShiftCase<FdoInt64Value, FdoInt64>(3, int64ShiftVals, expInt64ShiftValsDouble, FdoDataType_Double);

/* From Single Tests */

    FdoFloat singleVals[] = { 1, 0, 128, 255, 254, -1, -32768, 2000, 256, 32767, -32769, 32768, 100000, (FdoFloat) -2147483648LL, (pow((FdoFloat)2, (FdoFloat)23) - 1) * pow((FdoFloat)2, (FdoFloat)8), -pow((FdoFloat)2, (FdoFloat)31) - pow((FdoFloat)2, (FdoFloat)8), pow((FdoFloat)2, (FdoFloat)31), (FdoFloat) -999999995904LL, -pow((FdoFloat)2,(FdoFloat) 63), (pow((FdoFloat)2, (FdoFloat)23) - 1) * pow((FdoFloat)2, (FdoFloat)40), -pow((FdoFloat)2, (FdoFloat)63) - pow((FdoFloat)2, (FdoFloat)40), pow((FdoFloat)2,(FdoFloat) 63), (FdoFloat)-1.892e31, (FdoFloat)-3.4e38, (FdoFloat)3.4e38 };
    FdoString* expSingleVals[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767", L"-32769", L"32768", L"100000", L"-2147483648", L"2147483392", L"-2147483904", L"2147483648", L"-999999995904", L"-9223372036854775808", L"9223370937343148032", L"todo", L"todo", L"todo", L"todo", L"todo" };
    FdoString* expSingleValsStr[] = { L"'1'", L"'0'", L"'128'", L"'255'", L"'254'", L"'-1'", L"'-32768'", L"'2000'", L"'256'", L"'32767'", L"'-32769'", L"'32768'", L"'100000'", L"'-2.14748e+009'", L"'2.14748e+009'", L"'-2.14748e+009'", L"'2.14748e+009'", L"'-999999995904'", L"'-9.22337203685478e+018'", L"'9.22337203685477e+018'", L"todo", L"todo", L"todo", L"todo", L"todo" };
    FdoString* expSingleValsBool[] = { L"TRUE", L"FALSE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE" };
    FdoString* expSingleValsByte[] = { L"1", L"0", L"128", L"255", L"254", L"0", L"0", L"255", L"255", L"255", L"0", L"255", L"255", L"0", L"255", L"0", L"255", L"0", L"0", L"255", L"0", L"255", L"0", L"0", L"255" };
    FdoString* expSingleValsInt16[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767", L"-32768", L"32767", L"32767", L"-32768", L"32767", L"-32768", L"32767", L"-32768", L"-32768", L"32767", L"-32768", L"32767", L"-32768", L"-32768", L"32767" };
    FdoString* expSingleValsInt32[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767", L"-32769", L"32768", L"100000", L"-2147483648", L"2147483647", L"-2147483648", L"2147483647", L"-2147483648", L"-2147483648", L"2147483647", L"-2147483648", L"2147483647", L"-2147483648", L"-2147483648", L"2147483647" };
    FdoString* expSingleValsInt64[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767", L"-32769", L"32768", L"100000", L"-2147483648", L"2147483647", L"-2147483904", L"-2147483904", L"-2147483648", L"-2147483648", L"2147483647", L"-9223372036854775808", L"9223372036854775807", L"-9223372036854775808", L"-9223372036854775808", L"9223372036854775807" };
    FdoFloat singleFractVals[] = { (FdoFloat)-5.8, (FdoFloat)-5.5, (FdoFloat)-5.3, (FdoFloat)-0.51, (FdoFloat)-0.5, (FdoFloat)-0.1, (FdoFloat)0.2, (FdoFloat)0.5, (FdoFloat)0.51, (FdoFloat)100.4, (FdoFloat)101.5, (FdoFloat)105.9 };
    FdoString* expSingleFractVals[] = { L"-5.8", L"-5.5", L"-5.3", L"-0.51", L"-0.5", L"-0.1", L"0.2", L"0.5", L"0.51", L"100.4", L"101.5", L"105.9" };
    FdoString* expSingleRoundVals[] = { L"-6", L"-6", L"-5", L"-1", L"-1", L"0", L"0", L"1", L"1", L"100", L"102", L"106" };
    FdoString* expSingleFractValsStr[] = { L"'-5.8'", L"'-5.5'", L"'-5.3'", L"'-0.51'", L"'-0.5'", L"'-0.1'", L"'0.2'", L"'0.5'", L"'0.51'", L"'100.4'", L"'101.5'", L"'105.9'" };
    FdoString* expSingleRoundValsBool[] = { L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE" };
    FdoFloat singleFractValsByte[] = { (FdoFloat)-0.5, (FdoFloat)-0.1, (FdoFloat)255.2, (FdoFloat)255.5 };
    FdoString* expSingleFractValsByte[] = { L"0", L"0", L"255", L"255" };
    FdoFloat singleFractValsInt16[] = { (FdoFloat)-32768.5, (FdoFloat)-32768.1, (FdoFloat)32767.2, (FdoFloat)32767.5 };
    FdoString* expSingleFractValsInt16[] = { L"-32768", L"-32768", L"32767", L"32767" };

    NormalToBoolCase<FdoSingleValue, FdoFloat>();
    NormalCase<FdoSingleValue, FdoFloat>(5, singleVals, expSingleVals, FdoDataType_Byte);
    IncompatibleCase<FdoSingleValue, FdoFloat>(singleVals[7], FdoDataType_DateTime);
    NormalCase<FdoSingleValue, FdoFloat>(25, singleVals, expSingleVals, FdoDataType_Decimal);
    NormalCase<FdoSingleValue, FdoFloat>(25, singleVals, expSingleVals, FdoDataType_Double);
    NormalCase<FdoSingleValue, FdoFloat>(10, singleVals, expSingleVals, FdoDataType_Int16);
    NormalCase<FdoSingleValue, FdoFloat>(15, singleVals, expSingleVals, FdoDataType_Int32);
    NormalCase<FdoSingleValue, FdoFloat>(20, singleVals, expSingleVals, FdoDataType_Int64);
    NormalCase<FdoSingleValue, FdoFloat>(25, singleVals, expSingleVals, FdoDataType_Single);
    NormalCase<FdoSingleValue, FdoFloat>(13, singleVals, expSingleValsStr, FdoDataType_String);
    NullCases<FdoSingleValue>();
    TruncateCase<FdoSingleValue, FdoFloat>(23, &singleVals[2], &expSingleValsBool[2], FdoDataType_Boolean);
    TruncateCase<FdoSingleValue, FdoFloat>(20, &singleVals[5], &expSingleValsByte[5], FdoDataType_Byte);
    TruncateCase<FdoSingleValue, FdoFloat>(15, &singleVals[10], &expSingleValsInt16[10], FdoDataType_Int16);
    TruncateCase<FdoSingleValue, FdoFloat>(10, &singleVals[15], &expSingleValsInt32[15], FdoDataType_Int32);
    TruncateCase<FdoSingleValue, FdoFloat>(5, &singleVals[20], &expSingleValsInt64[20], FdoDataType_Int64);
    TruncateCase<FdoSingleValue, FdoFloat>(12, singleFractVals, expSingleRoundValsBool, FdoDataType_Boolean);
    NormalCase<FdoSingleValue, FdoFloat>(12, singleFractVals, expSingleFractVals, FdoDataType_Decimal);
    NormalCase<FdoSingleValue, FdoFloat>(12, singleFractVals, expSingleFractVals, FdoDataType_Double);
    ShiftCase<FdoSingleValue, FdoFloat>(12, singleFractVals, expSingleRoundVals, FdoDataType_Int16);
    ShiftCase<FdoSingleValue, FdoFloat>(12, singleFractVals, expSingleRoundVals, FdoDataType_Int32);
    ShiftCase<FdoSingleValue, FdoFloat>(12, singleFractVals, expSingleRoundVals, FdoDataType_Int64);
    NormalCase<FdoSingleValue, FdoFloat>(12, singleFractVals, expSingleVals, FdoDataType_Single);
    NormalCase<FdoSingleValue, FdoFloat>(12, singleFractVals, expSingleFractValsStr, FdoDataType_String);
    TruncateCase<FdoDecimalValue, FdoFloat>(4, singleFractValsByte, expSingleFractValsByte, FdoDataType_Byte);
    TruncateCase<FdoDecimalValue, FdoFloat>(4, singleFractValsInt16, expSingleFractValsInt16, FdoDataType_Int16);
    ShiftCase<FdoSingleValue, FdoFloat>(4, &singleVals[13], &expSingleValsStr[13], FdoDataType_String);

/* From String Tests */

    FdoString* stringVals[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767", L"-32769", L"32768", L"100000", L"-2147483648", L"2147483647", L"-2147483649", L"2147483648", L"-1000000000000", L"-9223372036854775808", L"9223372036854775807" };
    FdoString* stringValsDt[] = { L"TIMESTAMP '2008-10-30 16:00:30'" };
    FdoString* expStringValsBool[] = { L"TRUE", L"FALSE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE", L"TRUE" };
    FdoString* expStringValsStr[] = { L"'1'", L"'0'", L"'128'", L"'255'", L"'254'", L"'-1'", L"'-32768'", L"'2000'", L"'256'", L"'32767'", L"'-32769'", L"'32768'", L"'100000'", L"'-2147483648'", L"'2147483647'", L"'-2147483649'", L"'2147483648'", L"'-1000000000000'", L"'-9223372036854775808'", L"'9223372036854775807'" };
    FdoString* expStringValsByte[] = { L"1", L"0", L"128", L"255", L"254", L"0", L"0", L"255", L"255", L"255", L"0", L"255", L"255", L"0", L"255", L"0", L"255", L"0", L"0", L"255", L"0", L"255", L"0", L"0", L"255" };
    FdoString* expStringValsInt16[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767", L"-32768", L"32767", L"32767", L"-32768", L"32767", L"-32768", L"32767", L"-32768", L"-32768", L"32767", L"-32768", L"32767", L"-32768", L"-32768", L"32767" };
    FdoString* expStringValsInt32[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767", L"-32769", L"32768", L"100000", L"-2147483648", L"2147483647", L"-2147483648", L"2147483647", L"-2147483648", L"-2147483648", L"2147483647", L"-2147483648", L"2147483647", L"-2147483648", L"-2147483648", L"2147483647" };
    FdoString* expStringValsInt64[] = { L"1", L"0", L"128", L"255", L"254", L"-1", L"-32768", L"2000", L"256", L"32767", L"-32769", L"32768", L"100000", L"-2147483648", L"2147483647", L"-2147483904", L"-2147483904", L"-2147483648", L"-2147483648", L"2147483647", L"-9223372036854775808", L"9223372036854775807", L"-9223372036854775808", L"-9223372036854775808", L"9223372036854775807" };
    FdoString* stringFractVals[] = { L"8.125", L"15.5" };
    FdoString* expStringRoundVals[] = { L"8", L"16" };
    FdoString* expStringRoundValsBool[] = { L"TRUE", L"TRUE" };

    NormalCase<FdoStringValue, FdoString*>(2, stringVals, expStringValsBool, FdoDataType_Boolean);
    NormalCase<FdoStringValue, FdoString*>(2, expStringValsBool, expStringValsBool, FdoDataType_Boolean);
    NormalCase<FdoStringValue, FdoString*>(5, stringVals, stringVals, FdoDataType_Byte);
    NormalCase<FdoStringValue, FdoString*>(1, stringValsDt, stringValsDt, FdoDataType_DateTime);
    IncompatibleCase<FdoStringValue, FdoString*>(stringVals[7], FdoDataType_DateTime);
    NormalCase<FdoStringValue, FdoString*>(18, stringVals, stringVals, FdoDataType_Decimal);
    NormalCase<FdoStringValue, FdoString*>(18, stringVals, stringVals, FdoDataType_Double);
    NormalCase<FdoStringValue, FdoString*>(10, stringVals, stringVals, FdoDataType_Int16);
    NormalCase<FdoStringValue, FdoString*>(15, stringVals, stringVals, FdoDataType_Int32);
    NormalCase<FdoStringValue, FdoString*>(20, stringVals, stringVals, FdoDataType_Int64);
    NormalCase<FdoStringValue, FdoString*>(13, stringVals, stringVals, FdoDataType_Single);
    NormalCase<FdoStringValue, FdoString*>(20, stringVals, expStringValsStr, FdoDataType_String);
    NullCases<FdoStringValue>();
    IncompatibleCase<FdoStringValue, FdoString*>(L"NEITHER", FdoDataType_Boolean);
    IncompatibleCase<FdoStringValue, FdoString*>(L"NEITHER", FdoDataType_Byte);
    IncompatibleCase<FdoStringValue, FdoString*>(L"NEITHER", FdoDataType_Byte);
    IncompatibleCase<FdoStringValue, FdoString*>(L"8 + 9", FdoDataType_Decimal);
    IncompatibleCase<FdoStringValue, FdoString*>(L"NEITHER", FdoDataType_Double);
    IncompatibleCase<FdoStringValue, FdoString*>(L"NEITHER", FdoDataType_Int16);
    IncompatibleCase<FdoStringValue, FdoString*>(L"NEITHER", FdoDataType_Int32);
    IncompatibleCase<FdoStringValue, FdoString*>(L"NEITHER", FdoDataType_Int64);
    IncompatibleCase<FdoStringValue, FdoString*>(L"NEITHER", FdoDataType_Single);
    TruncateCase<FdoStringValue, FdoString*>(18, &stringVals[2], &expStringValsBool[2], FdoDataType_Boolean);
    TruncateCase<FdoStringValue, FdoString*>(15, &stringVals[5], &expStringValsByte[5], FdoDataType_Byte);
    TruncateCase<FdoStringValue, FdoString*>(15, &stringVals[5], &expStringValsByte[5], FdoDataType_Byte);
    TruncateCase<FdoStringValue, FdoString*>(10, &stringVals[10], &expStringValsInt16[10], FdoDataType_Int16);
    TruncateCase<FdoStringValue, FdoString*>(5, &stringVals[15], &expStringValsInt32[15], FdoDataType_Int32);
//    TruncateCase<FdoStringValue, FdoString*>(5, &stringVals[20], &expStringValsInt64[20], FdoDataType_Int64);
    TruncateCase<FdoStringValue, FdoString*>(2, stringFractVals, expStringRoundValsBool, FdoDataType_Boolean);
    ShiftCase<FdoStringValue, FdoString*>(2, stringFractVals, expStringRoundVals, FdoDataType_Byte);
    NormalCase<FdoStringValue, FdoString*>(2, stringFractVals, stringFractVals, FdoDataType_Decimal);
    NormalCase<FdoStringValue, FdoString*>(2, stringFractVals, stringFractVals, FdoDataType_Double);
    ShiftCase<FdoStringValue, FdoString*>(2, stringFractVals, expStringRoundVals, FdoDataType_Int16);
    ShiftCase<FdoStringValue, FdoString*>(2, stringFractVals, expStringRoundVals, FdoDataType_Int32);
    ShiftCase<FdoStringValue, FdoString*>(2, stringFractVals, expStringRoundVals, FdoDataType_Int64);
    NormalCase<FdoStringValue, FdoString*>(2, stringFractVals, stringFractVals, FdoDataType_Single);

/* From BLOB cases */
    FdoByte lobBytes[4] = { 'a', 'b', 'c', 0 };
    FdoPtr<FdoByteArray> lobValue = FdoByteArray::Create( lobBytes, 4 );
    FdoByteArray* lobVals[] = { lobValue.p };
    FdoString* expLobVals[] = { L"abc" };
    NormalCase<FdoBLOBValue, FdoByteArray*>(1, lobVals, expLobVals, FdoDataType_BLOB);
    IncompatibleCase<FdoBLOBValue, FdoByteArray*>(lobValue, FdoDataType_Boolean);
    IncompatibleCase<FdoBLOBValue, FdoByteArray*>(lobValue, FdoDataType_Byte);
    IncompatibleCase<FdoBLOBValue, FdoByteArray*>(lobValue, FdoDataType_Decimal);
    IncompatibleCase<FdoBLOBValue, FdoByteArray*>(lobValue, FdoDataType_Double);
    IncompatibleCase<FdoBLOBValue, FdoByteArray*>(lobValue, FdoDataType_Int16);
    IncompatibleCase<FdoBLOBValue, FdoByteArray*>(lobValue, FdoDataType_Int32);
    IncompatibleCase<FdoBLOBValue, FdoByteArray*>(lobValue, FdoDataType_Int64);
    IncompatibleCase<FdoBLOBValue, FdoByteArray*>(lobValue, FdoDataType_Single);

/* From CLOB cases */
    NormalCase<FdoCLOBValue, FdoByteArray*>(1, lobVals, expLobVals, FdoDataType_CLOB);
    IncompatibleCase<FdoCLOBValue, FdoByteArray*>(lobValue, FdoDataType_Boolean);
    IncompatibleCase<FdoCLOBValue, FdoByteArray*>(lobValue, FdoDataType_Byte);
    IncompatibleCase<FdoCLOBValue, FdoByteArray*>(lobValue, FdoDataType_Decimal);
    IncompatibleCase<FdoCLOBValue, FdoByteArray*>(lobValue, FdoDataType_Double);
    IncompatibleCase<FdoCLOBValue, FdoByteArray*>(lobValue, FdoDataType_Int16);
    IncompatibleCase<FdoCLOBValue, FdoByteArray*>(lobValue, FdoDataType_Int32);
    IncompatibleCase<FdoCLOBValue, FdoByteArray*>(lobValue, FdoDataType_Int64);
    IncompatibleCase<FdoCLOBValue, FdoByteArray*>(lobValue, FdoDataType_Single);

/* The above exercises all of the Create functions for the specific 
   FdoDataValue derivations but does not verify that they are exposed
   via the FDO API. The following verifies this
*/

    FdoPtr<FdoBooleanValue> bool1 = FdoBooleanValue::Create(true);
    FdoPtr<FdoBooleanValue> bool2 = FdoBooleanValue::Create( bool1 );
    CPPUNIT_ASSERT( bool2->GetBoolean() );

    FdoPtr<FdoByteValue> byte1 = FdoByteValue::Create(45);
    FdoPtr<FdoByteValue> byte2 = FdoByteValue::Create( byte1 );
    CPPUNIT_ASSERT( byte2->GetByte() == 45 );

    FdoPtr<FdoDecimalValue> decimal2 = FdoDecimalValue::Create(byte2);
    CPPUNIT_ASSERT( decimal2->GetDecimal() == 45 );

    FdoPtr<FdoDoubleValue> double2 = FdoDoubleValue::Create(byte2);
    CPPUNIT_ASSERT( double2->GetDouble() == 45 );

    FdoPtr<FdoInt16Value> int16v = FdoInt16Value::Create(byte2);
    CPPUNIT_ASSERT( int16v->GetInt16() == 45 );

    FdoPtr<FdoInt32Value> int32v = FdoInt32Value::Create(byte2);
    CPPUNIT_ASSERT( int32v->GetInt32() == 45 );

    FdoPtr<FdoInt64Value> int64v = FdoInt64Value::Create(byte2);
    CPPUNIT_ASSERT( int64v->GetInt64() == 45 );

    FdoPtr<FdoSingleValue> single2 = FdoSingleValue::Create(byte2);
    CPPUNIT_ASSERT( single2->GetSingle() == 45 );

    FdoPtr<FdoStringValue> string2 = FdoStringValue::Create(byte2);
    CPPUNIT_ASSERT( wcscmp(string2->GetString(), L"45") == 0 );

    FdoByte bytes1[3] = { 8, 42, 156 };
    FdoPtr<FdoByteArray> byteArray1 = FdoByteArray::Create( bytes1, 3 );
    FdoPtr<FdoBLOBValue> blob1 = FdoBLOBValue::Create( byteArray1 );
    FdoPtr<FdoBLOBValue> blob2 = FdoBLOBValue::Create( blob1 );
    FdoPtr<FdoByteArray> byteArray2 = blob2->GetData();
    CPPUNIT_ASSERT( byteArray2->GetCount() == 3 ); 
    FdoByte* bytes2 = byteArray2->GetData();
    CPPUNIT_ASSERT( bytes2[0] == bytes1[0] ); 
    CPPUNIT_ASSERT( bytes2[1] == bytes1[1] ); 
    CPPUNIT_ASSERT( bytes2[2] == bytes1[2] ); 

    FdoByte chars1[4] = { 'a', 'c', 'x', 'z' };
    FdoPtr<FdoByteArray> charArray1 = FdoByteArray::Create( chars1, 4 );
    FdoPtr<FdoCLOBValue> clob1 = FdoCLOBValue::Create( charArray1 );
    FdoPtr<FdoCLOBValue> clob2 = FdoCLOBValue::Create( clob1 );
    FdoPtr<FdoByteArray> charArray2 = clob2->GetData();
    CPPUNIT_ASSERT( charArray2->GetCount() == 4 ); 
    FdoByte* chars2 = charArray2->GetData();
    CPPUNIT_ASSERT( chars2[0] == chars1[0] ); 
    CPPUNIT_ASSERT( chars2[1] == chars1[1] ); 
    CPPUNIT_ASSERT( chars2[2] == chars1[2] ); 
    CPPUNIT_ASSERT( chars2[3] == chars1[3] ); 
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

void ExpressionTest::NormalSubCase( FdoDataValue* val, FdoString* expected, FdoDataType outType, FdoBoolean nullIfIncompatible, FdoBoolean shift, FdoBoolean truncate )
{
    FdoPtr<FdoDataValue> dv2 = FdoDataValue::Create( 
        outType,
        val,
        nullIfIncompatible,
        shift,
        truncate
    );

    if ( (!dv2) || dv2->IsNull() ) 
        throw FdoException::Create( L"NormalSubCase:: result is NULL" );

    if ( dv2->GetDataType() != outType ) 
        throw FdoException::Create( L"NormalSubCase:: result has wrong data type" );

    FdoDataType valtype =  val->GetDataType();

    if ( (valtype == FdoDataType_Decimal || valtype == FdoDataType_Double || valtype == FdoDataType_Single) &&
         (outType == FdoDataType_Decimal || outType == FdoDataType_Double || outType == FdoDataType_Single)
    ) {
/* For future
        if ( val->Compare(dv2) != FdoCompareType_Equal )
        {
            throw FdoException::Create(
                FdoStringP::Format(
                    L"NormalSubCase(double): got %ls, expected %ls",
                    dv2->ToString(),
                    val->ToString()
                )
            );
        }
*/
        double valDbl = 0;
        double outDbl = 0;

        switch ( valtype ) 
        {
        case FdoDataType_Decimal:
            valDbl = ((FdoDecimalValue*) val)->GetDecimal();
            break;
        case FdoDataType_Double:
            valDbl = ((FdoDoubleValue*) val)->GetDouble();
            break;
        case FdoDataType_Single:
            valDbl = ((FdoSingleValue*) val)->GetSingle();
            break;
        }

        switch ( outType ) 
        {
        case FdoDataType_Decimal:
            outDbl = ((FdoDecimalValue*) dv2.p)->GetDecimal();
            break;
        case FdoDataType_Double:
            outDbl = ((FdoDoubleValue*) dv2.p)->GetDouble();
            break;
        case FdoDataType_Single:
            outDbl = ((FdoSingleValue*) dv2.p)->GetSingle();
            break;
        }

        if ( outDbl != valDbl )
        {
            throw FdoException::Create(
                FdoStringP::Format(
                    L"NormalSubCase(double): got %ls, expected %ls",
                    dv2->ToString(),
                    val->ToString()
                )
            );
        }

    }
    else if ( outType == FdoDataType_BLOB )
    {
        FdoPtr<FdoByteArray> outArray = ((FdoBLOBValue*) dv2.p)->GetData();
        FdoStringP outString = (char*) outArray->GetData();
        if ( outString != expected)
        {
            throw FdoException::Create(
                FdoStringP::Format(
                    L"NormalSubCase: got %ls, expected %ls",
                    (FdoString*) outString,
                    expected
                )
            );
        }

    }
    else if ( outType == FdoDataType_CLOB )
    {
        FdoPtr<FdoByteArray> outArray = ((FdoCLOBValue*) dv2.p)->GetData();
        FdoStringP outString = (char*) outArray->GetData();
        if ( outString != expected)
        {
            throw FdoException::Create(
                FdoStringP::Format(
                    L"NormalSubCase: got %ls, expected %ls",
                    (FdoString*) outString,
                    expected
                )
            );
        }

    }
    else
    {
        if ( wcscmp(expected, dv2->ToString()) != 0)
        {
            throw FdoException::Create(
                FdoStringP::Format(
                    L"NormalSubCase: got %ls, expected %ls",
                    dv2->ToString(),
                    expected
                )
            );
        }
    }
}

void ExpressionTest::NullSubSubCase( FdoDataValue* val, FdoDataType outType, FdoBoolean nullIfIncompatible, FdoBoolean shift, FdoBoolean truncate )
{
    FdoPtr<FdoDataValue> dv2 = FdoDataValue::Create( 
        outType,
        val,
        nullIfIncompatible,
        shift,
        truncate
    );

    if ( (!dv2) ) 
        throw FdoException::Create( L"NullSubCase:: result is NULL pointer" );

    if ( dv2->GetDataType() != outType ) 
        throw FdoException::Create( L"NullSubSubCase:: result has wrong data type" );
      
    if ( !(dv2->IsNull()) ) 
        throw FdoException::Create( L"NullSubCase:: result is not null" );
}

void ExpressionTest::IncompatibleSubCase( FdoDataValue* val, FdoDataType outType, FdoBoolean nullIfIncompatible, FdoBoolean shift, FdoBoolean truncate )
{
    bool success = false;
    FdoPtr<FdoDataValue> dv2;
    
    try {
        dv2 = FdoDataValue::Create( 
            outType,
            val,
            nullIfIncompatible,
            shift,
            truncate
        );

        success = true;
    }
    catch ( FdoExpressionException* ex ) 
    {
        if ( nullIfIncompatible ) 
            throw;
        else
            FDO_SAFE_RELEASE(ex);
    }

    if ( success )
    {
        if ( !nullIfIncompatible ) 
            throw FdoException::Create( L"IncompatibleSubCase: conversion succeeded, expected exception" );

        if ( !dv2 ) 
            throw FdoException::Create( L"IncompatibleSubCase:: result is NULL pointer" );

        if ( !(dv2->IsNull()) ) 
            throw FdoException::Create( L"IncompatibleSubCase: result is not null" );
    }
}

void ExpressionTest::TruncateSubCase( FdoDataValue* val, FdoString* expected, FdoDataType outType, FdoBoolean nullIfIncompatible, FdoBoolean shift, FdoBoolean truncate )
{
    bool success = false;
    FdoPtr<FdoDataValue> dv2;
    
    try {
        dv2 = FdoDataValue::Create( 
            outType,
            val,
            nullIfIncompatible,
            shift,
            truncate
        );

        success = true;
    }
    catch ( FdoExpressionException* ex ) 
    {
        if ( truncate || nullIfIncompatible ) 
            throw;
        else
            FDO_SAFE_RELEASE(ex);
    }


    if ( success ) 
    {
        if ( !dv2 ) 
            throw FdoException::Create( L"TruncateSubCase:: result is NULL pointer" );

        if ( dv2->GetDataType() != outType ) 
            throw FdoException::Create( L"TruncateSubCase:: result has wrong data type" );

        if ( truncate ) 
        {
            if ( dv2->IsNull() ) 
                throw FdoException::Create( L"TruncateSubCase:: result is NULL" );

            if ( wcscmp(expected, dv2->ToString()) != 0)
            {
                throw FdoException::Create(
                    FdoStringP::Format(
                        L"TruncateSubCase: got %ls, expected %ls",
                        dv2->ToString(),
                        expected
                    )
                );
            }
        }
        else 
        {
            if ( !(dv2->IsNull()) ) 
                throw FdoException::Create( L"TruncateSubCase:: result is not NULL" );
        }
    }
}

void ExpressionTest::ShiftSubCase( FdoDataValue* val, FdoString* expected, FdoDataType outType, FdoBoolean nullIfIncompatible, FdoBoolean shift, FdoBoolean truncate )
{
    bool success = false;
    FdoPtr<FdoDataValue> dv2;
    
    try {
        dv2 = FdoDataValue::Create( 
            outType,
            val,
            nullIfIncompatible,
            shift,
            truncate
        );

        success = true;
    }
    catch ( FdoExpressionException* ex ) 
    {
        if ( shift || nullIfIncompatible ) 
            throw;
        else
            FDO_SAFE_RELEASE(ex);
    }


    if ( success ) 
    {
        if ( !dv2 ) 
            throw FdoException::Create( L"ShiftSubCase:: result is NULL pointer" );

        if ( dv2->GetDataType() != outType ) 
            throw FdoException::Create( L"ShiftSubCase:: result has wrong data type" );

        if ( shift ) 
        {
            if ( dv2->IsNull() ) 
                throw FdoException::Create( L"ShiftSubCase:: result is NULL" );

            if ( wcscmp(expected, dv2->ToString()) != 0)
            {
                throw FdoException::Create(
                    FdoStringP::Format(
                        L"ShiftSubCase: got %ls, expected %ls",
                        dv2->ToString(),
                        expected
                    )
                );
            }
        }
        else 
        {
            if ( !(dv2->IsNull()) ) 
                throw FdoException::Create( L"ShiftSubCase:: result is not NULL" );
        }
    }
}



