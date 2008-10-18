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
using OSGeo.FDO.Connections.Capabilities;
using OSGeo.FDO.Schema;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace Fdo_Test
{
	/// <summary>
	/// Summary description for mgCapabilitiesTest.
	/// </summary>
	public class mgCapabilitiesTest
	{
		public mgCapabilitiesTest()
		{
			Console.WriteLine("start mgCapabilitiesTest");
		}

		public void testFunctionDefinition()
		{
			Console.WriteLine("start mgCapabilitiesTest.testFunctionDefinition");

			//ArgumentDefinition
			ArgumentDefinition [] argue = new ArgumentDefinition[4];
			argue[0] = new ArgumentDefinition("Arg1", "Argument 1 (Boolean)", DataType.DataType_Boolean);
			argue[1] = new ArgumentDefinition("Arg2", null, DataType.DataType_Byte);
			argue[2] = new ArgumentDefinition("Arg3", "Argument 3 (BLOB)", DataType.DataType_BLOB);
			argue[3] = new ArgumentDefinition("Arg4", "Argument 4 (Int64)", DataType.DataType_Int64);

			Debug.Assert(argue[0].Name == "Arg1");
			Debug.Assert(argue[0].Description == "Argument 1 (Boolean)");
			Debug.Assert(argue[0].DataType == DataType.DataType_Boolean);
			Debug.Assert(argue[1].Name == "Arg2");
			Debug.Assert(argue[1].Description == string.Empty);  //in C++ test, it use NULL; but in .NET empty string is better than null
			Debug.Assert(argue[1].DataType == DataType.DataType_Byte);
			Debug.Assert(argue[2].Name == "Arg3");
			Debug.Assert(argue[2].Description == "Argument 3 (BLOB)");
			Debug.Assert(argue[2].DataType == DataType.DataType_BLOB);
			Debug.Assert(argue[3].Name == "Arg4");
			Debug.Assert(argue[3].Description == "Argument 4 (Int64)");
			Debug.Assert(argue[3].DataType == DataType.DataType_Int64);
		
			//ArgumentDefinitionCollection
			ArgumentDefinitionCollection collect = new ArgumentDefinitionCollection();
			for(int i  = 0; i < argue.Length; i ++)
			{
				collect.Add(argue[i]);
			}

			//FunctionDefinition
			FunctionDefinition fun = new FunctionDefinition("Function", "Test of FdoFunctionDefinition", DataType.DataType_Double, collect);
			
			//ReadOnlyArgumentDefinitionCollection
			ReadOnlyArgumentDefinitionCollection readonlyargues = fun.Arguments;

			Debug.Assert(readonlyargues.Count == argue.Length); 
			for(int i  = 0; i < readonlyargues.Count; i ++)
			{
				ArgumentDefinition arg = readonlyargues[i];
				Debug.Assert(arg == argue[i]);
			}
			Debug.Assert(readonlyargues.Contains(argue[0]))	;

			ArgumentDefinition arguement = readonlyargues["Arg2"];
			Debug.Assert(arguement != null);

			Debug.Assert(fun.ReturnType == DataType.DataType_Double);
			Debug.Assert(fun.Name == "Function");
			Debug.Assert(fun.Description == "Test of FdoFunctionDefinition");

			//Console.WriteLine("finish mgCapabilitiesTest.testFunctionDefinition");
			
		}
	}
}

