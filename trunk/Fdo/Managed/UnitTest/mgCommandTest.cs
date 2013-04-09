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
using OSGeo.FDO.Commands;
using OSGeo.FDO.Expression;

namespace Fdo_Test
{
	/// <summary>
	/// Summary description for mgCommandTest.
	/// </summary>
	public class mgCommandTest
	{
		public mgCommandTest()
		{
			Console.WriteLine("start mgCommandTest");
		}

		public void testBatchParameterValueCollection()
		{
			Console.WriteLine("start mgCommandTest.testBatchParameterValueCollection");

			ParameterValueCollection pColl;

			// create data
			BatchParameterValueCollection pBatch = new BatchParameterValueCollection();
			ParameterValueCollection pColl_1 = new ParameterValueCollection();
			ParameterValueCollection pColl_2 = new ParameterValueCollection();
			ParameterValueCollection pColl_3 = new ParameterValueCollection();

			// add to collection
			pBatch.Add(pColl_1);
			pBatch.Add(pColl_2);
			pBatch.Insert(1, pColl_3);

			// check collection is consistent
			pColl = pBatch[0];
			Debug.Assert(pColl == pColl_1);

			pColl = pBatch[1];
			Debug.Assert(pColl == pColl_3);
    
			pColl = pBatch[2];
			Debug.Assert(pColl == pColl_2);

			Debug.Assert(pBatch.Count == 3);

			Debug.Assert(pBatch.Contains(pColl_1));
			Debug.Assert(pBatch.Contains(pColl_2));
			Debug.Assert(pBatch.Contains(pColl_3));

			Debug.Assert(pBatch.IndexOf(pColl_1) == 0);
			Debug.Assert(pBatch.IndexOf(pColl_2) == 2);
			Debug.Assert(pBatch.IndexOf(pColl_3) == 1);

			// remove an item
			pBatch.Remove(pColl_3);

			// check collection is consistent
			Debug.Assert(pBatch.Count == 2);

			pColl = pBatch[0];
			Debug.Assert(pColl == pColl_1);

			pColl = pBatch[1];
			Debug.Assert(pColl == pColl_2);

			Debug.Assert(pBatch.Contains(pColl_1));
			Debug.Assert(pBatch.Contains(pColl_2));
			Debug.Assert(!pBatch.Contains(pColl_3));

			Debug.Assert(pBatch.IndexOf(pColl_1) == 0);
			Debug.Assert(pBatch.IndexOf(pColl_2) == 1);
			Debug.Assert(pBatch.IndexOf(pColl_3) == -1);

			// remove another item
			pBatch.RemoveAt(1);

			// check collection is consistent
			Debug.Assert(pBatch.Count == 1);

			pColl = pBatch[0];
			Debug.Assert(pColl == pColl_1);

			Debug.Assert(pBatch.Contains(pColl_1));
			Debug.Assert(!pBatch.Contains(pColl_2));
			Debug.Assert(!pBatch.Contains(pColl_3));

			Debug.Assert(pBatch.IndexOf(pColl_1) == 0);
			Debug.Assert(pBatch.IndexOf(pColl_2) == -1);
			Debug.Assert(pBatch.IndexOf(pColl_3) == -1);
			
			//Console.WriteLine("finish mgCommandTest.testBatchParameterValueCollection");
			
		}

		public void testIdentifierCollection()
		{
			Console.WriteLine("start mgCommandTest.testIdentifierCollection");

			Identifier pIdent;

			// create data
			IdentifierCollection pIdentColl = new IdentifierCollection();
			Identifier pIdent1 = new Identifier("Ident1");
			Identifier pIdent2 = new Identifier();
            pIdent2.Text = "Ident2.set.later";
			Identifier pIdent3 = new Identifier("Ident2");;
			Identifier pIdent4 = new Identifier("Ident3");;

			// add identifiers to collection
			pIdentColl.Add(pIdent2);
			pIdentColl.Insert(0, pIdent1);
			pIdentColl.Add(pIdent4);
			pIdentColl.Insert(2, pIdent3);

			// check collection is consistent
			Debug.Assert(pIdentColl.Count == 4);

            pIdent = pIdentColl[0];
			Debug.Assert(pIdent == pIdent1);
			pIdent = pIdentColl[1];
			Debug.Assert(pIdent == pIdent2);
			pIdent = pIdentColl[2];
			Debug.Assert(pIdent == pIdent3);
			pIdent = pIdentColl[3];
			Debug.Assert(pIdent == pIdent4);

			Debug.Assert(pIdentColl.Contains(pIdent1));
			Debug.Assert(pIdentColl.Contains(pIdent2));
			Debug.Assert(pIdentColl.Contains(pIdent3));
			Debug.Assert(pIdentColl.Contains(pIdent4));

			Debug.Assert(pIdentColl.IndexOf(pIdent1) == 0);
			Debug.Assert(pIdentColl.IndexOf(pIdent2) == 1);
			Debug.Assert(pIdentColl.IndexOf(pIdent3) == 2);
			Debug.Assert(pIdentColl.IndexOf(pIdent4) == 3);

			pIdent = pIdentColl.GetItem("Ident3");
			Debug.Assert(pIdent == pIdent4);
			Debug.Assert(pIdentColl.FindItem("Bogus") == null);

			// remove first and last items
			pIdentColl.RemoveAt(0);
			pIdentColl.RemoveAt(2);

			// check collection is consistent
			Debug.Assert(pIdentColl.Count == 2);

			pIdent = pIdentColl[0];
			Debug.Assert(pIdent == pIdent2);
			pIdent = pIdentColl[1];
			Debug.Assert(pIdent == pIdent3);

			Debug.Assert(!pIdentColl.Contains(pIdent1));
			Debug.Assert(pIdentColl.Contains(pIdent2));
			Debug.Assert(pIdentColl.Contains(pIdent3));
			Debug.Assert(!pIdentColl.Contains(pIdent4));

			Debug.Assert(pIdentColl.IndexOf(pIdent1) == -1);
			Debug.Assert(pIdentColl.IndexOf(pIdent2) == 0);
			Debug.Assert(pIdentColl.IndexOf(pIdent3) == 1);
			Debug.Assert(pIdentColl.IndexOf(pIdent4) == -1);

			//Console.WriteLine("finish mgCommandTest.testIdentifierCollection");
			
		}

		public void testParameterValueCollection()
		{
			Console.WriteLine("start mgCommandTest.testParameterValueCollection");

			ParameterValue pVal;

			// create some data
			ParameterValueCollection pColl = new ParameterValueCollection();
			ParameterValue pVal1 = new ParameterValue("Name1");
			ParameterValue pVal2 = new ParameterValue();
			DoubleValue pLitVal = new DoubleValue(3.14);
			ParameterValue pVal3 = new ParameterValue("Name3", pLitVal);

			// Fill collection
			pColl.Insert(0, pVal1);
			pColl.Insert(1, pVal2);
			pColl.Insert(2, pVal3);

			// check collection
			Debug.Assert(pColl.Count == 3);

			Debug.Assert(pColl.Contains(pVal1));
			Debug.Assert(pColl.Contains(pVal2));
			Debug.Assert(pColl.Contains(pVal3));

			Debug.Assert(pColl.IndexOf(pVal1) == 0);
			Debug.Assert(pColl.IndexOf(pVal2) == 1);
			Debug.Assert(pColl.IndexOf(pVal3) == 2);

			pVal = pColl[0];
			Debug.Assert(pVal == pVal1);
			pVal = pColl[1];
			Debug.Assert(pVal == pVal2);
			pVal = pColl[2];
			Debug.Assert(pVal == pVal3);

			pVal = pColl.GetItem("Name3");
			Debug.Assert(pVal == pVal3);
			Debug.Assert(pColl.FindItem("Bogus") == null );
			
			//Console.WriteLine("finish mgCommandTest.testParameterValueCollection");
			
		}

		public void testPropertyValueCollection()
		{
			Console.WriteLine("start mgCommandTest.testPropertyValueCollection");

			PropertyValue   pVal;

			// create some data
			PropertyValueCollection pColl = new PropertyValueCollection();

			Identifier pIdent = new Identifier("Ident.For.Test");
			DoubleValue pDblVal = new DoubleValue(3.14);
			ByteValue pByteVal = new ByteValue(0x12);
			Int16Value pInt16Val = new Int16Value(123);

			PropertyValue pVal1 = new PropertyValue("Name1", pByteVal);
			PropertyValue pVal2 = new PropertyValue(pIdent, pInt16Val);
			PropertyValue pVal3 = new PropertyValue("Name3", pDblVal);

			// Fill collection
			pColl.Insert(0, pVal1);
			pColl.Insert(1, pVal2);
			pColl.Insert(2, pVal3);

			// check collection
			Debug.Assert(pColl.Count == 3);

			Debug.Assert(pColl.Contains(pVal1));
			Debug.Assert(pColl.Contains(pVal2));
			Debug.Assert(pColl.Contains(pVal3));

			Debug.Assert(pColl.IndexOf(pVal1) == 0);
			Debug.Assert(pColl.IndexOf(pVal2) == 1);
			Debug.Assert(pColl.IndexOf(pVal3) == 2);


			Debug.Assert(pColl.FindItem("Bogus") == null);

			// check that both item by index and name give same result
			pVal = pColl[0];
			Debug.Assert(pVal == pVal1);
			pVal = pColl.GetItem("Name1");
			Debug.Assert(pVal == pVal1);
			pVal = pColl.FindItem("Name1");
			Debug.Assert(pVal == pVal1);
			// and the rest are there
			pVal = pColl[1];
			Debug.Assert(pVal == pVal2);
			pVal = pColl[2];
			Debug.Assert(pVal == pVal3);

			//Console.WriteLine("finish mgCommandTest.testPropertyValueCollection");
			
		}
	}
}

