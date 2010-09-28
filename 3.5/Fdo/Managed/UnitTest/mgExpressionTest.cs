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
	}	
}

