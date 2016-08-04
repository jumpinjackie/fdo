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
using OSGeo.FDO.Filter;

namespace Fdo_Test
{
	/// <summary>
	/// Summary description for mgFilterTest.
	/// </summary>
	public class mgFilterTest
	{
		static string []  aszNames =
		{
			"One",
			"Two",
			"Three"
	};
		public mgFilterTest()
		{
			Console.WriteLine("start mgFilterTest");
		}

		public void testFilterTree()
		{
			Console.WriteLine("start mgFilterTest.testFilterTree");

			// expression classes needed for test
			DataValue           pDVal = new StringValue("abcabc'");
			Function            pFunc = new Function("FunctionName", new Expression[0]);
			BinaryExpression    pExprMult = new BinaryExpression(pDVal, BinaryOperations.BinaryOperations_Multiply, pFunc);
			Identifier          pIdent = new Identifier("th is.is.a.test");
			byte[]			pFGF = (new ASCIIEncoding()).GetBytes("SomeGeometryData");
			GeometryValue       pGVal = new GeometryValue(pFGF);

			// Filter Classes
			ComparisonCondition pCompare = new ComparisonCondition(pExprMult, ComparisonOperations.ComparisonOperations_EqualTo, pDVal);
			DistanceCondition   pDistance = new DistanceCondition(pIdent, DistanceOperations.DistanceOperations_Beyond, pGVal, 3.14);
			BinaryLogicalOperator   pEqual = new BinaryLogicalOperator(pCompare, BinaryLogicalOperations.BinaryLogicalOperations_And, pDistance);
#if DUMPTESTCONTENT
			Console.WriteLine(pEqual.ToString());
#endif
			InCondition         pIn = new InCondition(pIdent, aszNames);

			NullCondition       pNull = new NullCondition(pIdent);
#if DUMPTESTCONTENT
			Console.WriteLine(pNull.ToString());
#endif

			SpatialCondition    pSpatial = new SpatialCondition(pIdent, SpatialOperations.SpatialOperations_Contains, pGVal);
			UnaryLogicalOperator    pNot = new UnaryLogicalOperator(pSpatial, UnaryLogicalOperations.UnaryLogicalOperations_Not);
#if DUMPTESTCONTENT
			Console.WriteLine(pNot.ToString());
#endif

            //Console.WriteLine("finish mgFilterTest.testFilterTree");
        }

		public void testBinaryLogicalOperator()
		{
			Console.WriteLine("start mgFilterTest.testBinaryLogicalOperator");

			// create some data for the test.
			DataValue           pDVal = new StringValue("'abcabc'");
			Identifier          pIdent = new Identifier("this.is.a.t'e\"st");
			ComparisonCondition pCompare1 = new ComparisonCondition(pIdent, ComparisonOperations.ComparisonOperations_EqualTo, pDVal);
			ComparisonCondition pCompare2 = new ComparisonCondition(pIdent, ComparisonOperations.ComparisonOperations_GreaterThan, pDVal);

			// check that the constructor put everything in it's place
			BinaryLogicalOperator   pEqual = new BinaryLogicalOperator(pCompare1, BinaryLogicalOperations.BinaryLogicalOperations_Or, pCompare2);
			Debug.Assert(pEqual.Operation == BinaryLogicalOperations.BinaryLogicalOperations_Or);
			Filter   pFilter = pEqual.LeftOperand;
			Debug.Assert(pFilter == pCompare1);			
			pFilter = pEqual.RightOperand;
			Debug.Assert(pFilter == pCompare2);

			// check setting to null
			pEqual.LeftOperand = null;
			pEqual.RightOperand = null;
			Debug.Assert(pEqual.LeftOperand == null);
			Debug.Assert(pEqual.RightOperand == null);

			// check the set methods.
			pEqual.LeftOperand = pCompare2;
			pEqual.RightOperand = pCompare1;
			pEqual.Operation = BinaryLogicalOperations.BinaryLogicalOperations_And;
			Debug.Assert(pEqual.Operation == BinaryLogicalOperations.BinaryLogicalOperations_And);
			pFilter = pEqual.LeftOperand;
			Debug.Assert(pFilter == pCompare2);
			pFilter = pEqual.RightOperand;
			Debug.Assert(pFilter == pCompare1);

			//Console.WriteLine("finish mgFilterTest.testBinaryLogicalOperator");
			
		}

		public void testComparisonCondition()
		{
			Console.WriteLine("start mgFilterTest.testComparisonCondition");

			// create some data for the test.
			DataValue           pDVal = new Int32Value(123);
			Identifier          pIdent = new Identifier("thisisatest");

			// test the constructor
			ComparisonCondition pCompare = new ComparisonCondition(pIdent, ComparisonOperations.ComparisonOperations_EqualTo, pDVal);
			Debug.Assert(pCompare.Operation == ComparisonOperations.ComparisonOperations_EqualTo);
			Expression  pExpr = pCompare.LeftExpression;
			Debug.Assert(pExpr == pIdent);
			pExpr = pCompare.RightExpression;
			Debug.Assert(pExpr == pDVal);

			// test the setters and getters
			pCompare.Operation = ComparisonOperations.ComparisonOperations_GreaterThan;
			pCompare.LeftExpression = pDVal;
			pCompare.RightExpression = pIdent;
			Debug.Assert(pCompare.Operation == ComparisonOperations.ComparisonOperations_GreaterThan);
			pExpr = pCompare.LeftExpression;
			Debug.Assert(pExpr == pDVal);
			pExpr = pCompare.RightExpression;
			Debug.Assert(pExpr == pIdent);
			
			//Console.WriteLine("finish mgFilterTest.testComparisonCondition");
			
		}

		public void testDistanceCondition()
		{
			Console.WriteLine("start mgFilterTest.testDistanceCondition");

			// create some data for the test.
			Identifier          pIdent = new Identifier("how.now.brown.cow");
			byte[]	pFGF1 = (new ASCIIEncoding()).GetBytes("testing123");
			GeometryValue       pGVal1 = new GeometryValue(pFGF1);
			byte[]	pFGF2 = (new ASCIIEncoding()).GetBytes("testtesttest");
			GeometryValue       pGVal2 = new GeometryValue(pFGF2);

			// check constructor.
			DistanceCondition   pCond1 = new DistanceCondition("PropName", DistanceOperations.DistanceOperations_Beyond, pGVal1, 3.14);
			Debug.Assert(pCond1.Distance == 3.14);
			Expression   pExpr = pCond1.Geometry;
			Debug.Assert(pExpr == pGVal1);
			Debug.Assert(pCond1.Operation == DistanceOperations.DistanceOperations_Beyond);
			Identifier  pIdentTest = pCond1.PropertyName;
			Debug.Assert(pIdentTest.Text ==  "PropName");

			// test the setters and getters
			pCond1.Distance = 1.21;
			pCond1.Geometry = null;
			pCond1.PropertyName = pIdent;
			Debug.Assert(pCond1.Distance == 1.21);
			Debug.Assert(pCond1.Geometry == null);
			pIdentTest = pCond1.PropertyName;
			Debug.Assert(pIdentTest == pIdent);

			// check the other constructor
			DistanceCondition   pCond2 = new DistanceCondition(pIdent, DistanceOperations.DistanceOperations_Within, pGVal2, .9944);
			Debug.Assert(pCond2.Distance == .9944);
			pExpr = pCond2.Geometry;
			Debug.Assert(pExpr == pGVal2);
			Debug.Assert(pCond2.Operation == DistanceOperations.DistanceOperations_Within);
			pIdentTest = pCond2.PropertyName;
			Debug.Assert(pIdentTest == pIdent);

			// more tests of the setters and getters
			pCond2.Geometry = pGVal1;
			pCond2.Operation = DistanceOperations.DistanceOperations_Beyond;
			pCond2.PropertyName = null;
			pExpr = pCond2.Geometry;
			Debug.Assert(pExpr == pGVal1);
			Debug.Assert(pCond2.Operation == DistanceOperations.DistanceOperations_Beyond);
			Debug.Assert(pCond2.PropertyName == null);

			//Console.WriteLine("finish  mgFilterTest.testDistanceCondition");
			
		}

		public void testInCondition()
		{
			Console.WriteLine("start mgFilterTest.testInCondition");

			// create some data for the test.
			Identifier          pIdent = new Identifier("zip.a.dee.doo.da");

			// check constructor.
			InCondition         pIn1 = new InCondition("RogerWilco", aszNames);
			Identifier  pIdentTest = pIn1.PropertyName;
			Debug.Assert(pIdentTest.Text == "RogerWilco");
			ValueExpressionCollection pColl = pIn1.Values;
			Debug.Assert(pColl.Count == 3);
			for (int i = 0; i < pColl.Count; i++)
			{
				ValueExpression v = (ValueExpression)pColl[i];
				Parameter   pParam = (Parameter)v;
				Debug.Assert(pParam != null);
				Debug.Assert(pParam.Name == aszNames[i]);
			}

			// test the setters and getters
			pIn1.PropertyName = pIdent;
			pIdentTest = pIn1.PropertyName;
			Debug.Assert(pIdentTest == pIdent);

			// check the other constructor
			InCondition         pIn2 = new InCondition(pIdent, new string[] {aszNames[0]});
			ValueExpressionCollection pColl2 = pIn2.Values;
			Debug.Assert(pColl2.Count == 1);
			pIdentTest = pIn2.PropertyName;
			Debug.Assert(pIdentTest == pIdent);

			// test the setters and getters with null values
			pIn2.PropertyName = null;
			Debug.Assert(pIn2.PropertyName == null);

			// check the constructor with null values
			InCondition         pIn3 = new InCondition(pIdent, new string[0]);
			ValueExpressionCollection pColl3 = pIn3.Values;
			Debug.Assert(pColl3.Count == 0);

			//Console.WriteLine("finish  mgFilterTest.testInCondition");
			
		}

		public void testNullCondition()
		{
			Console.WriteLine("start mgFilterTest.testNullCondition");

			// create some data for the test.
			Identifier          pIdent = new Identifier("Sham.A.Lam.A.Ding.Dong");

			// check constructor.
			NullCondition       pNull = new NullCondition(pIdent);
			Identifier  pIdentTest = pNull.PropertyName;
			Debug.Assert(pIdentTest == pIdent);

			// test the setters and getters
			pNull.PropertyName = null;
			Debug.Assert(pNull.PropertyName == null);

			// check the other constructor
			NullCondition       pNull2 = new NullCondition("DooWap");
			pIdentTest = pNull2.PropertyName;
			Debug.Assert(pIdentTest.Text == "DooWap");

			//Console.WriteLine("finish  mgFilterTest.testNullCondition");
			
		}

		public void testSpatialCondition()
		{
			Console.WriteLine("start mgFilterTest.testSpatialCondition");

			// create some data for the test.
			Identifier          pIdent = new Identifier("TestIdent");
			byte[]			pFGF1 = (new ASCIIEncoding()).GetBytes("testing123");
			GeometryValue       pGVal1 = new GeometryValue(pFGF1);
			byte[]			pFGF2 = (new ASCIIEncoding()).GetBytes("testtesttest");
			GeometryValue       pGVal2 = new GeometryValue(pFGF2);

			Identifier      pIdentTest;
			Expression      pGeomTest;

			// check constructor.
			SpatialCondition    pSC1 = new SpatialCondition("StringName", SpatialOperations.SpatialOperations_Within, pGVal1);
			pGeomTest = pSC1.Geometry;
			Debug.Assert(pGeomTest == pGVal1);
			Debug.Assert(pSC1.Operation == SpatialOperations.SpatialOperations_Within);
			pIdentTest = pSC1.PropertyName;
			Debug.Assert(pIdentTest.Text == "StringName");

			// test the setters and getters
			pSC1.Geometry = pGVal2;
			pSC1.Operation = SpatialOperations.SpatialOperations_Crosses;
			pSC1.PropertyName = pIdent;
			pGeomTest = pSC1.Geometry;
			Debug.Assert(pGeomTest == pGVal2);
			Debug.Assert(pSC1.Operation == SpatialOperations.SpatialOperations_Crosses);
			pIdentTest = pSC1.PropertyName;
			Debug.Assert(pIdentTest == pIdent);

			// test the setters and getters with null values
			pSC1.PropertyName = null;
			pSC1.Geometry = null;
			Debug.Assert(pSC1.Geometry == null);
			Debug.Assert(pSC1.PropertyName == null);

			// check the other constructor
			SpatialCondition    pSC2 = new SpatialCondition(pIdent, SpatialOperations.SpatialOperations_Crosses, pGVal2);
			pGeomTest = pSC2.Geometry;
			Debug.Assert(pGeomTest == pGVal2);
			Debug.Assert(pSC2.Operation == SpatialOperations.SpatialOperations_Crosses);
			pIdentTest = pSC2.PropertyName;
			Debug.Assert(pIdentTest == pIdent);

			//Console.WriteLine("finish  mgFilterTest.testSpatialCondition");
			
		}

		public void testUnaryLogicalOperator()
		{
			Console.WriteLine("start mgFilterTest.testUnaryLogicalOperator");

			// create some data for the test.
			DataValue           pDVal = new StringValue("abc\"abc");
			Identifier          pIdent = new Identifier("'this.is.a.test");
			ComparisonCondition pCompare1 = new ComparisonCondition(pIdent, ComparisonOperations.ComparisonOperations_EqualTo, pDVal);
			ComparisonCondition pCompare2 = new ComparisonCondition(pIdent, ComparisonOperations.ComparisonOperations_GreaterThan, pDVal);

			// check constructor.
			UnaryLogicalOperator    pNot = new UnaryLogicalOperator(pCompare1, UnaryLogicalOperations.UnaryLogicalOperations_Not);
			Debug.Assert(pNot.Operation == UnaryLogicalOperations.UnaryLogicalOperations_Not);
			Filter  pFilter = pNot.Operand;
			Debug.Assert(pFilter == pCompare1);

			// set to null and check
			pNot.Operand = null;
			Debug.Assert(pNot.Operand == null);

			// set to operand and check.
			pNot.Operand = pCompare2;
			pFilter = pNot.Operand;
			Debug.Assert(pFilter == pCompare2);

			//Console.WriteLine("finish  mgFilterTest.testUnaryLogicalOperator");
			
		}

		public void testComputedIdentifier()
		{
			Console.WriteLine("start mgFilterTest.testComputedIdentifier");

			Identifier pWidth = new Identifier("Width");
			Identifier pLength = new Identifier("Length");

			BinaryExpression pArea = new BinaryExpression(pWidth, BinaryOperations.BinaryOperations_Multiply, pLength);

			ComputedIdentifier pCompIdent = new ComputedIdentifier("Area", pArea);
			DataValue pDVal = new Int32Value(123);

			// test the constructor
			ComparisonCondition pCompare = new ComparisonCondition(pCompIdent, ComparisonOperations.ComparisonOperations_EqualTo, pDVal);

#if DUMPTESTCONTENT
			Console.WriteLine(pCompare.ToString() );
#endif
			//Console.WriteLine("finish  mgFilterTest.testComputedIdentifier");
			
		}
	}
}

