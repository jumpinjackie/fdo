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

using System;
using System.Diagnostics;
using System.Text;
using OSGeo.FDO.Expression;
using OSGeo.FDO.Schema;

namespace Fdo_Test
{
	/// <summary>
	/// Summary description for mgExpressionTest.
	/// </summary>
	public class mgExpressionTest 
	{
		public mgExpressionTest()
		{
			Console.WriteLine("start mgExpressionTest");
		}

		public void testExpressionTree()
		{
			Console.WriteLine("start mgExpressionTest.testExpressionTree");

			// create a tree of Expression Classes
			DataValue pDVal = new StringValue("abc");
			Function pFunc = new Function("FunctionName", new Expression[0]);
			BinaryExpression pExprMult = new BinaryExpression(pDVal, BinaryOperations.BinaryOperations_Multiply, pFunc);
			
			Identifier pIdent = new Identifier("this.is.a.test");
			Parameter pParam = new Parameter("AndSoIsThis");
			BinaryExpression pExprDiv = new BinaryExpression(pIdent, BinaryOperations.BinaryOperations_Divide, pParam);
			BinaryExpression pExprAdd = new BinaryExpression(pExprDiv, BinaryOperations.BinaryOperations_Add, pExprMult);

			byte[] pFGF = (new ASCIIEncoding()).GetBytes("SomeGeometryData");
			GeometryValue pGVal = new GeometryValue(pFGF);

#if DUMPTESTCONTENT
            Console.WriteLine(pGVal.ToString());
#endif

			BinaryExpression pExprSub = new BinaryExpression(pGVal, BinaryOperations.BinaryOperations_Subtract, pExprAdd);

			UnaryExpression pExprNeg = new UnaryExpression(UnaryOperations.UnaryOperations_Negate, pExprSub);;

			// test ToString
#if DUMPTESTCONTENT
            Console.WriteLine(pExprNeg.ToString());
#endif

            // test Processor
			mgUnitTestProcessor testProc = new mgUnitTestProcessor();
			pExprNeg.Process(testProc);			
			
			//Console.WriteLine("finish mgExpressionTest.testExpressionTree");
			
		}

		public void testBinaryExpression()
		{
			Console.WriteLine("start mgExpressionTest.testBinaryExpression");

			// create some data for the test.
			DataValue pDVal = new DataValue("abc");
			Parameter pParam = new Parameter("AndSoIsThis");

			// test the constructor
			BinaryExpression pBExpr = new BinaryExpression(pDVal, BinaryOperations.BinaryOperations_Multiply, pParam);
			Expression pExpr = pBExpr.LeftExpression;
			Debug.Assert(pExpr == pDVal);

			Debug.Assert(pBExpr.Operation == BinaryOperations.BinaryOperations_Multiply);

			pExpr = pBExpr.RightExpression;
			Debug.Assert(pExpr == pParam);

			// test the setters and getters with null values
			pBExpr.LeftExpression = null;
			pBExpr.RightExpression = null;
			Debug.Assert(pBExpr.LeftExpression == null);
			Debug.Assert(pBExpr.RightExpression == null);

			// test the setters and getters
			pBExpr.LeftExpression = pParam;
			pBExpr.Operation = BinaryOperations.BinaryOperations_Add;
			pBExpr.RightExpression = pDVal;
			pExpr = pBExpr.LeftExpression;
			Debug.Assert(pExpr == pParam);
			Debug.Assert(pBExpr.Operation == BinaryOperations.BinaryOperations_Add);
			pExpr = pBExpr.RightExpression;
			Debug.Assert(pExpr == pDVal);

			//Console.WriteLine("finish mgExpressionTest.testBinaryExpression");
			
		}

		public void testDataValue()
		{
			Console.WriteLine("start mgExpressionTest.testDataValue");

			// test the default constructor
			BooleanValue pDVal1 = new BooleanValue();
			Debug.Assert(pDVal1.IsNull());
			string pStr = pDVal1.ToString();
			
			Debug.Assert(pStr == "NULL");
			pDVal1.Boolean = true;
			Debug.Assert(!pDVal1.IsNull());
			Debug.Assert(pDVal1.DataType == DataType.DataType_Boolean);
			Debug.Assert(pDVal1.Boolean);

			ByteValue pDVal2 = new ByteValue();
			Debug.Assert(pDVal2.IsNull());
			pStr = pDVal2.ToString();
			Debug.Assert(pStr == "NULL");
			pDVal2.Byte = 0x03;
			Debug.Assert(!pDVal2.IsNull());
			Debug.Assert(pDVal2.DataType == DataType.DataType_Byte);
			Debug.Assert(pDVal2.Byte == 0x03);

			DateTimeValue pDVal3 = new DateTimeValue();
			Debug.Assert(pDVal3.IsNull());
			pStr = pDVal3.ToString();
			Debug.Assert(pStr == "NULL");
			DateTime timeVal = new DateTime(1962, 1, 15, 17, 11, 0);
			pDVal3.DateTime = timeVal;
			Debug.Assert(!pDVal3.IsNull());
			Debug.Assert(pDVal3.DataType == DataType.DataType_DateTime);
			DateTime timeVal1 = pDVal3.DateTime;
			Debug.Assert(timeVal1.Year == timeVal.Year);
			Debug.Assert(timeVal1.Month == timeVal.Month);
			Debug.Assert(timeVal1.Day == timeVal.Day);
			Debug.Assert(timeVal1.Hour == timeVal.Hour);
			Debug.Assert(timeVal1.Minute == timeVal.Minute);
			Debug.Assert(timeVal1.Second == timeVal.Second);
			pDVal3.ToString();

			DecimalValue pDVal4 = new DecimalValue();
			Debug.Assert(pDVal4.IsNull());
			pStr = pDVal4.ToString();
			Debug.Assert(pStr == "NULL");
			pDVal4.Decimal = 3.14;
			Debug.Assert(!pDVal4.IsNull());
			Debug.Assert(pDVal4.DataType == DataType.DataType_Decimal);
			Debug.Assert(pDVal4.Decimal == 3.14);

			DoubleValue pDVal5 = new DoubleValue();
			Debug.Assert(pDVal5.IsNull());
			pStr = pDVal5.ToString();
			Debug.Assert(pStr == "NULL");
			pDVal5.Double = 2.234;
			Debug.Assert(!pDVal5.IsNull());
			Debug.Assert(pDVal5.DataType == DataType.DataType_Double);
			Debug.Assert(pDVal5.Double == 2.234);

			Int16Value pDVal6 = new Int16Value();
			Debug.Assert(pDVal6.IsNull());
			pStr = pDVal6.ToString();
			Debug.Assert(pStr == "NULL");
			pDVal6.Int16 = 32700;
			Debug.Assert(!pDVal6.IsNull());
			Debug.Assert(pDVal6.DataType == DataType.DataType_Int16);
			Debug.Assert(pDVal6.Int16 == 32700);

			Int32Value pDVal7 = new Int32Value();
			Debug.Assert(pDVal7.IsNull());
			pStr = pDVal7.ToString();
			Debug.Assert(pStr == "NULL");
			pDVal7.Int32 = 93000000;
			Debug.Assert(!pDVal7.IsNull());
			Debug.Assert(pDVal7.DataType == DataType.DataType_Int32);
			Debug.Assert(pDVal7.Int32 == 93000000);

			Int64Value pDVal8 = new Int64Value();
			Debug.Assert(pDVal8.IsNull());
			pStr = pDVal8.ToString();
			Debug.Assert(pStr == "NULL");
			pDVal8.Int64 = 12345678901234L;
			Debug.Assert(!pDVal8.IsNull());
			Debug.Assert(pDVal8.DataType == DataType.DataType_Int64);
			Debug.Assert(pDVal8.Int64 == 12345678901234L);

			SingleValue pDVal9 = new SingleValue();
			Debug.Assert(pDVal9.IsNull());
			pStr = pDVal9.ToString();
			Debug.Assert(pStr == "NULL");
			pDVal9.Single = 1.21f;
			Debug.Assert(!pDVal9.IsNull());
			Debug.Assert(pDVal9.DataType == DataType.DataType_Single);
			Debug.Assert(pDVal9.Single == 1.21f);

			StringValue pDVal10 = new StringValue();
			Debug.Assert(pDVal10.IsNull());
			pStr = pDVal10.ToString();
			Debug.Assert(pStr == "NULL");  // this was throwing an exception.  Make sure it returns a string.
			pDVal10.String = "TestValue";
			Debug.Assert(!pDVal10.IsNull());
			Debug.Assert(pDVal10.DataType == DataType.DataType_String);
			Debug.Assert(pDVal10.String == "TestValue");

			BLOBValue pDVal11 = new BLOBValue();
			Debug.Assert(pDVal11.IsNull());
			pStr = pDVal11.ToString();
			Debug.Assert(pStr == "{NULL}");
			byte [] pByteSet = (new ASCIIEncoding()).GetBytes("123456");
			pDVal11.Data = pByteSet;
			Debug.Assert(!pDVal11.IsNull());
			Debug.Assert(pDVal11.DataType == DataType.DataType_BLOB);
			byte[] pByteGet = pDVal11.Data;
			int iLen = pByteGet.Length;
			Debug.Assert(iLen == 6);
			for(int i = 0; i < pByteGet.Length; i ++)
			{
				Debug.Assert(pByteGet[i] == pByteSet[i]);
			}

			CLOBValue pDVal12 = new CLOBValue();
			Debug.Assert(pDVal12.IsNull());
			pStr = pDVal12.ToString();
			Debug.Assert(pStr == "{NULL}");
			pByteSet = (new ASCIIEncoding()).GetBytes("98765");
			pDVal12.Data = pByteSet;
			Debug.Assert(!pDVal12.IsNull());
			Debug.Assert(pDVal12.DataType == DataType.DataType_CLOB);
			pByteGet = pDVal12.Data;
			iLen = pByteGet.Length;
			Debug.Assert(iLen == 5);
			for (int i  = 0; i < pByteGet.Length; i ++)
			{
				Debug.Assert(pByteGet[i] == pByteSet[i]);
			}

			// test the setters and getters for a null value
			StringValue pDVal13 = new StringValue();
			Debug.Assert(pDVal13.IsNull());
			pStr = pDVal13.ToString();
			Debug.Assert(pStr == "NULL");
			pDVal13.SetNull();
			pStr = pDVal13.ToString();
			Debug.Assert(pStr == "NULL");;
			Debug.Assert(pDVal13.IsNull());
			Debug.Assert(pDVal13.DataType == DataType.DataType_String);

			//Console.WriteLine("finish mgExpressionTest.testDataValue");
			
		}

		public void testFunction()
		{
			Console.WriteLine("start mgExpressionTest.testFunction");

			Expression [] apExp = new Expression[3];

			// create some data for the test.
			apExp[0] = new Parameter("TestParam");
			apExp[1] = new Identifier("TestIdent");;
			apExp[2] = new DoubleValue(1.234);

			// test the constructor
			Function pFunc = new Function("Name", apExp);
			Debug.Assert(pFunc.Name == "Name");
			ExpressionCollection pColl = pFunc.Arguments;
			Debug.Assert(pColl.Count == 3);

		    // test the setters and getters
			pFunc.Name = "Name2";
			Debug.Assert(pFunc.Name == "Name2");
			
			//Console.WriteLine("finish mgExpressionTest.testFunction");
			
		}

		public void testGeometryValue()
		{
			Console.WriteLine("start mgExpressionTest.testGeometryValue");

			// test the constructor			
			byte[] pFGF1 = (new ASCIIEncoding()).GetBytes("testing123");
			GeometryValue pGeom = new GeometryValue(pFGF1);

			byte[] pByte = pGeom.Geometry;
			int iLen = pByte.Length;
			Debug.Assert(iLen == 10);
			for(int i = 0; i < pByte.Length; i ++)
			{
				Debug.Assert(pByte[i] == pFGF1[i]);
			}
			pByte = null;
			Debug.Assert(!pGeom.IsNull());

			// test the setters and getters with null values
			pGeom.SetNull();
			Debug.Assert(pGeom.IsNull());

			// test the setters and getters
			byte[] pFGF2 = (new ASCIIEncoding()).GetBytes("Test456");
			pGeom.Geometry = pFGF2;

			pByte = pGeom.Geometry;
			Debug.Assert(pByte.Length == 7);
			for(int i = 0; i < pByte.Length; i ++)
			{
				Debug.Assert(pByte[i] == pFGF2[i]);
			}
			pByte = null;
			Debug.Assert(!pGeom.IsNull());

			//Console.WriteLine("finish mgExpressionTest.testGeometryValue");
			
		}

		public void testIdentifier()
		{
			Console.WriteLine("start mgExpressionTest.testIdentifier");

			// test the constructor
			Identifier pIdent = new Identifier("This.Is.A.Test");
			Debug.Assert(pIdent.Name == "Test");
			string [] apScope = pIdent.Scope;
			Debug.Assert(apScope.Length == 3);
			Debug.Assert(apScope[0] == "This");
			Debug.Assert(apScope[1] == "Is");
			Debug.Assert(apScope[2] == "A");
			Debug.Assert(pIdent.Text == "This.Is.A.Test");

			// test the setters and getters
			pIdent.Text = "Testing";
			Debug.Assert(pIdent.Name == "Testing");
			apScope = pIdent.Scope;
			Debug.Assert(apScope.Length == 0);
			Debug.Assert(pIdent.Text == "Testing");

			// test the setters and getters with null values
			pIdent.Text = string.Empty;  
			Debug.Assert(pIdent.Name == string.Empty); 
			Debug.Assert(pIdent.Scope.Length == 0); 
			Debug.Assert(pIdent.Text == string.Empty);

			 // Test the schema part
			pIdent = new Identifier("Electric:Pole.Make.Id");
			Debug.Assert(pIdent.SchemaName == "Electric");
			pIdent.Text = "Pole.Make.Id";
			Debug.Assert(pIdent.SchemaName.Length == 0);

			pIdent.Text = "Telco:Cable.Make.Id";
			Debug.Assert(pIdent.SchemaName == "Telco");
			Debug.Assert(pIdent.Name == "Id");
			apScope = pIdent.Scope;
			Debug.Assert(apScope.Length == 2);
			Debug.Assert(apScope[0] == "Cable");
			Debug.Assert(apScope[1] == "Make");

			pIdent.Text = "Telco:Cable";
			Debug.Assert(pIdent.SchemaName == "Telco");
			Debug.Assert(pIdent.Name == "Cable");

			pIdent.Text = string.Empty;  
			Debug.Assert(pIdent.SchemaName.Length == 0); 

			//Console.WriteLine("finish mgExpressionTest.testIdentifier");
			
		}

		public void testComputedIdentifier()
		{
			Console.WriteLine("start mgExpressionTest.testComputedIdentifier");

			Identifier pWidth = new Identifier("Width");
			Identifier pLength = new Identifier("Length");
			BinaryExpression pArea = new BinaryExpression(pWidth, BinaryOperations.BinaryOperations_Multiply, pLength);
			ComputedIdentifier pCompIdent = new ComputedIdentifier("Area", pArea);
			Debug.Assert(pCompIdent.Name == "Area");
			Debug.Assert(pCompIdent.ToString() == "( Width*Length ) AS Area");

			//Console.WriteLine("finish mgExpressionTest.testComputedIdentifier");
			
		}

		public void testParameter()
		{
			Console.WriteLine("start mgExpressionTest.testParameter");

			// test the constructor
			Parameter pParam = new Parameter("ParamTest");
			Debug.Assert(pParam.Name == "ParamTest");

			// test the setters and getters with null values
			pParam.Name = string.Empty; 
			Debug.Assert(pParam.Name == string.Empty);

			// test the setters and getters
			pParam.Name = "TestAgain";
			Debug.Assert(pParam.Name == "TestAgain");

			//Console.WriteLine("finish mgExpressionTest.testParameter");
			
		}

		public void testUnaryExpression()
		{
			Console.WriteLine("start mgExpressionTest.testUnaryExpression");

			// create some data for the test.
			DataValue pDVal1 = new BooleanValue(true);
			DataValue pDVal2 = new Int16Value(123);

			// test the constructor
			UnaryExpression pExpr = new UnaryExpression(UnaryOperations.UnaryOperations_Negate, pDVal1);
			Debug.Assert(pExpr.Operation == UnaryOperations.UnaryOperations_Negate);
			Expression pExprTest = pExpr.Expressions;
			Debug.Assert(pExprTest == pDVal1);

			// test the setters and getters with nulls
			pExpr.Expressions = null;
			Debug.Assert(pExpr.Expressions == null);

			// test the setters and getters
			pExpr.Expressions = pDVal2;
			pExprTest = pExpr.Expressions;
			Debug.Assert(pExprTest == pDVal2);
			
			//Console.WriteLine("finish  mgExpressionTest.testUnaryExpression");
			
		}

        public void testDataValueConvert()
        {
            try {
                bool[] normBool = { true, false };
                String[] expNormBool = { "TRUE", "FALSE" };
                String[] expNormBoolNum = { "1", "0" };
                String[] expNormBoolStr = { "'TRUE'", "'FALSE'" };

                NormalCase<BooleanValue, bool>(2, normBool, expNormBool, DataType.DataType_Boolean);
                IncompatibleCase<BooleanValue, bool>(normBool[0], DataType.DataType_DateTime);
                NormalCase<BooleanValue, bool>(2, normBool, expNormBoolNum, DataType.DataType_Byte);
                NormalCase<BooleanValue, bool>(2, normBool, expNormBoolNum, DataType.DataType_Decimal);
                NormalCase<BooleanValue, bool>(2, normBool, expNormBoolNum, DataType.DataType_Double);
                NormalCase<BooleanValue, bool>(2, normBool, expNormBoolNum, DataType.DataType_Int16);
                NormalCase<BooleanValue, bool>(2, normBool, expNormBoolNum, DataType.DataType_Int32);
                NormalCase<BooleanValue, bool>(2, normBool, expNormBoolNum, DataType.DataType_Int64);
                NormalCase<BooleanValue, bool>(2, normBool, expNormBoolNum, DataType.DataType_Single);
                NormalCase<BooleanValue, bool>(2, normBool, expNormBoolStr, DataType.DataType_String);
                NullCases<BooleanValue>();

                byte[] normByte = { 5, 0, 128, 255, 254 };
                String[] expNormByte = { "5", "0", "128", "255", "254" };
                String[] expNormByteStr = { "'5'", "'0'", "'128'", "'255'", "'254'" };
                byte[] truncByte = { 2, 100, 255 };
                String[] expTruncByte = { "TRUE", "TRUE", "TRUE" };
                byte[] boolByte = { 0, 1 };


                NormalToBoolCase<ByteValue, byte>(boolByte);
                IncompatibleCase<ByteValue, byte>(normByte[0], DataType.DataType_DateTime);
                NormalCase<ByteValue, byte>(5, normByte, expNormByte, DataType.DataType_Byte);
                NormalCase<ByteValue, byte>(5, normByte, expNormByte, DataType.DataType_Decimal);
                NormalCase<ByteValue, byte>(5, normByte, expNormByte, DataType.DataType_Double);
                NormalCase<ByteValue, byte>(5, normByte, expNormByte, DataType.DataType_Int16);
                NormalCase<ByteValue, byte>(5, normByte, expNormByte, DataType.DataType_Int32);
                NormalCase<ByteValue, byte>(5, normByte, expNormByte, DataType.DataType_Int64);
                NormalCase<ByteValue, byte>(5, normByte, expNormByte, DataType.DataType_Single);
                NormalCase<ByteValue, byte>(5, normByte, expNormByteStr, DataType.DataType_String);
                NullCases<ByteValue>();
                TruncateCase<ByteValue, byte>(0, 3, truncByte, expTruncByte, DataType.DataType_Boolean);

                DateTime[] normDateTime = new DateTime[1];
                normDateTime[0] = new DateTime(2008, 10, 30, 16, 0, 30);
                String[] expNormDateTime = { "TIMESTAMP '2008-10-30 16:00:30'" };
                String[] expNormDateTimeStr = { "'TIMESTAMP ''2008-10-30 16:00:30'''" };

                IncompatibleCase<DateTimeValue, DateTime>(normDateTime[0], DataType.DataType_Boolean);
                IncompatibleCase<DateTimeValue, DateTime>(normDateTime[0], DataType.DataType_Byte);
                NormalCase<DateTimeValue, DateTime>(1, normDateTime, expNormDateTime, DataType.DataType_DateTime);
                IncompatibleCase<DateTimeValue, DateTime>(normDateTime[0], DataType.DataType_Decimal);
                IncompatibleCase<DateTimeValue, DateTime>(normDateTime[0], DataType.DataType_Double);
                IncompatibleCase<DateTimeValue, DateTime>(normDateTime[0], DataType.DataType_Int16);
                IncompatibleCase<DateTimeValue, DateTime>(normDateTime[0], DataType.DataType_Int32);
                IncompatibleCase<DateTimeValue, DateTime>(normDateTime[0], DataType.DataType_Int64);
                IncompatibleCase<DateTimeValue, DateTime>(normDateTime[0], DataType.DataType_Single);
                NormalCase<DateTimeValue, DateTime>(1, normDateTime, expNormDateTimeStr, DataType.DataType_String);
                NullCases<DateTimeValue>();

      
                double[] doubleVals = { 1, 0, 128, 255, 254, -1, -32768, 2000, 256, 32767, -32769, 32768, 100000, -2147483648, 2147483647, -2147483649, 2147483648, -1000000000000, 1.892e101, -1.7e308, 1.7e308 };
                String[] expDoubleVals = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767", "-32769", "32768", "100000", "-2147483648", "2147483647", "-2147483649", "2147483648", "-1000000000000", 
                    "1.892e101", "-1.7e308", "1.7e308" };
                String[] expDoubleValsStr = { "'1'", "'0'", "'128'", "'255'", "'254'", "'-1'", "'-32768'", "'2000'", "'256'", "'32767'", "'-32769'", "'32768'", "'100000'", "'-2147483648'", "'2147483647'", "'-2147483649'", "'2147483648'", "'-1000000000000'", 
                    "'-9.22337203685478e+018'", "'9.22337203685477e+018'", "'-9.22337203685478e+018'", "'9.22337203685478e+018'", 
                    "'1.892e101'", "'-1.7e308'", "'1.7e308'" };
                String[] expDoubleValsBool = { "TRUE", "FALSE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE" };
                String[] expDoubleValsByte = { "1", "0", "128", "255", "254", "0", "0", "255", "255", "255", "0", "255", "255", "0", "255", "0", "255", "0", "0", "255", "0", "255", "255", "0", "255" };
                String[] expDoubleValsInt16 = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767", "-32768", "32767", "32767", "-32768", "32767", "-32768", "32767", "-32768", "-32768", "32767", "-32768", "32767", "32767", "-32768", "32767" };
                String[] expDoubleValsInt32 = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767", "-32769", "32768", "100000", "-2147483648", "2147483647", "-2147483648", "2147483647", "-2147483648", "-2147483648", "2147483647", "-2147483648", "2147483647", "2147483647", "-2147483648", "2147483647" };
                String[] expDoubleValsInt64 = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767", "-32769", "32768", "100000", "-2147483648", "2147483647", "-2147483648", "2147483647", "-2147483648", "-2147483648", "2147483647", "-9223372036854775808", "9223372036854775807", "9223372036854775807", "-9223372036854775808", "9223372036854775807" };
                String[] expDoubleValsSingle = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767", "-32769", "32768", "100000", "-2147483648", "2147483647", "-2147483648", "2147483647", "-2147483648", "-2147483648", "2147483647", "-9223372036854775808", "9223372036854775807", 
                    "3.4e+038", "-3.4e+038", "3.4e+038" };
                double[] doubleFractVals = { -5.8, -5.5, -5.3, -0.51, -0.5, -0.1, 0.2, 0.5, 0.51, 100.4, 101.5, 105.9 };
                String[] expDoubleFractVals = { "-5.8", "-5.5", "-5.3", "-0.51", "-0.5", "-0.1", "0.2", "0.5", "0.51", "100.4", "101.5", "105.9" };
                String[] expDoubleRoundVals = { "-6", "-6", "-5", "-1", "-1", "0", "0", "1", "1", "100", "102", "106" };
                String[] expDoubleFractValsStr = { "'-5.8'", "'-5.5'", "'-5.3'", "'-0.51'", "'-0.5'", "'-0.1'", "'0.2'", "'0.5'", "'0.51'", "'100.4'", "'101.5'", "'105.9'" };
                String[] expDoubleRoundValsBool = { "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE" };
                double[] doubleFractValsByte = { -0.5, -0.1, 255.2, 255.5 };
                String[] expDoubleFractValsByte = { "0", "0", "255", "255" };
                double[] doubleFractValsInt16 = { -32768.5, -32768.1, 32767.2, 32767.5 };
                String[] expDoubleFractValsInt16 = { "-32768", "-32768", "32767", "32767" };
                double[] doubleFractValsInt32 = { -2147483648.5, -2147483648.1, 2147483647.2, 2147483647.5 };
                String[] expDoubleFractValsInt32 = { "-2147483648", "-2147483648", "2147483647", "2147483647" };
                double[] boolDouble = { 0, 1 };

            
                NormalToBoolCase<DecimalValue, double>(boolDouble);
                NormalCase<DecimalValue, double>(5, doubleVals, expDoubleVals, DataType.DataType_Byte);
                IncompatibleCase<DecimalValue, double>(doubleVals[7], DataType.DataType_DateTime);
                NormalCase<DecimalValue, double>(21, doubleVals, expDoubleVals, DataType.DataType_Decimal);
                NormalCase<DecimalValue, double>(21, doubleVals, expDoubleVals, DataType.DataType_Double);
                NormalCase<DecimalValue, double>(10, doubleVals, expDoubleVals, DataType.DataType_Int16);
                NormalCase<DecimalValue, double>(15, doubleVals, expDoubleVals, DataType.DataType_Int32);
                NormalCase<DecimalValue, double>(18, doubleVals, expDoubleVals, DataType.DataType_Int64);
                NormalCase<DecimalValue, double>(13, doubleVals, expDoubleVals, DataType.DataType_Single);
                NormalCase<DecimalValue, double>(18, doubleVals, expDoubleValsStr, DataType.DataType_String);
                NullCases<DecimalValue>();
                TruncateCase<DecimalValue, double>(0, 12, doubleFractVals, expDoubleRoundValsBool, DataType.DataType_Boolean);
                NormalCase<DecimalValue, double>(12, doubleFractVals, expDoubleFractVals, DataType.DataType_Decimal);
                NormalCase<DecimalValue, double>(12, doubleFractVals, expDoubleFractVals, DataType.DataType_Double);
                ShiftCase<DecimalValue, double>(12, doubleFractVals, expDoubleRoundVals, DataType.DataType_Int16);
                ShiftCase<DecimalValue, double>(12, doubleFractVals, expDoubleRoundVals, DataType.DataType_Int32);
                ShiftCase<DecimalValue, double>(12, doubleFractVals, expDoubleRoundVals, DataType.DataType_Int64);
                ShiftCase<DecimalValue, double>(1, doubleFractVals, expDoubleFractVals, DataType.DataType_Single);
                NormalCase<DecimalValue, double>(12, doubleFractVals, expDoubleFractValsStr, DataType.DataType_String);

                NormalToBoolCase<DoubleValue, double>(boolDouble);
                NormalCase<DoubleValue, double>(5, doubleVals, expDoubleVals, DataType.DataType_Byte);
                IncompatibleCase<DoubleValue, double>(doubleVals[7], DataType.DataType_DateTime);
                NormalCase<DoubleValue, double>(21, doubleVals, expDoubleVals, DataType.DataType_Decimal);
                NormalCase<DoubleValue, double>(21, doubleVals, expDoubleVals, DataType.DataType_Double);
                NormalCase<DoubleValue, double>(10, doubleVals, expDoubleVals, DataType.DataType_Int16);
                NormalCase<DoubleValue, double>(15, doubleVals, expDoubleVals, DataType.DataType_Int32);
                NormalCase<DoubleValue, double>(18, doubleVals, expDoubleVals, DataType.DataType_Int64);
                NormalCase<DoubleValue, double>(13, doubleVals, expDoubleVals, DataType.DataType_Single);
                NormalCase<DoubleValue, double>(18, doubleVals, expDoubleValsStr, DataType.DataType_String);
                NullCases<DoubleValue>();
                TruncateCase<DoubleValue, double>(2, 19, doubleVals, expDoubleValsBool, DataType.DataType_Boolean);
                TruncateCase<DoubleValue, double>(5, 16, doubleVals, expDoubleValsByte, DataType.DataType_Byte);
                TruncateCase<DoubleValue, double>(10, 11, doubleVals, expDoubleValsInt16, DataType.DataType_Int16);
                TruncateCase<DoubleValue, double>(15, 6, doubleVals, expDoubleValsInt32, DataType.DataType_Int32);
                TruncateCase<DoubleValue, double>(20, 1, doubleVals, expDoubleValsInt64, DataType.DataType_Int64);
                TruncateCase<DoubleValue, double>(2, 19, doubleVals, expDoubleValsBool, DataType.DataType_Boolean);
                TruncateCase<DoubleValue, double>(5, 16, doubleVals, expDoubleValsByte, DataType.DataType_Byte);
                TruncateCase<DoubleValue, double>(10, 11, doubleVals, expDoubleValsInt16, DataType.DataType_Int16);
                TruncateCase<DoubleValue, double>(15, 6, doubleVals, expDoubleValsInt32, DataType.DataType_Int32);
                TruncateCase<DoubleValue, double>(20, 1, doubleVals, expDoubleValsInt64, DataType.DataType_Int64);
                TruncateCase<DoubleValue, double>(0, 12, doubleFractVals, expDoubleRoundValsBool, DataType.DataType_Boolean);
                NormalCase<DoubleValue, double>(12, doubleFractVals, expDoubleFractVals, DataType.DataType_Decimal);
                NormalCase<DoubleValue, double>(12, doubleFractVals, expDoubleFractVals, DataType.DataType_Double);
                ShiftCase<DoubleValue, double>(12, doubleFractVals, expDoubleRoundVals, DataType.DataType_Int16);
                ShiftCase<DoubleValue, double>(12, doubleFractVals, expDoubleRoundVals, DataType.DataType_Int32);
                ShiftCase<DoubleValue, double>(12, doubleFractVals, expDoubleRoundVals, DataType.DataType_Int64);
                ShiftCase<DoubleValue, double>(1, doubleFractVals, expDoubleFractVals, DataType.DataType_Single);
                NormalCase<DoubleValue, double>(12, doubleFractVals, expDoubleFractValsStr, DataType.DataType_String);

                TruncateCase<DecimalValue, double>(0, 4, doubleFractValsByte, expDoubleFractValsByte, DataType.DataType_Byte);
                TruncateCase<DecimalValue, double>(0, 4, doubleFractValsInt16, expDoubleFractValsInt16, DataType.DataType_Int16);
                TruncateCase<DecimalValue, double>(0, 4, doubleFractValsInt32, expDoubleFractValsInt32, DataType.DataType_Int32);

                Int16[] int16Vals = { 1, 0, 128, 255, 254, -1, -32768, 2000, 256, 32767 };
                String[] expInt16Vals = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767" };
                String[] expInt16ValsStr = { "'1'", "'0'", "'128'", "'255'", "'254'", "'-1'", "'-32768'", "'2000'", "'256'", "'32767'" };
                String[] expInt16ValsBool = { "TRUE", "FALSE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE" };
                String[] expInt16ValsByte = { "1", "0", "128", "255", "254", "0", "0", "255", "255", "255" };
                Int16[] boolInt16 = { 0, 1 };

                NormalToBoolCase<Int16Value, Int16>(boolInt16);
                NormalCase<Int16Value, Int16>(5, int16Vals, expInt16Vals, DataType.DataType_Byte);
                IncompatibleCase<Int16Value, Int16>(int16Vals[7], DataType.DataType_DateTime);
                NormalCase<Int16Value, Int16>(10, int16Vals, expInt16Vals, DataType.DataType_Decimal);
                NormalCase<Int16Value, Int16>(10, int16Vals, expInt16Vals, DataType.DataType_Double);
                NormalCase<Int16Value, Int16>(10, int16Vals, expInt16Vals, DataType.DataType_Int16);
                NormalCase<Int16Value, Int16>(10, int16Vals, expInt16Vals, DataType.DataType_Int32);
                NormalCase<Int16Value, Int16>(10, int16Vals, expInt16Vals, DataType.DataType_Int64);
                NormalCase<Int16Value, Int16>(10, int16Vals, expInt16Vals, DataType.DataType_Single);
                NormalCase<Int16Value, Int16>(10, int16Vals, expInt16ValsStr, DataType.DataType_String);
                NullCases<Int16Value>();
                TruncateCase<Int16Value, Int16>(2, 8, int16Vals, expInt16ValsBool, DataType.DataType_Boolean);
                TruncateCase<Int16Value, Int16>(5, 5, int16Vals, expInt16ValsByte, DataType.DataType_Byte);

                Int32[] int32Vals = { 1, 0, 128, 255, 254, -1, -32768, 2000, 256, 32767, -32769, 32768, 100000, -2147483647 - 1, 2147483647 };
                String[] expInt32Vals = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767", "-32769", "32768", "100000", "-2147483648", "2147483647" };
                String[] expInt32ValsStr = { "'1'", "'0'", "'128'", "'255'", "'254'", "'-1'", "'-32768'", "'2000'", "'256'", "'32767'", "'-32769'", "'32768'", "'100000'", "'-2147483648'", "'2147483647'" };
                String[] expInt32ValsBool = { "TRUE", "FALSE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE" };
                String[] expInt32ValsByte = { "1", "0", "128", "255", "254", "0", "0", "255", "255", "255", "0", "255", "255", "0", "255" };
                String[] expInt32ValsInt16 = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767", "-32768", "32767", "32767", "-32768", "32767" };
                Int32[] int32ShiftVals = { -2147483647, 2147483391, 2147483647 };
                String[] expInt32ShiftValsSingle = { 
                    "-2.14748e+009", "2.14748e+009", "2.14748e+009" };
                Int32[] boolInt32 = { 0, 1 };

                NormalToBoolCase<Int32Value, Int32>(boolInt32);
                NormalCase<Int32Value, Int32>(5, int32Vals, expInt32Vals, DataType.DataType_Byte);
                IncompatibleCase<Int32Value, Int32>(int32Vals[7], DataType.DataType_DateTime);
                NormalCase<Int32Value, Int32>(15, int32Vals, expInt32Vals, DataType.DataType_Decimal);
                NormalCase<Int32Value, Int32>(15, int32Vals, expInt32Vals, DataType.DataType_Double);
                NormalCase<Int32Value, Int32>(10, int32Vals, expInt32Vals, DataType.DataType_Int16);
                NormalCase<Int32Value, Int32>(15, int32Vals, expInt32Vals, DataType.DataType_Int32);
                NormalCase<Int32Value, Int32>(15, int32Vals, expInt32Vals, DataType.DataType_Int64);
                NormalCase<Int32Value, Int32>(13, int32Vals, expInt32Vals, DataType.DataType_Single);
                NormalCase<Int32Value, Int32>(15, int32Vals, expInt32ValsStr, DataType.DataType_String);
                NullCases<Int32Value>();
                TruncateCase<Int32Value, Int32>(2, 13, int32Vals, expInt32ValsBool, DataType.DataType_Boolean);
                TruncateCase<Int32Value, Int32>(5, 10, int32Vals, expInt32ValsByte, DataType.DataType_Byte);
                TruncateCase<Int32Value, Int32>(10, 5, int32Vals, expInt32ValsInt16, DataType.DataType_Int16);
                ShiftCase<Int32Value, Int32>(3, int32ShiftVals, expInt32ShiftValsSingle, DataType.DataType_Single);

                Int64[] int64Vals = { 1, 0, 128, 255, 254, -1, -32768, 2000, 256, 32767, -32769, 32768, 100000, -2147483648, 2147483647, -2147483649, 2147483648, -1000000000000, -9223372036854775807 - 1, 9223372036854775807 };
                String[] expInt64Vals = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767", "-32769", "32768", "100000", "-2147483648", "2147483647", "-2147483649", "2147483648", "-1000000000000", "-9223372036854775808", "9223372036854775807" };
                String[] expInt64ValsStr = { "'1'", "'0'", "'128'", "'255'", "'254'", "'-1'", "'-32768'", "'2000'", "'256'", "'32767'", "'-32769'", "'32768'", "'100000'", "'-2147483648'", "'2147483647'", "'-2147483649'", "'2147483648'", "'-1000000000000'", "'-9223372036854775808'", "'9223372036854775807'" };
                String[] expInt64ValsBool = { "TRUE", "FALSE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE" };
                String[] expInt64ValsByte = { "1", "0", "128", "255", "254", "0", "0", "255", "255", "255", "0", "255", "255", "0", "255", "0", "255", "0", "0", "255" };
                String[] expInt64ValsInt16 = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767", "-32768", "32767", "32767", "-32768", "32767", "-32768", "32767", "-32768", "-32768", "32767" };
                String[] expInt64ValsInt32 = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767", "-32769", "32768", "100000", "-2147483648", "2147483647", "-2147483648", "2147483647", "-2147483648", "-2147483648", "2147483647" };
                Int64[] int64ShiftVals = { - 9223372036854775807, 9223372036854775807 - 4096, 9223372036854775807 };
                String[] expInt64ShiftValsSingle = { "-9.22337e+018", "9.22337e+018", "9.22337e+018" };
                String[] expInt64ShiftValsDouble = { "-9.22337203685478e+018", "9.22337203685477e+018", "9.22337203685478e+018" };
                Int64[] boolInt64 = { 0, 1 };

                NormalToBoolCase<Int64Value, Int64>(boolInt64);
                NormalCase<Int64Value, Int64>(5, int64Vals, expInt64Vals, DataType.DataType_Byte);
                IncompatibleCase<Int64Value, Int64>(int64Vals[7], DataType.DataType_DateTime);
                NormalCase<Int64Value, Int64>(18, int64Vals, expInt64Vals, DataType.DataType_Decimal);
                NormalCase<Int64Value, Int64>(18, int64Vals, expInt64Vals, DataType.DataType_Double);
                NormalCase<Int64Value, Int64>(10, int64Vals, expInt64Vals, DataType.DataType_Int16);
                NormalCase<Int64Value, Int64>(15, int64Vals, expInt64Vals, DataType.DataType_Int32);
                NormalCase<Int64Value, Int64>(20, int64Vals, expInt64Vals, DataType.DataType_Int64);
                NormalCase<Int64Value, Int64>(13, int64Vals, expInt64Vals, DataType.DataType_Single);
                NormalCase<Int64Value, Int64>(20, int64Vals, expInt64ValsStr, DataType.DataType_String);
                NullCases<Int64Value>();
                TruncateCase<Int64Value, Int64>(2, 18, int64Vals, expInt64ValsBool, DataType.DataType_Boolean);
                TruncateCase<Int64Value, Int64>(5, 15, int64Vals, expInt64ValsByte, DataType.DataType_Byte);
                TruncateCase<Int64Value, Int64>(10, 10, int64Vals, expInt64ValsInt16, DataType.DataType_Int16);
                TruncateCase<Int64Value, Int64>(15, 5, int64Vals, expInt64ValsInt32, DataType.DataType_Int32);
                ShiftCase<Int64Value, Int64>(3, int64ShiftVals, expInt64ShiftValsSingle, DataType.DataType_Single);
                ShiftCase<Int64Value, Int64>(3, int64ShiftVals, expInt64ShiftValsDouble, DataType.DataType_Double);

         
                float[] singleVals = { 1, 0, 128, 255, 254, -1, -32768, 2000, 256, 32767, -32769, 32768, 100000, -2147483648, -999999995904 };
                String[] expSingleVals = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767", "-32769", "32768", "100000", "-2147483648", "-999999995904"};
                String[] expSingleValsStr = { "'1'", "'0'", "'128'", "'255'", "'254'", "'-1'", "'-32768'", "'2000'", "'256'", "'32767'", "'-32769'", "'32768'", "'100000'", 
                    "'-2.14748e+009'", "'2.14748e+009'", "'-2.14748e+009'", "'2.14748e+009'", "'-999999995904'", "'-9.22337203685478e+018'", "'9.22337203685477e+018'", 
                    "todo", "todo", "todo", "todo", "todo" };
                String[] expSingleValsBool = { "TRUE", "FALSE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE" };
                String[] expSingleValsByte = { "1", "0", "128", "255", "254", "0", "0", "255", "255", "255", "0", "255", "255", "0", "255", "0", "255", "0", "0", "255", "0", "255", "0", "0", "255" };
                String[] expSingleValsInt16 = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767", "-32768", "32767", "32767", "-32768", "32767", "-32768", "32767", "-32768", "-32768", "32767", "-32768", "32767", "-32768", "-32768", "32767" };
                String[] expSingleValsInt32 = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767", "-32769", "32768", "100000", "-2147483648", "2147483647", "-2147483648", "2147483647", "-2147483648", "-2147483648", "2147483647", "-2147483648", "2147483647", "-2147483648", "-2147483648", "2147483647" };
                String[] expSingleValsInt64 = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767", "-32769", "32768", "100000", "-2147483648", "2147483647", "-2147483904", "-2147483904", "-2147483648", "-2147483648", "2147483647", "-9223372036854775808", "9223372036854775807", "-9223372036854775808", "-9223372036854775808", "9223372036854775807" };
                float[] singleFractVals = { (float)-5.8, (float)-5.5, (float)-5.3, (float)-0.51, (float)-0.5, (float)-0.1, (float)0.2, (float)0.5, (float)0.51, (float)100.4, (float)101.5, (float)105.9 };
                String[] expSingleFractVals = { "-5.8", "-5.5", "-5.3", "-0.51", "-0.5", "-0.1", "0.2", "0.5", "0.51", "100.4", "101.5", "105.9" };
                String[] expSingleRoundVals = { "-6", "-6", "-5", "-1", "-1", "0", "0", "1", "1", "100", "102", "106" };
                String[] expSingleFractValsStr = { "'-5.8'", "'-5.5'", "'-5.3'", "'-0.51'", "'-0.5'", "'-0.1'", "'0.2'", "'0.5'", "'0.51'", "'100.4'", "'101.5'", "'105.9'" };
                String[] expSingleRoundValsBool = { "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE" };
                float[] singleFractValsByte = { (float)-0.5, (float)-0.1, (float)255.2, (float)255.5 };
                String[] expSingleFractValsByte = { "0", "0", "255", "255" };
                float[] singleFractValsInt16 = { (float)-32768.5, (float)-32768.1, (float)32767.2, (float)32767.5 };
                String[] expSingleFractValsInt16 = { "-32768", "-32768", "32767", "32767" };
                float[] boolFloat = { 0, 1 };

                NormalToBoolCase<SingleValue, float>(boolFloat);
                NormalCase<SingleValue, float>(5, singleVals, expSingleVals, DataType.DataType_Byte);
                IncompatibleCase<SingleValue, float>(singleVals[7], DataType.DataType_DateTime);
                NormalCase<SingleValue, float>(15, singleVals, expSingleVals, DataType.DataType_Decimal);
                NormalCase<SingleValue, float>(15, singleVals, expSingleVals, DataType.DataType_Double);
                NormalCase<SingleValue, float>(10, singleVals, expSingleVals, DataType.DataType_Int16);
                NormalCase<SingleValue, float>(13, singleVals, expSingleVals, DataType.DataType_Int32);
                NormalCase<SingleValue, float>(15, singleVals, expSingleVals, DataType.DataType_Int64);
                NormalCase<SingleValue, float>(15, singleVals, expSingleVals, DataType.DataType_Single);
                NormalCase<SingleValue, float>(13, singleVals, expSingleValsStr, DataType.DataType_String);
                NullCases<SingleValue>();
                TruncateCase<SingleValue, float>(2, 13, singleVals, expSingleValsBool, DataType.DataType_Boolean);
                TruncateCase<SingleValue, float>(5, 10, singleVals, expSingleValsByte, DataType.DataType_Byte);
                TruncateCase<SingleValue, float>(10, 5, singleVals, expSingleValsInt16, DataType.DataType_Int16);
                TruncateCase<SingleValue, float>(0, 12, singleFractVals, expSingleRoundValsBool, DataType.DataType_Boolean);
                NormalCase<SingleValue, float>(12, singleFractVals, expSingleFractVals, DataType.DataType_Decimal);
                NormalCase<SingleValue, float>(12, singleFractVals, expSingleFractVals, DataType.DataType_Double);
                ShiftCase<SingleValue, float>(12, singleFractVals, expSingleRoundVals, DataType.DataType_Int16);
                ShiftCase<SingleValue, float>(12, singleFractVals, expSingleRoundVals, DataType.DataType_Int32);
                
                ShiftCase<SingleValue, float>(12, singleFractVals, expSingleRoundVals, DataType.DataType_Int64);
                NormalCase<SingleValue, float>(12, singleFractVals, expSingleVals, DataType.DataType_Single);
                NormalCase<SingleValue, float>(12, singleFractVals, expSingleFractValsStr, DataType.DataType_String);
                TruncateCase<DecimalValue, float>(0, 4, singleFractValsByte, expSingleFractValsByte, DataType.DataType_Byte);
                TruncateCase<DecimalValue, float>(0, 4, singleFractValsInt16, expSingleFractValsInt16, DataType.DataType_Int16);

                String[] stringVals = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767", "-32769", "32768", "100000", "-2147483648", "2147483647", "-2147483649", "2147483648", "-1000000000000", "-9223372036854775808", "9223372036854775807" };
                String[] stringValsDt = { "TIMESTAMP '2008-10-30 16:00:30'" };
                String[] expStringValsBool = { "TRUE", "FALSE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE", "TRUE" };
                String[] expStringValsStr = { "'1'", "'0'", "'128'", "'255'", "'254'", "'-1'", "'-32768'", "'2000'", "'256'", "'32767'", "'-32769'", "'32768'", "'100000'", "'-2147483648'", "'2147483647'", "'-2147483649'", "'2147483648'", "'-1000000000000'", "'-9223372036854775808'", "'9223372036854775807'" };
                String[] expStringValsByte = { "1", "0", "128", "255", "254", "0", "0", "255", "255", "255", "0", "255", "255", "0", "255", "0", "255", "0", "0", "255", "0", "255", "0", "0", "255" };
                String[] expStringValsInt16 = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767", "-32768", "32767", "32767", "-32768", "32767", "-32768", "32767", "-32768", "-32768", "32767", "-32768", "32767", "-32768", "-32768", "32767" };
                String[] expStringValsInt32 = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767", "-32769", "32768", "100000", "-2147483648", "2147483647", "-2147483648", "2147483647", "-2147483648", "-2147483648", "2147483647", "-2147483648", "2147483647", "-2147483648", "-2147483648", "2147483647" };
                String[] expStringValsInt64 = { "1", "0", "128", "255", "254", "-1", "-32768", "2000", "256", "32767", "-32769", "32768", "100000", "-2147483648", "2147483647", "-2147483904", "-2147483904", "-2147483648", "-2147483648", "2147483647", "-9223372036854775808", "9223372036854775807", "-9223372036854775808", "-9223372036854775808", "9223372036854775807" };
                String[] stringFractVals = { "8.125", "15.5" };
                String[] expStringRoundVals = { "8", "16" };
                String[] expStringRoundValsBool = { "TRUE", "TRUE" };

                NormalCase<StringValue,String>(2, stringVals, expStringValsBool, DataType.DataType_Boolean);
                NormalCase<StringValue, String>(2, expStringValsBool, expStringValsBool, DataType.DataType_Boolean);
                NormalCase<StringValue, String>(5, stringVals, stringVals, DataType.DataType_Byte);
                NormalCase<StringValue, String>(1, stringValsDt, stringValsDt, DataType.DataType_DateTime);
                IncompatibleCase<StringValue, String>(stringVals[7], DataType.DataType_DateTime);
                NormalCase<StringValue, String>(18, stringVals, stringVals, DataType.DataType_Decimal);
                NormalCase<StringValue, String>(18, stringVals, stringVals, DataType.DataType_Double);
                NormalCase<StringValue, String>(10, stringVals, stringVals, DataType.DataType_Int16);
                NormalCase<StringValue, String>(15, stringVals, stringVals, DataType.DataType_Int32);
                NormalCase<StringValue, String>(20, stringVals, stringVals, DataType.DataType_Int64);
                NormalCase<StringValue, String>(13, stringVals, stringVals, DataType.DataType_Single);
                NormalCase<StringValue, String>(20, stringVals, expStringValsStr, DataType.DataType_String);
                NullCases<StringValue>();
                IncompatibleCase<StringValue, String>("NEITHER", DataType.DataType_Boolean);
                IncompatibleCase<StringValue, String>("NEITHER", DataType.DataType_Byte);
                IncompatibleCase<StringValue, String>("NEITHER", DataType.DataType_Byte);
                IncompatibleCase<StringValue, String>("8 + 9", DataType.DataType_Decimal);
                IncompatibleCase<StringValue, String>("NEITHER", DataType.DataType_Double);
                IncompatibleCase<StringValue, String>("NEITHER", DataType.DataType_Int16);
                IncompatibleCase<StringValue, String>("NEITHER", DataType.DataType_Int32);
                IncompatibleCase<StringValue, String>("NEITHER", DataType.DataType_Int64);
                IncompatibleCase<StringValue, String>("NEITHER", DataType.DataType_Single);
                TruncateCase<StringValue, String>(2, 18, stringVals, expStringValsBool, DataType.DataType_Boolean);
                TruncateCase<StringValue, String>(5, 15, stringVals, expStringValsByte, DataType.DataType_Byte);
                TruncateCase<StringValue, String>(5, 15, stringVals, expStringValsByte, DataType.DataType_Byte);
                TruncateCase<StringValue, String>(10, 10, stringVals, expStringValsInt16, DataType.DataType_Int16);
                TruncateCase<StringValue, String>(15, 5, stringVals, expStringValsInt32, DataType.DataType_Int32);
                TruncateCase<StringValue, String>(20, 5, stringVals, expStringValsInt64, DataType.DataType_Int64);
                TruncateCase<StringValue, String>(0, 2, stringFractVals, expStringRoundValsBool, DataType.DataType_Boolean);
                ShiftCase<StringValue, String>(2, stringFractVals, expStringRoundVals, DataType.DataType_Byte);
                NormalCase<StringValue, String>(2, stringFractVals, stringFractVals, DataType.DataType_Decimal);
                NormalCase<StringValue, String>(2, stringFractVals, stringFractVals, DataType.DataType_Double);
                ShiftCase<StringValue, String>(2, stringFractVals, expStringRoundVals, DataType.DataType_Int16);
                ShiftCase<StringValue, String>(2, stringFractVals, expStringRoundVals, DataType.DataType_Int32);
                ShiftCase<StringValue, String>(2, stringFractVals, expStringRoundVals, DataType.DataType_Int64);
                NormalCase<StringValue, String>(2, stringFractVals, stringFractVals, DataType.DataType_Single);

                byte[][] lobBytes = new byte[1][];
                lobBytes[0] = new byte[4];
                lobBytes[0][0] = 0x61;
                lobBytes[0][1] = 0x62;
                lobBytes[0][2] = 0x63;
                lobBytes[0][3] = 0x64;

                String[] expLobVals = new String[1];
                expLobVals[0] = "abcd";
                NormalCase<BLOBValue, byte[]>(1, lobBytes, expLobVals, DataType.DataType_BLOB);
                IncompatibleCase<BLOBValue, byte[]>(lobBytes[0], DataType.DataType_Boolean);
                IncompatibleCase<BLOBValue, byte[]>(lobBytes[0], DataType.DataType_Byte);
                IncompatibleCase<BLOBValue, byte[]>(lobBytes[0], DataType.DataType_Decimal);
                IncompatibleCase<BLOBValue, byte[]>(lobBytes[0], DataType.DataType_Double);
                IncompatibleCase<BLOBValue, byte[]>(lobBytes[0], DataType.DataType_Int16);
                IncompatibleCase<BLOBValue, byte[]>(lobBytes[0], DataType.DataType_Int32);
                IncompatibleCase<BLOBValue, byte[]>(lobBytes[0], DataType.DataType_Int64);
                IncompatibleCase<BLOBValue, byte[]>(lobBytes[0], DataType.DataType_Single);

                NormalCase<CLOBValue, byte[]>(1, lobBytes, expLobVals, DataType.DataType_CLOB);
                IncompatibleCase<CLOBValue, byte[]>(lobBytes[0], DataType.DataType_Boolean);
                IncompatibleCase<CLOBValue, byte[]>(lobBytes[0], DataType.DataType_Byte);
                IncompatibleCase<CLOBValue, byte[]>(lobBytes[0], DataType.DataType_Decimal);
                IncompatibleCase<CLOBValue, byte[]>(lobBytes[0], DataType.DataType_Double);
                IncompatibleCase<CLOBValue, byte[]>(lobBytes[0], DataType.DataType_Int16);
                IncompatibleCase<CLOBValue, byte[]>(lobBytes[0], DataType.DataType_Int32);
                IncompatibleCase<CLOBValue, byte[]>(lobBytes[0], DataType.DataType_Int64);
                IncompatibleCase<CLOBValue, byte[]>(lobBytes[0], DataType.DataType_Single);
            }
            catch ( Exception ex )
            
            {
                System.Console.WriteLine(ex.Message);
            }
        }

        static void NormalCase<CI, TI>( int valCount, TI[] vals, String[] expected, DataType outType ) where CI : DataValue
        {
            int i = 0;

            for ( i = 0; i < valCount; i++ ) 
            {
                CI dv1 = NewDataValue<CI,TI>( vals[i] );
                NormalSubCase( dv1, expected[i], outType, false, false, false );
                NormalSubCase( dv1, expected[i], outType, false, false, true );
                NormalSubCase( dv1, expected[i], outType, false, true, false );
                NormalSubCase( dv1, expected[i], outType, false, true, true );
                NormalSubCase( dv1, expected[i], outType, true, false, false );
                NormalSubCase( dv1, expected[i], outType, true, false, true );
                NormalSubCase( dv1, expected[i], outType, true, true, false );
                NormalSubCase( dv1, expected[i], outType, true, true, true );
            }
        }

        static void NormalToBoolCase<CI,TI>  (TI[] vals) where CI : DataValue
        {
            String[] expectedVals = { "FALSE", "TRUE" };
            NormalCase<CI,TI>( 2, vals, expectedVals, DataType.DataType_Boolean );
        }

        static void NullCases<CI> ( ) where CI : DataValue, new()
        {
            NullCase<CI>(DataType.DataType_BLOB);
            NullCase<CI>(DataType.DataType_Boolean);
            NullCase<CI>(DataType.DataType_Byte);
            NullCase<CI>(DataType.DataType_CLOB);
            NullCase<CI>(DataType.DataType_DateTime);
            NullCase<CI>(DataType.DataType_Decimal);
            NullCase<CI>(DataType.DataType_Double);
            NullCase<CI>(DataType.DataType_Int16);
            NullCase<CI>(DataType.DataType_Int32);
            NullCase<CI>(DataType.DataType_Int64);
            NullCase<CI>(DataType.DataType_Single);
            NullCase<CI>(DataType.DataType_String);
        }

        static void NullCase<CI> ( DataType outType ) where CI : DataValue, new()
        {
            NullSubCase<CI>( outType, false, false, false );
            NullSubCase<CI>( outType, false, false, true );
            NullSubCase<CI>( outType, false, true, false );
            NullSubCase<CI>( outType, false, true, true );
            NullSubCase<CI>( outType, true, false, false );
            NullSubCase<CI>( outType, true, false, true );
            NullSubCase<CI>( outType, true, true, false );
            NullSubCase<CI>( outType, true, true, true );
        }

        static void NullSubCase<CI> ( DataType outType, Boolean nullIfIncompatible, Boolean shift, Boolean truncate ) where CI : DataValue, new()
        {
            CI dv = new CI();// NewDataValue<CI, DataValue>((DataValue)null);
            //NullSubSubCase<CI>( (CI) null, outType, nullIfIncompatible, shift, truncate );
            NullSubSubCase( dv, outType, nullIfIncompatible, shift, truncate );
        }

        static void IncompatibleCase<CI,TI> ( TI val, DataType outType ) where CI : DataValue
        {
            CI dv1 = NewDataValue<CI,TI>( val );
            IncompatibleSubCase( dv1, outType, false, false, false );
            IncompatibleSubCase( dv1, outType, false, false, true );
            IncompatibleSubCase( dv1, outType, false, true, false );
            IncompatibleSubCase( dv1, outType, false, true, true );
            IncompatibleSubCase( dv1, outType, true, false, false );
            IncompatibleSubCase( dv1, outType, true, false, true );
            IncompatibleSubCase( dv1, outType, true, true, false );
            IncompatibleSubCase( dv1, outType, true, true, true );
        }

        static void TruncateCase<CI,TI> ( int valStart, int valCount, TI[] vals, String[] expected, DataType outType ) where CI : DataValue
        {
            int i = 0;

            for ( i = valStart; i < valCount; i++ ) 
            {
                CI dv1 = NewDataValue<CI,TI> (vals[i]);
                TruncateSubCase( dv1, expected[i], outType, false, false, false );
                TruncateSubCase( dv1, expected[i], outType, false, false, true );
                TruncateSubCase( dv1, expected[i], outType, false, true, false );
                TruncateSubCase( dv1, expected[i], outType, false, true, true );
                TruncateSubCase( dv1, expected[i], outType, true, false, false );
                TruncateSubCase( dv1, expected[i], outType, true, false, true );
                TruncateSubCase( dv1, expected[i], outType, true, true, false );
                TruncateSubCase( dv1, expected[i], outType, true, true, true );
            }
        }

        static void ShiftCase<CI,TI> ( int valCount, TI[] vals, String[] expected, DataType outType ) where CI : DataValue
        {
            int i = 0;

            for ( i = 0; i < valCount; i++ ) 
            {
               CI dv1 = NewDataValue<CI,TI>( vals[i] );
                ShiftSubCase( dv1, expected[i], outType, false, false, false );
                ShiftSubCase( dv1, expected[i], outType, false, false, true );
                ShiftSubCase( dv1, expected[i], outType, false, true, false );
                ShiftSubCase( dv1, expected[i], outType, false, true, true );
                ShiftSubCase( dv1, expected[i], outType, true, false, false );
                ShiftSubCase( dv1, expected[i], outType, true, false, true );
                ShiftSubCase( dv1, expected[i], outType, true, true, false );
                ShiftSubCase( dv1, expected[i], outType, true, true, true );
            }
        }

        static CI NewDataValue<CI, TI>(TI oldVal) where CI : DataValue
        {
            Type tci = typeof(CI);
            Type[] types = new Type[1];
            types[0] = oldVal.GetType();
            System.Reflection.ConstructorInfo constr = tci.GetConstructor(types);

            object[] parms = new object[1];
            parms[0] = oldVal;
            CI dv = (CI) constr.Invoke( parms );

            return dv;
        }

        static DataValue CopyDataValue(DataType outType, DataValue src)
        {
            DataValue dest = null;

            switch (outType)
            {
                case DataType.DataType_BLOB:
                    dest = new BLOBValue(src);
                    break;
                case DataType.DataType_Boolean:
                    dest = new BooleanValue(src);
                    break;
                case DataType.DataType_Byte:
                    dest = new ByteValue(src);
                    break;
                case DataType.DataType_CLOB:
                    dest = new CLOBValue(src);
                    break;
                case DataType.DataType_DateTime:
                    dest = new DateTimeValue(src);
                    break;
                case DataType.DataType_Decimal:
                    dest = new DecimalValue(src);
                    break;
                case DataType.DataType_Double:
                    dest = new DoubleValue(src);
                    break;
                case DataType.DataType_Int16:
                    dest = new Int16Value(src);
                    break;
                case DataType.DataType_Int32:
                    dest = new Int32Value(src);
                    break;
                case DataType.DataType_Int64:
                    dest = new Int64Value(src);
                    break;
                case DataType.DataType_Single:
                    dest = new SingleValue(src);
                    break;
                case DataType.DataType_String:
                    dest = new StringValue(src);
                    break;
                default:
                    dest = new DataValue(outType, src);
                    break;
            }

            return dest;
        }

        static DataValue CopyDataValue(DataType outType, DataValue src, Boolean nullIfIncompatible, Boolean shift, Boolean truncate)
        {
            DataValue dest = null;

            switch (outType)
            {
                case DataType.DataType_BLOB:
                    dest = new BLOBValue(src, nullIfIncompatible, shift, truncate);
                    break;
                case DataType.DataType_Boolean:
                    dest = new BooleanValue(src, nullIfIncompatible, shift, truncate);
                    break;
                case DataType.DataType_Byte:
                    dest = new ByteValue(src, nullIfIncompatible, shift, truncate);
                    break;
                case DataType.DataType_CLOB:
                    dest = new CLOBValue(src, nullIfIncompatible, shift, truncate);
                    break;
                case DataType.DataType_DateTime:
                    dest = new DateTimeValue(src, nullIfIncompatible, shift, truncate);
                    break;
                case DataType.DataType_Decimal:
                    dest = new DecimalValue(src, nullIfIncompatible, shift, truncate);
                    break;
                case DataType.DataType_Double:
                    dest = new DoubleValue(src, nullIfIncompatible, shift, truncate);
                    break;
                case DataType.DataType_Int16:
                    dest = new Int16Value(src, nullIfIncompatible, shift, truncate);
                    break;
                case DataType.DataType_Int32:
                    dest = new Int32Value(src, nullIfIncompatible, shift, truncate);
                    break;
                case DataType.DataType_Int64:
                    dest = new Int64Value(src, nullIfIncompatible, shift, truncate);
                    break;
                case DataType.DataType_Single:
                    dest = new SingleValue(src, nullIfIncompatible, shift, truncate);
                    break;
                case DataType.DataType_String:
                    dest = new StringValue(src, nullIfIncompatible, shift, truncate);
                    break;
                default:
                    dest = new DataValue(outType, src, nullIfIncompatible, shift, truncate);
                    break;
            }

            return dest;
        }

        static void NormalSubCase<CI>(CI val, String expected, DataType outType, Boolean nullIfIncompatible, Boolean shift, Boolean truncate) where CI : DataValue
        {
            DataValue dv2 = CopyDataValue(
                outType,
                val,
                nullIfIncompatible,
                shift,
                truncate
            );

            NormalSubSubCase<CI>(val, dv2, expected, outType, nullIfIncompatible, shift, truncate);

            DataValue dv3 = new DataValue(outType, val, nullIfIncompatible, shift, truncate);

            DataValue dv4 = CopyDataValue(
                outType,
                dv2,
                false,
                false,
                false
            );

            NormalSubSubCase<CI>(val, dv4, expected, outType, nullIfIncompatible, shift, truncate);

            if (!nullIfIncompatible && shift && !truncate)
            {
                DataValue dv5 = CopyDataValue(
                    outType,
                    val
                );

                NormalSubSubCase<CI>(val, dv5, expected, outType, nullIfIncompatible, shift, truncate);

                DataValue dv6 = new DataValue(outType, val);

                DataValue dv7 = CopyDataValue(
                    outType,
                    dv2,
                    false,
                    false,
                    false
                );

                NormalSubSubCase<CI>(val, dv7, expected, outType, nullIfIncompatible, shift, truncate);

            }
        }

        static void NormalSubSubCase<CI>( CI val, DataValue dv2, String expected, DataType outType, Boolean nullIfIncompatible, Boolean shift, Boolean truncate ) where CI : DataValue
        {
            if ( (dv2 == null) || dv2.IsNull() ) 
                throw new Exception( "NormalSubCase:: result is NULL" );

            if ( dv2.DataType != outType ) 
                throw new Exception( "NormalSubCase:: result has wrong data type" );

            DataType valtype =  ((DataValue)val).DataType;

            if ( (valtype == DataType.DataType_Decimal || valtype == DataType.DataType_Double || valtype == DataType.DataType_Single) &&
                 (outType == DataType.DataType_Decimal || outType == DataType.DataType_Double || outType == DataType.DataType_Single)
            ) {
                DoubleValue valDbl = new DoubleValue(val);
                DoubleValue outDbl = new DoubleValue(dv2);


                if ( outDbl.Double != valDbl.Double )
                {
                    throw new Exception(
                      "NormalSubCase(double): got " + dv2.ToString() + ", expected " + val.ToString()
                    );
                }

            }
            else if ( (outType == DataType.DataType_BLOB) || (outType == DataType.DataType_CLOB) )
            {
                LOBValue bv2 = dv2 as LOBValue;

                String outString = "";

                for (int i = 0; i < bv2.Data.Length; i++)
                {
                    char ch = (char)bv2.Data[i];
                    outString += ch;
                }

                if ( outString != expected)
                {
                    throw new Exception(
                        "NormalSubCase: got " + outString + ", expected " + expected
                    );
                }

            }
            else
            {
                if ( expected != dv2.ToString())
                {
                    throw new Exception(
                        "NormalSubCase: got " + dv2.ToString() + ", expected " + expected
                    );
                }
            }
        }

        static void NullSubSubCase<CI>( CI val, DataType outType, Boolean nullIfIncompatible, Boolean shift, Boolean truncate ) where CI : DataValue
        {
            DataValue dv2 = CopyDataValue( 
                outType,
                val,
                nullIfIncompatible,
                shift,
                truncate
            );

            if ( dv2 == null ) 
                throw new Exception( "NullSubCase:: result is NULL pointer" );

            if ( dv2.DataType != outType ) 
                throw new Exception( "NullSubSubCase:: result has wrong data type" );
              
            if ( !(dv2.IsNull()) ) 
                throw new Exception( "NullSubCase:: result is not nul" );
        }

        static void IncompatibleSubCase<CI>( CI val, DataType outType, Boolean nullIfIncompatible, Boolean shift, Boolean truncate ) where CI : DataValue
        {
            bool success = false;
            DataValue dv2 = null;
            
            try {
                dv2 = CopyDataValue( 
                    outType,
                    val,
                    nullIfIncompatible,
                    shift,
                    truncate
                );

                success = true;
            }
            catch ( Exception ) 
            {
                if ( nullIfIncompatible ) 
                    throw;
            }

            if ( success )
            {
                if ( !nullIfIncompatible ) 
                    throw new Exception( "IncompatibleSubCase: conversion succeeded, expected exception" );

                if ( dv2 == null ) 
                    throw new Exception( "IncompatibleSubCase:: result is NULL pointer" );

                if ( !(dv2.IsNull()) ) 
                    throw new Exception( "IncompatibleSubCase: result is not nul" );
            }
        }

        static void TruncateSubCase<CI>( CI val, String expected, DataType outType, Boolean nullIfIncompatible, Boolean shift, Boolean truncate ) where CI : DataValue
        {
            bool success = false;
            DataValue dv2 = null;
            DataValue dv3 = null;
            int testCount = 2;
            if (!nullIfIncompatible && shift && !truncate)
                testCount = 4;

            for (int i = 0; i < testCount; i++)
            {
                try
                {
                    switch (i)
                    {
                        case 0:
                            dv2 = CopyDataValue(
                                outType,
                                val,
                                nullIfIncompatible,
                                shift,
                                truncate
                            );
                            break;

                        case 1:
                            dv3 = new DataValue(
                                outType,
                                val,
                                nullIfIncompatible,
                                shift,
                                truncate
                            );
                            dv2 = CopyDataValue(outType, dv3, false, true, false);
                            break;
                        case 2:
                            dv2 = CopyDataValue(
                                outType,
                                val
                            );
                            break;

                        case 3:
                            dv3 = new DataValue(
                                outType,
                                val
                            );
                            dv2 = CopyDataValue(outType, dv3, false, true, false);
                            break;
                    }
                    success = true;
                }
                catch (Exception)
                {
                    if (truncate || nullIfIncompatible)
                        throw;
                }


                if (success)
                {
                    if (dv2 == null)
                        throw new Exception("TruncateSubCase:: result is NULL pointer");

                    if (dv2.DataType != outType)
                        throw new Exception("TruncateSubCase:: result has wrong data type");

                    if (truncate)
                    {
                        if (dv2.IsNull())
                            throw new Exception("TruncateSubCase:: result is NUL");

                        if (expected != dv2.ToString())
                        {
                            throw new Exception(
                                "TruncateSubCase: got " + dv2.ToString() + ", expected " + expected
                            );
                        }
                    }
                    else
                    {
                        if (!(dv2.IsNull()))
                            throw new Exception("TruncateSubCase:: result is not NUL");
                    }
                }
            }
        }

        static void ShiftSubCase<CI>(CI val, String expected, DataType outType, Boolean nullIfIncompatible, Boolean shift, Boolean truncate) where CI : DataValue
        {
            bool success = false;
            DataValue dv2 = null;
            DataValue dv3 = null;
            int testCount = 2;
            if (!nullIfIncompatible && shift && !truncate)
                testCount = 4;

            for (int i = 0; i < testCount; i++)
            {
                try
                {
                    switch (i)
                    {
                        case 0:
                            dv2 = CopyDataValue(
                                outType,
                                val,
                                nullIfIncompatible,
                                shift,
                                truncate
                            );
                            break;

                        case 1:
                            dv3 = new DataValue(
                                outType,
                                val,
                                nullIfIncompatible,
                                shift,
                                truncate
                            );
                            dv2 = CopyDataValue(outType, dv3, false, true, false);  
                            break;
                        case 2:
                            dv2 = CopyDataValue(
                                outType,
                                val
                            );
                            break;

                        case 3:
                            dv3 = new DataValue(
                                outType,
                                val
                            );
                            dv2 = CopyDataValue(outType, dv3, false, true, false);
                            break;
                    }
                    success = true;
                }
                catch (Exception)
                {
                    if (shift || nullIfIncompatible)
                        throw;
                }


                if (success)
                {
                    if (dv2 == null)
                        throw new Exception("ShiftSubCase:: result is NULL pointer");

                    if (dv2.DataType != outType)
                        throw new Exception("ShiftSubCase:: result has wrong data type");

                    if (shift)
                    {
                        if (dv2.IsNull())
                            throw new Exception("ShiftSubCase:: result is NUL");

                        if (expected != dv2.ToString())
                        {
                            throw new Exception(
                                "ShiftSubCase: got " + dv2.ToString() + ", expected " + expected
                            );
                        }
                    }
                    else
                    {
                        if (!(dv2.IsNull()))
                            throw new Exception("ShiftSubCase:: result is not NUL");
                    }
                }
            }
        }
    }
}


