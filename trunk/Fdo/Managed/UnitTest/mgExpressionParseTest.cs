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
using OSGeo.FDO.Expression;

namespace Fdo_Test
{
	/// <summary>
	/// Summary description for mgExpressionParseTest.
	/// </summary>
	public class mgExpressionParseTest
	{
		public mgExpressionParseTest()
		{
			Console.WriteLine("start mgExpressionParseTest");
		}

		private void ParseExpressionToNull(string pwzExpression)
		{
#if DUMPTESTCONTENT
			Console.WriteLine(pwzExpression);
#endif
			Expression pExpression = null;
			try
			{
				pExpression = Expression.Parse(pwzExpression);
			}
            catch (OSGeo.FDO.Common.Exception ex)
			{
                Debug.Assert(ex.Message.IndexOf("String does not represent a valid expression") != -1, "Exception null should be another exception.");
			}

			Debug.Assert(pExpression == null, "FdoExpression::Parse() should have returned NULL!");
		}

		private void ParseExpression(string pwzExpression, string pwzResult)
		{
#if DUMPTESTCONTENT
			Console.WriteLine(pwzExpression);
#endif
			Expression pExpression = Expression.Parse(pwzExpression);
			Debug.Assert(pExpression != null, "FdoExpression::Parse() returned NULL!");
			string pwzOut = pExpression.ToString();
			Debug.Assert(pwzOut != null, "FdoExpression::ToString() returned NULL!");
			char [] szError = new char[4000+4000+30];
			if(pwzResult == null)
			{
				Debug.Assert(pwzOut == pwzExpression, "Parse/ToString do not match!\n\t" + pwzOut + " should be " + pwzExpression + "\n");
			}
			else
			{
				Debug.Assert(pwzOut == pwzResult, "Parse/ToString do not match!\n\t" + pwzOut + " should be " + pwzResult + "\n");
			}
		}

		private void ParseExpression(string pwzExpression)
		{
			ParseExpression(pwzExpression, null);
		}

		// test expressions other than FGFT geometry values
		public void testExpressions()
		{
			Console.WriteLine("start mgExpressionParseTest.testExpressions");

			// trivial test
			ParseExpression("4");
			ParseExpression("4.5");
			ParseExpression("TRUE");
			ParseExpression("true",  "TRUE");
			ParseExpression("True",  "TRUE");
			ParseExpression("4+4.5");
			ParseExpression("4.5+5.4*6.6-3.2/12");
			ParseExpression("-3.4");
			ParseExpression("-3.4-5.6");
			ParseExpression("-(3.4*5.6)");
			ParseExpression("-'abc'",	 "-('abc')");
			ParseExpression("-'abc'-'abc'", "-('abc')-'abc'");
			ParseExpression("(2*6)-(3*5)", "2*6-3*5");
			ParseExpression("-TRUE",  "-(TRUE)");
			ParseExpression("'abc'");
			ParseExpression("'abc''def'");
			ParseExpression("a+b*34/12");
            ParseExpression("12345678901234567", "12345678901234567"); 
			ParseExpression("1.2e13", 	"12000000000000");
			ParseExpression("-2 --2 +2 ++2", "-2--2+2+2");
			
			// dates 
			ParseExpression("DATE '1951-12-24'");
			ParseExpression("DATE '1971-12-24'");
			
			// time is meaningess since it woud be stored since Jan 1, 1970
			ParseExpression("TIME '12:01:30'");
			ParseExpression("TIMESTAMP '2003-10-23 11:00:02'");
			
			// compound expressions (some from Orace manua)
			ParseExpression("SQRT(144)");
			ParseExpression("SQRT('abc')");
			ParseExpression("SQRT(SQRT(2)+3)*2");
			ParseExpression("my_fun(TO_CHAR('abc','DD-MMM-YY'))", 	"my_fun(TO_CHAR('abc', 'DD-MMM-YY'))");
			ParseExpression(":EnterName");
			ParseExpression(":'Enter Name'");
			ParseExpression("MIN(a,b,2*3+45.67,d,e)", "MIN(a, b, 2*3+45.67, d, e)");
			ParseExpression("'CLARK' || 'SMITH'", "'CLARK'+'SMITH'");
			ParseExpression("'Lewis'+'Clark'");
			ParseExpression("LENGTH('MOOSE') * 57", "LENGTH('MOOSE')*57");
			ParseExpression("Pipes.state");
			ParseExpression("\"Pipes . state\"");
			ParseExpression("\"%one two thre#EE\"");
			ParseExpression("'%one two thre#EE'");	
			ParseExpression("2.3e400", "1.#INF");
			ParseExpression("(2+3)*12");
			ParseExpression("sqrt(144)+(12-32/12)", "sqrt(144)+12-32/12");			
			ParseExpression("( Width*Height ) AS Area");
				
			//Console.WriteLine("finish mgExpressionParseTest.testExpressions");
			
		}

		public void testFGFT()
		{
			Console.WriteLine("start mgExpressionParseTest.testFGFT");

			// negative vaue tests
			ParseExpression("GeomFromText('POINT XYZ (-1051999.72951478 -.232884005 -0.00000001)')", "GeomFromText('POINT XYZ (-1051999.72951478 -0.232884005 -0.00000001)')");
			ParseExpression("GeomFromText('POINT XYZ (- 1051999.72951478 - .232884005 - 0.00000001)')", "GeomFromText('POINT XYZ (-1051999.72951478 -0.232884005 -0.00000001)')");
			
			// Trivia exampe of how to get a geometry vaue from FGFT string.
			// Works for this specia case because the parse tree has ony one
			// node and the root is a FdoGeometryVaue
			GeometryValue gvalue = (GeometryValue)Expression.Parse("GeomFromText('POLYGON ((10 10, 10 20, 20 20, 20 15, 10 10))')");

#if DUMPTESTCONTENT
			Console.WriteLine(gvalue.ToString());
#endif

			Debug.Assert(gvalue.ToString() == "GeomFromText('POLYGON ((10 10, 10 20, 20 20, 20 15, 10 10))')", "Trivia test faied!");
			ParseExpression("GeomFromText('POLYGON XYZ ((1051999.72951478 563999.232884005 0.00000001, 1055009.22168253 563999.232884005 0.00000001, 1055009.22168253 566002.587400644 0.00000001, 1051999.72951478 566002.587400644 0.00000001, 1051999.72951478 563999.232884005 0.00000001))')");
			ParseExpression("GeomFromText('POINT(0.0 0.0)')","GeomFromText('POINT (0 0)')");
			
			// Exampes from Open GIS Simpe Feature Specification for SQ
			ParseExpression("GeomFromText('POINT (10 10)')");
			ParseExpression("GeomFromText('LINESTRING (10 10, 20 20, 30 40)')");
			ParseExpression("GeomFromText('POLYGON ((10 10, 10 20, 20 20, 20 15, 10 10))')");
			ParseExpression("GeomFromText('MULTIPOINT (10 10, 20 20)')");
			ParseExpression("GeomFromText('MULTILINESTRING ((10 10, 20 20), (15 15, 30 15))')");
			ParseExpression("GeomFromText('MULTIPOLYGON (((10 10, 10 20, 20 20, 20 15, 10 10)), ((60 60, 70 70, 80 60, 60 60)))')");
			
			// other more compicated WKT sampes
			// ... mutipe inear rings
			ParseExpression("GeomFromText('POLYGON ((10 10, 10 20, 20 20, 20 15, 10 10), (11 11, 11 21, 21 21, 21 16, 11 11))')");
			// two exterior ring ony poygons
			ParseExpression("GeomFromText('MULTIPOLYGON (((10 10, 10 20, 20 20, 20 15, 10 10)), ((60 60, 70 70, 80 60, 60 60)))')");
			// two rings in one poygon
			ParseExpression("GeomFromText('MULTIPOLYGON (((10 10, 10 20, 20 20, 20 15, 10 10), (60 60, 70 70, 80 60, 60 60)))')");
			// two rings in two poygons
			ParseExpression("GeomFromText('MULTIPOLYGON (((10 10, 10 20, 20 20, 20 15, 10 10), (60 60, 70 70, 80 60, 60 60)), ((10 10, 10 20, 20 20, 20 15, 10 10), (60 60, 70 70, 80 60, 60 60)))')");
			
			// geometry coection of WKT types
			ParseExpression("GeomFromText('GEOMETRYCOLLECTION (POINT (10 10),POINT (30 30), LINESTRING (15 15, 20 20))')",
				"GeomFromText('GEOMETRYCOLLECTION (POINT (10 10), POINT (30 30), LINESTRING (15 15, 20 20))')");
			//TODO:GC "Muti" types not yet supported in GEOMETRYCOECTION
			//    ParseExpression("GeomFromText('GEOMETRYCOECTION (POYGON ((10 10, 10 20, 20 20, 20 15, 10 10)), MUTIPOINT (10 10, 20 20), MUTIPOYGON (((10 10, 10 20, 20 20, 20 15, 10 10)),((60 60, 70 70, 80 60, 60 60 ) )))')",
			//		"GeomFromText('NU')");
			
			// et's do a geometry coection within a geometry coection, just to make sure it's 
			// possibe to parse, but there is no code to support it yet
			//TODO:GC geometry says bad parameter with nested geometry coections
			// see Fgf/MultiGeometry.cpp 	case GisGeometryType_MutiGeometry:     // No nesting currenty supported.
			//	ParseExpression("GeomFromText('GEOMETRYCOECTION (POINT (10 10), GEOMETRYCOECTION(POINT (30 30)))')",
			//		"GeomFromText('NU')");
			
			// Autodesk extensions FGFT
			// ... dimensionaity types
			ParseExpression("GeomFromText('POINT (10 11)')");
			ParseExpression("GeomFromText('POINT XY (10 11)')","GeomFromText('POINT (10 11)')");
			ParseExpression("GeomFromText('POINT XYZ (10 11 12)')");
			ParseExpression("GeomFromText('POINT XYM (10 11 1.2)')");
			ParseExpression("GeomFromText('POINT XYZM (10 11 12 1.2)')");
			
			// .. geometry coection of extended types 
			ParseExpression("GeomFromText('GEOMETRYCOLLECTION (POINT xyz (10 11 12),POINT XYM (30 20 1.8), LINESTRING XYZM(1 2 3 4, 3 5 15, 3 20 20))')",
				"GeomFromText('GEOMETRYCOLLECTION (POINT XYZ (10 11 12), POINT XYM (30 20 1.8), LINESTRING XYZM (1 2 3 4, 3 5 15 0, 3 20 20 0))')");
			
			// ... additiona entity types
			ParseExpression("GeomFromText('CURVESTRING (0 0 (LINESTRINGSEGMENT (10 10, 20 20, 30 40)))')");
			ParseExpression("GeomFromText('CURVESTRING (0 0 (CIRCULARARCSEGMENT (11 11, 12 12), LINESTRINGSEGMENT (10 10, 20 20, 30 40)))')");
			ParseExpression("GeomFromText('CURVESTRING (0 0 (ARC (11 11, 12 12), LINESTRINGSEGMENT (10 10, 20 20, 30 40)))')",
				"GeomFromText('CURVESTRING (0 0 (CIRCULARARCSEGMENT (11 11, 12 12), LINESTRINGSEGMENT (10 10, 20 20, 30 40)))')");
			ParseExpression("GeomFromText('CURVESTRING XYZ (0 0 0 (LINESTRINGSEGMENT (10 10 1, 20 20 1, 30 40 1)))')");
			ParseExpression("GeomFromText('MULTICURVESTRING ((0 0 (LINESTRINGSEGMENT (10 10, 20 20, 30 40))),(0 0 (ARC (11 11, 12 12), LINESTRINGSEGMENT (10 10, 20 20, 30 40))))')",
				"GeomFromText('MULTICURVESTRING ((0 0 (LINESTRINGSEGMENT (10 10, 20 20, 30 40))), (0 0 (CIRCULARARCSEGMENT (11 11, 12 12), LINESTRINGSEGMENT (10 10, 20 20, 30 40))))')");
			ParseExpression("GeomFromText('MULTICURVESTRING ((100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (103 100))), (200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (203 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (303 300))))')");
			// this can throw an exception if it is not closed (defined by exact double == of first and last point
			ParseExpression("GeomFromText('CURVEPOLYGON ((0 0 (LINESTRINGSEGMENT (10 10, 20 20, 30 40, 0 0))), (0 0 (LINESTRINGSEGMENT (10 10, 40 40, 90 90, 0 0))))')");
			ParseExpression("GeomFromText('CURVEPOLYGON ((100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (100 100))), (200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300))))')");
			ParseExpression("GeomFromText('MULTICURVEPOLYGON (((200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300))), (400 400 (CIRCULARARCSEGMENT (400 401, 401 402), LINESTRINGSEGMENT (400 400)))), ((201 201 (CIRCULARARCSEGMENT (201 202, 202 203), LINESTRINGSEGMENT (201 201))), (301 301 (CIRCULARARCSEGMENT (301 302, 302 303), LINESTRINGSEGMENT (301 301))), (401 401 (CIRCULARARCSEGMENT (401 402, 402 403), LINESTRINGSEGMENT (401 401)))), ((202 202 (CIRCULARARCSEGMENT (202 203, 203 204), LINESTRINGSEGMENT (202 202))), (302 302 (CIRCULARARCSEGMENT (302 303, 303 304), LINESTRINGSEGMENT (302 302))), (402 402 (CIRCULARARCSEGMENT (402 403, 403 404), LINESTRINGSEGMENT (402 402)))))')");
			ParseExpression("GeomFromText('MULTICURVEPOLYGON (((10 10 (LINESTRINGSEGMENT (10 10, 10 20, 20 20), ARC (20 15, 10 10))), (90 90 (ARC (11 11, 12 12), LINESTRINGSEGMENT (10 10, 20 20, 40 40, 90 90)))),((8 8 (LINESTRINGSEGMENT (10 10, 10 20, 20 20), ARC (20 15, 8 8))), (90 90 (ARC (11 11, 12 12), LINESTRINGSEGMENT (10 10, 20 20, 40 40, 90 90)))))')",
				"GeomFromText('MULTICURVEPOLYGON (((10 10 (LINESTRINGSEGMENT (10 10, 10 20, 20 20), CIRCULARARCSEGMENT (20 15, 10 10))), (90 90 (CIRCULARARCSEGMENT (11 11, 12 12), LINESTRINGSEGMENT (10 10, 20 20, 40 40, 90 90)))), ((8 8 (LINESTRINGSEGMENT (10 10, 10 20, 20 20), CIRCULARARCSEGMENT (20 15, 8 8))), (90 90 (CIRCULARARCSEGMENT (11 11, 12 12), LINESTRINGSEGMENT (10 10, 20 20, 40 40, 90 90)))))')");
			
			// TODO: add more ParseExpressionToNu exampes
			
			// ... missing , between rings causes NU GeometryVaue()
			// in VaueExpressionCoection, DistanceCondition, and SpatiaCondition
			ParseExpressionToNull("GeomFromText('POLYGON ((10 10)(11 11)))')");	
			
			Console.WriteLine("finish mgExpressionParseTest.testFGFT");
			
		}

        public void testLargeExpressions()
        {
            Console.WriteLine("start mgExpressionParseTest.testLargeExpressions");

            try
            {

                // Test large FGFT geometry expression:
                ParseExpression("GeomFromText('CURVESTRING (0 0 (LINESTRINGSEGMENT (" +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000," +
                    "10.0000000000000000000000 10.0000000000000000000000, 20.0000000000000000000000 20.0000000000000000000000" +
                    ")))')"
                    ,
                    "GeomFromText('CURVESTRING (0 0 (LINESTRINGSEGMENT (10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, " +
                    "10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, " +
                    "10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, " +
                    "10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, " +
                    "10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, " +
                    "10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20, " +
                    "10 10, 20 20, 10 10, 20 20, 10 10, 20 20, 10 10, 20 20)))')");

                // Test large literal string expression:
                const int STRING_TEST_LENGTH = 20000;
                char[] veryLargeLiteralString = new char[STRING_TEST_LENGTH];
                for (int i = 0; i < STRING_TEST_LENGTH; i++)
                {
                    if (i == 0 || i == STRING_TEST_LENGTH - 1)
                        veryLargeLiteralString[i] = '\'';
                    //                    else if (i==STRING_TEST_LENGTH-1)
                    //                             veryLargeLiteralString[i] = '\0';  //在.NET中，不需要以\0结束
                    else
                        veryLargeLiteralString[i] = (char)(97 + i % 26);  // ascii-7 alphabet characters
                }
                ParseExpression(new string(veryLargeLiteralString));
            }
            catch (OSGeo.FDO.Common.Exception)
            {
                Debug.Assert(false);
            }

            Console.WriteLine("finish mgExpressionParseTest.testLargeExpressions");
            
        }
	}
}

