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
import sys
from FDO import *
import unittest

class TypeMapTest(unittest.TestCase):
	"""
	Unit test for SWIG typemaps.  The typemaps handle the marshalling
	of C++ datatypes into the Python environment.  The marshalling
	includes:
		- converting wchar_t* into a Python string
		- converting enumerations into Python numbers
		- converting int* to Python lists, whose first item is a tuple of
		immutable string values, and the second item is the size of the tuple
		- converting wchar_t** to Python lists, whose first item is a tuple
		of strings, and the second item is the size of the tuple
	"""
	def testPrimitives(self):
		"""
		Test the marshalling of primitives back and forth between
		Python and C++
		"""
		
		# Check doubles
		doubleVal = FdoDoubleValue.Create(1234.123212)
		self.assert_(doubleVal.GetDouble() == 1234.123212)
		doubleVal.Release()

		# Check int 64s
		int64Val = FdoInt64Value.Create()
		int64Val.SetInt64(123)
		self.assert_(int64Val.GetInt64() == 123)
		int64Val.Release()
		
		# Check int 32s
		int32Val = FdoInt32Value.Create()
		int32Val.SetInt32(12345)
		self.assert_(int32Val.GetInt32() == 12345)
		int32Val.Release()
	
		# Check int 16s	
		int16Val = FdoInt16Value.Create()
		int16Val.SetInt16(12345)
		self.assert_(int16Val.GetInt16() == 12345)
		int16Val.Release()
		
		# Check utf-8 strings
		stringVal = FdoStringValue.Create()
		stringVal.SetString("Hello World")
		self.assert_(stringVal.ToString() == "'Hello World'")
		stringVal.Release()
		
		# Check unicode strings
		stringVal = FdoStringValue.Create()
		stringVal.SetString(u"Andr\202 Encyclop\221dia")		
		uString = stringVal.ToString().encode('ascii', 'xmlcharrefreplace')		
		self.assert_(uString == "'Andr&#130; Encyclop&#145;dia'")
		stringVal.Release()
		print "\nTesting TypeMapTest:Primitives..."
		
	def testExceptions(self):
		"""
		Verify the marshalling of FdoException objects
		"""
		try:
			print "\nTesting TypeMapTest:Exceptions..."
			FdoProxyConnectionManager.CreateConnection("This doesn't exist")
		except:
			excType, excValue, excTraceback = sys.exc_info()			
			self.assert_(str(excType) == "FDOw.FdoException")			
			return None
			
		# An exception was expected here.  Raise an exception
		raise AssertionError("Exception expected")

	def testEnumerations(self):
		"""
		Test the marshalling of FDO enumerated types into
		Python integers.  These are a spotcheck for
		most of the FDO enumerated types.
		"""	
		self.assert_(FdoDataType_Boolean == 0)
		self.assert_(FdoDataType_Int32 == 6	)
		self.assert_(FdoConnectionState_Closed == 1)
		self.assert_(FdoClassType_NetworkLinkClass == 5)
		self.assert_(FdoGeometryType_MultiCurveString == 12)
		print "\nTesting TypeMapTest:Enumerations..."


	def testPrimitiveArrays(self):
		"""
		Test the marshalling of int* arrays into Python lists.
		Most of these typemaps are required in the FdoCapabilities API
		"""	
		provider = self._getSdfProvider()
		connection = FdoProxyConnectionManager.CreateConnection(provider.GetName())

		# Check the typemapping for FdoICommandCapabilities::GetCommands()
		cmdCapabilities = connection.GetCommandCapabilities()
		self._validateIntList(cmdCapabilities.GetCommands())
		cmdCapabilities.Release()

		# Check the typemapping for FdoIConnectionCapabilities::GetLockTypes()
		# and FdoIConnectionCapabilities::GetSpatialContextTypes() 
		conCapabilities = connection.GetConnectionCapabilities()
		self._validateIntList(conCapabilities.GetLockTypes())
		self._validateIntList(conCapabilities.GetSpatialContextTypes())
		conCapabilities.Release() 	

		# Check the typemapping for FdoIExpressionCapabilities::GetExpressionTypes()
		expCapabilities = connection.GetExpressionCapabilities()
		self._validateIntList(expCapabilities.GetExpressionTypes())
		expCapabilities.Release()
		
		# Check the typemapping for FdoIFilterCapabilities::GetConditionTypes()
		# and FdoIFilterCapabilities::GetDistanceOperations()
		filterCapabilities = connection.GetFilterCapabilities()
		self._validateIntList(filterCapabilities.GetConditionTypes())
		self._validateIntList(filterCapabilities.GetDistanceOperations())
		self._validateIntList(filterCapabilities.GetSpatialOperations())
		filterCapabilities.Release()
	
		# Check the typemapping for FdoIGeometryCapabilities::GetGeometryTypes()
		# and FdoIGeometryCapabilities::GetGeometryComponentTypes()
		geoCapabilities = connection.GetGeometryCapabilities()
		self._validateIntList(geoCapabilities.GetGeometryTypes())
		self._validateIntList(geoCapabilities.GetGeometryComponentTypes())
		geoCapabilities.Release()

		# Check the typemapping for FdoISchemaCapabilities::GetClassTypes()
		# and FdoISchemaCapabilities::GetDataTypes()
		schemaCapabilities = connection.GetSchemaCapabilities()
		self._validateIntList(schemaCapabilities.GetClassTypes())
		self._validateIntList(schemaCapabilities.GetDataTypes())
		self._validateIntList(schemaCapabilities.GetSupportedAutoGeneratedTypes())
		schemaCapabilities.Release()
		
		# Check the typemapping for FdoIConnectionPropertyDictionary::GetPropertyNames()		
		connectionInfo = connection.GetConnectionInfo()
		properties = connectionInfo.GetConnectionProperties()
		
		# !!! The following test is failing !!!
		self._validateUnicodeList(properties.GetPropertyNames())
		
		properties.Release()
		connectionInfo.Release()
		print "\nTesting TypeMapTest:PrimitiveArrays..."
		

	def _validateIntList(self, capabilityList):
		"""
		Validate the output from the FDO capabilities API.  There is a 
		'family' of methods with a signature like this:

		GisInt* GetSomething(GisInt32& length)

		This output consists of two things:
			- a C++ 'array' of integers or enumerations
			- the length of the array

		The typemapping in Python will translate the output into
		a two-element List with two things:
			- a Python Tuple containing the contents of the C++ array
			- the length of the array as a number

		This function validates that the first item in the list is a
		Tuple, and the second value is an integer.

		Arguments:
			capabilityList - the list created by an FDO Get... function

		Returns:
			None

		Raises an exception is there's a problem
		"""
		self.assert_(isinstance(capabilityList, list))
		self.assert_(isinstance(capabilityList[0], tuple))
		self.assert_(isinstance(capabilityList[1], int))
		self.assert_(len(capabilityList[0]) == capabilityList[1])
		

	def _validateUnicodeList(self, capabilityList):
		"""
		Validate the output from the FDO capabilities API.
		There is a  'family' of methods with a signature like this:

		wchar_t** GetSomething(GisInt32& length)

		This output consists of two things:
			- a C++ 'array' of wide char strings
			- the length of the array

		The typemapping in Python will translate the output into
		a two-element List with two things:
			- a Python Tuple containing the contents of the C++ array
			- the length of the array as a number

		This function validates that the first item in the list is a
		Tuple, and the second value is an integer.

		Arguments:
			capabilityList - the list created by an FDO Get... function

		Returns:
			None

		Raises an exception is there's a problem
		"""		
		self.assert_(isinstance(capabilityList, list))
		self.assert_(isinstance(capabilityList[0], tuple))				
		self.assert_(isinstance(capabilityList[1], int))
		self.assert_(len(capabilityList[0]) == capabilityList[1])
		
		# If the capability list has at least one item, check the type
		if capabilityList[1] > 0:
			self.assert_(isinstance(capabilityList[0][0], unicode))


	def _getSdfProvider(self):
		"""
		Returns an instance of the SDF provider.
		"""

		retVal = None

		registry = FdoFeatureAccessManager.GetProviderRegistry()
		providers = registry.GetProviders()

		# Iterate through the provider registry, and find
		# providers with 'Sdf' in their name
		for index in range(providers.GetCount()):
			provider = providers.GetItem(index)

			if string.find(provider.GetName(), "OSGeo.SDF.3.5") > -1:
				retVal = provider
				break
	
		if provider is None:
			raise RuntimeError("Cannot find the SDF provider")

		return provider
