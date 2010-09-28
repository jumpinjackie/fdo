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
using OSGeo.FDO.Filter;

namespace Fdo_Test
{
	/// <summary>
	/// Summary description for mgFilterParseTest.
	/// </summary>
	public class mgFilterParseTest
	{
		public mgFilterParseTest()
		{
			Console.WriteLine("start mgFilterParseTest");
		}

		// parse an FDO Filter that we except to return null because of error
		private void ParseFilterToNull(String pwzFilter)
		{
			// input expression to parse
#if DUMPTESTCONTENT
			Console.WriteLine( pwzFilter);
#endif
			// get root node of expression parse tree
			Filter  pFilter = null;
			try
			{
				pFilter = Filter.Parse(pwzFilter);
			}
			catch(OSGeo.FDO.Common.Exception ex)
			{
                Debug.Assert(ex.Message.IndexOf("String does not represent a valid filter") != -1, "Exception null should be another exception.");
			}
			Debug.Assert(pFilter == null, "Filter::Parse() should have returned null!\n");
		}

		// parse an FDO Filter
		private void ParseFilter(String pwzFilter, String pwzResult)
		{
			// input expression to parse
#if DUMPTESTCONTENT
			Console.WriteLine(pwzFilter);
#endif

			// get root node of expression parse tree
			Filter pFilter = Filter.Parse(pwzFilter);
			Debug.Assert(pFilter != null, "Filter::Parse() returned null!");

			// output back to string if successful
			String pwzOut = pFilter.ToString();
			Debug.Assert(pwzOut != null, "Filter::ToString() returned null!");

			char [] szError = new char[512];
			if (pwzResult == null)
			{
				Debug.Assert(pwzOut == pwzFilter, "Parse/ToString do not match!\n\t" + pwzOut + " should be " + pwzFilter);
			}
			else
			{
				Debug.Assert(pwzOut == pwzResult, "Parse/ToString do not match!\n\t" + pwzOut + " should be " + pwzResult);
			}

//			mgUnitTestProcessor testProc = new mgUnitTestProcessor();
//			pFilter.Process(testProc);
//			Console.WriteLine(pwzOut);
		}

		private void ParseFilter(String pwzFilter)
		{
			ParseFilter(pwzFilter, null);
		}

		public void test()
		{
			Console.WriteLine("start mgFilterParseTest.test");

			ParseFilter("colourIndex > -a", "colourIndex > -(a)");
			ParseFilter("colourIndex > -(5)");
			ParseFilter("colourIndex > -5");
			ParseFilter("colourIndex < -5");
			ParseFilter("colourIndex >= -5");
			ParseFilter("colourIndex <= -5");
			ParseFilter("(Id = 1 and Name = 'Fred') or (Id = 2 and Name = 'John')",	"Id = 1 AND Name = 'Fred' OR Id = 2 AND Name = 'John'");

			// identifiers with special characters
			ParseFilter("\"A\"\"B\" = '123'");
			ParseFilter("\"@#$%OR\" = '123'");	// DID 550139
			ParseFilter("\"OR\" = '123'");
			ParseFilter("\"or\" = '123' OR \"Or\" = 'abc'");

			// grammatical nightmares
			ParseFilterToNull("1 12345678901234567 1.234 1.2e13 -2 --2 +2 ++2");
			ParseFilterToNull("+ - * / < <= = == >= > : || @");
			ParseFilterToNull(" AND BEYOND BOUNDINGBOX COMPARE CONTAINS COVEREDBY CROSSES DATE DAY DISJOINT DISTANCE ENVELOPEINTERSECTS EQUALS FALSE HOUR IN INSIDE INTERSECTS LIKE MINUTE MONTH NOT null OR OVERLAPS SECOND SPATIAL TIME TIMESTAMP TOUCHES TRUE WITHIN YEAR");
			ParseFilterToNull(" And beyond BoundingBox Compare conTAINS coveredby");
			ParseFilterToNull(" \"missing quote");
			//?	ParseFilterToNull("tooooolonnnnngggggidentifierakdadjsaljdasljlasjdsljdslj");
			ParseFilterToNull("\"tooooooolonnngggstringliteralsasad;sklad;sls                                                                                                                                                                                                              ");
			ParseFilterToNull("+=4=");

			ParseFilter("a<4", "a < 4");
			ParseFilter("a<4 or col= 2", "a < 4 OR col = 2");
			ParseFilter("a = b OR c = d");
			ParseFilter("a = b AND c = d");
			ParseFilter("a = b OR c = d or a = b AND c = d", 	"a = b OR c = d OR a = b AND c = d");
			ParseFilter("a = b AND c = d or a = b AND c = d", "a = b AND c = d OR a = b AND c = d");
			ParseFilter("a = b");
			ParseFilter("a <> b");
			ParseFilter("a != b", "a <> b");
			ParseFilter("a < b");
			ParseFilter("a <= b");
			ParseFilter("a > b");
			ParseFilter("a >= b");
			ParseFilter("a LIKE 'abc*'");
			ParseFilter("\"Feature Number\" = 4642");

			// Sascha Nikolayev sample
			ParseFilter("col1 > 10 and col2 in (1,2,3,4)", "col1 > 10 AND col2 IN (1, 2, 3, 4)");

			// Example from Vinay.  Identifier were only returning their name, not 
			// the fully qualified text.
			ParseFilter("NOT Entity.Color NULL");
			ParseFilter("Entity.Color IN ('Red', 'Blue', 'Green')");

			// Examples from FDO Expressions and FiltersRpt.doc
			// Hence a filter to select roads with four or more lanes might look like:
			ParseFilter("Lanes >= 4");

			// Similarly in the Civil model a filter to select all PipeNetworks that
			// have at least one Pipe in the proposed state might look like:
			ParseFilter("Pipes.state = 'proposed'");

			// Also using the Civil model a filter to select all existing Parcels
			// whose owner contains the text "Smith" might look something like:
			ParseFilter("state = 'existing' and owner like '%Smith%'", "state = 'existing' AND owner LIKE '%Smith%'");

			// Likewise a filter to select all Parcels that are either affected
			// or encroached upon by some change might look like:
			ParseFilter("state in ('affected', 'encroached')", "state IN ('affected', 'encroached')");

			ParseFilter("state in (:'Enter State', 'encroached', TRUE, 123.333, 14)", "state IN (:'Enter State', 'encroached', TRUE, 123.333, 14)");

			// Distance Conditions
			ParseFilter("feature beyond GeomFromText('POINT(0.0 0.0)') 12.0", "feature BEYOND GeomFromText('POINT (0 0)') 12");
			ParseFilter("feature withindistance GeomFromText('POINT(0.0 0.0)') 12", "feature WITHINDISTANCE GeomFromText('POINT (0 0)') 12");
			ParseFilter("feature withindistance GeomFromText('POINT(0.0 0.0)') 12.0", "feature WITHINDISTANCE GeomFromText('POINT (0 0)') 12");

			// Spatial Conditions
			ParseFilter("feature contains GeomFromText('POINT(0.0 0.0)')", "feature CONTAINS GeomFromText('POINT (0 0)')");
			ParseFilter("feature crosses GeomFromText('POINT(0.0 0.0)')",  "feature CROSSES GeomFromText('POINT (0 0)')");
			ParseFilter("feature disjoint GeomFromText('POINT(0.0 0.0)')", "feature DISJOINT GeomFromText('POINT (0 0)')");
			ParseFilter("feature equals GeomFromText('POINT(0.0 0.0)')",  "feature EQUALS GeomFromText('POINT (0 0)')");
			ParseFilter("feature intersects GeomFromText('POINT(0.0 0.0)')", "feature INTERSECTS GeomFromText('POINT (0 0)')");
			ParseFilter("feature overlaps GeomFromText('POINT(0.0 0.0)')", "feature OVERLAPS GeomFromText('POINT (0 0)')");
			ParseFilter("feature touches GeomFromText('POINT(0.0 0.0)')", 	"feature TOUCHES GeomFromText('POINT (0 0)')");
			ParseFilter("feature within GeomFromText('POINT(0.0 0.0)')", "feature WITHIN GeomFromText('POINT (0 0)')");
			ParseFilter("feature coveredby GeomFromText('POINT(0.0 0.0)')", "feature COVEREDBY GeomFromText('POINT (0 0)')");
			ParseFilter("feature inside GeomFromText('POINT(0.0 0.0)')", "feature INSIDE GeomFromText('POINT (0 0)')");
			ParseFilter("feature envelopeintersects GeomFromText('POINT(0.0 0.0)')", "feature ENVELOPEINTERSECTS GeomFromText('POINT (0 0)')");

			ParseFilter("feature touches GeomFromText('POINT(0.0 0.0)') AND feature beyond GeomFromText('POINT(0.0 0.0)') 12.0 AND state = 'troubled'",
				"feature TOUCHES GeomFromText('POINT (0 0)') AND feature BEYOND GeomFromText('POINT (0 0)') 12 AND state = 'troubled'");

			ParseFilter("( NumberOfLanes*Speed ) AS Capacity, Capacity > 200");

			ParseFilter("( Width*Length ) AS Area, Area > 100 AND Area < 1000");
			ParseFilter("( (Width+Length)*2 ) AS perimeter, perimeter > 100 AND perimeter < 1000");
			ParseFilter("( (Width+Length)*2 ) AS perimeter, ( Width*Length ) AS Area, Area > 1000 OR perimeter < 100");
			ParseFilter("( (Width+Length)*2 ) AS perimeter, ( Width*Length ) AS Area, Area > perimeter");

			//Console.WriteLine("finish mgFilterParseTest.test");
			
		}
	}
}

