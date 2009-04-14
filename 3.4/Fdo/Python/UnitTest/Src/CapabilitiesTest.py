#
# Copyright (C) 2004-2007  Autodesk, Inc.
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of version 2.1 of the GNU Lesser
# General Public License as published by the Free Software Foundation.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#

import traceback
import string
import os.path
from FDO import *
import unittest

class CapabilitiesTest(unittest.TestCase):
	"""
	Unit test for handling FDO capabilities in Python
	"""
	def testCapabilities(self):
		print "\nTesting CapabilitiesTest:Capabilities..."

		# ArgumentDefinition
		argue1 = FdoArgumentDefinition.Create("Arg1", "Argument 1 (Boolean)", FdoDataType_Boolean)
		argue2 = FdoArgumentDefinition.Create("Arg2", "Argument 2 (Byte)", FdoDataType_Byte);
		argue3 = FdoArgumentDefinition.Create("Arg3", "Argument 3 (BLOB)", FdoDataType_BLOB);
		argue4 = FdoArgumentDefinition.Create("Arg4", "Argument 4 (Int64)", FdoDataType_Int64);

		self.assert_(argue1.GetName(), "Arg1")
		self.assert_(argue1.GetDescription(), "Argument 1 (Boolean)")
		self.assertEquals(argue1.GetDataType(), FdoDataType_Boolean)
		self.assert_(argue2.GetName(), "Arg2")
		self.assert_(argue2.GetDescription(), "Argument 2 (Byte)")
		self.assertEquals(argue2.GetDataType(), FdoDataType_Byte)
		self.assert_(argue3.GetName(), "Arg3")
		self.assert_(argue3.GetDescription(), "Argument 3 (BLOB)")
		self.assertEquals(argue3.GetDataType(), FdoDataType_BLOB)
		self.assert_(argue4.GetName(), "Arg4")
		self.assert_(argue4.GetDescription(), "Argument 4 (Int64)")
		self.assertEquals(argue4.GetDataType(), FdoDataType_Int64)

		# ArgumentDefinitionCollection
		collect = FdoArgumentDefinitionCollection.Create()
		collect.Add(argue1)
		collect.Add(argue2)
		collect.Add(argue3)
		collect.Add(argue4)

		# FunctionDefinition
		fun = FdoFunctionDefinition.Create("Function", "Test of FdoFunctionDefinition", FdoDataType_Double, collect)

		# ReadOnlyArgumentDefinitionCollection
		readonlyargues = fun.GetArguments()
		count = readonlyargues.GetCount()
		self.assertEquals(count, 4)

		arg = readonlyargues.GetItem(0)
		self.assert_(arg.GetName, argue1)

		self.assert_(readonlyargues.Contains(argue1))

		arguement = readonlyargues.GetItem("Arg2");
		self.assert_(arguement.GetName(), "Arg2");

		self.assert_(fun.GetReturnType(), FdoDataType_Double);
		self.assert_(fun.GetName(), "Function");
		self.assert_(fun.GetDescription(), "Test of FdoFunctionDefinition");

		argue1.Release()
		argue2.Release()
		argue3.Release()
		argue4.Release()
		collect.Release()
