# File : \\otwqa\Providers\qa\tools\FdoPythonWrappers\UnitTests\lib\ApplySchemaTest.py
#
# Description: Defines the ApplySchema unit test class
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

# Import Python libs
import unittest
import traceback
import string
import os.path
from ConfigParser import *
import re

# Import FDO libs
from FDO import *
from SdfConnectionFactory import *
from SdfFileFactory import *


G_INI_FILENAME="./Input/UnitTestConfig.ini"


class ApplySchemaTest(unittest.TestCase):
	"""
	Unit test for FDO Command classes.  

	This class has the following dependencies:
	1. A config file called input/UnitTestConfig.ini must exist
	2. input/UnitTestConfig.ini must contain the following entries:

		[Connection]
		ConnectString=<A FDO connection string>
		ProviderName=<An FDO provider name. Can also be a regular expression>

	"""	
	_oConfig = ConfigParser()	
	_oConnection = None

	def setUp(self):
		"""
		Initialize resources required by the test.
		Creates, initializes and open a FdoIConnection

		Arguments:
			None

		Returns:
			none
		"""

		# Create the SDF file
		SdfFileFactory.createFromIni(G_INI_FILENAME, True)

		# Get a SDF connection
		self._oConfig.read([G_INI_FILENAME])
		self._oConnection = SdfConnectionFactory.create()		
		
		# Read the .ini file and configure the connection
		sConnectString = self._oConfig.get("Connection", "ConnectString")
		self._oConnection.SetConnectionString(sConnectString)
		self._oConnection.Open()

	
	def testApplySchema(self):
		"""Test the Apply Schema command against an SDF datasource"""
		self.assertEquals(self._oConnection.GetConnectionState(), FdoConnectionState_Open)
		
		# Apply the schema to the datastore
		oApplySchemaCmd = self._oConnection.CreateCommand(FdoCommandType_ApplySchema)
		oApplySchemaCmd = FdoICommandToFdoIApplySchema(oApplySchemaCmd)
		oApplySchemaCmd.SetFeatureSchema(self._createFeatureSchema("TestSchema"))
		oApplySchemaCmd.Execute()		
		oApplySchemaCmd.Release()		
		
		# Verify that the Feature Schema has been correctly created
		oDescribeSchemaCmd = self._oConnection.CreateCommand(FdoCommandType_DescribeSchema)
		oDescribeSchemaCmd = FdoICommandToFdoIDescribeSchema(oDescribeSchemaCmd)
		self.assertEquals(oDescribeSchemaCmd.__class__.__name__, "FdoIDescribeSchema")
		oSchemaCol = oDescribeSchemaCmd.Execute()
		oDescribeSchemaCmd.Release()
		self.assertEquals(oSchemaCol.GetCount(), 1)
		
		# Validate the properties of the schema
		oSchema = oSchemaCol.GetItem(0)
		self.assert_(oSchema.GetName(), 'TestSchema')		
		
		# Validate the classes in the schema
		oClasses = oSchema.GetClasses()
		self.assertEquals(oClasses.GetCount(), 2)
		
		# Validate the SewerPipe feature class
		oClass = oClasses.FindItem("SewerPipe")
		self.assert_(oClass.GetName(), 'SewerPipe')
		self.assert_(oClass.GetQualifiedName(), 'TestSchema:SewerPipe')
		
		# Validate the properties of the SewerPipe feature class
		oProperties = oClass.GetProperties()
		self.assertEquals(oProperties.GetCount(), 4)		
				
		# Cleanup
		oProperties.Release()
		oClass.Release()		
		oClasses.Release()
		oSchema.Release()
		oSchemaCol.Release()
		

	def _createFeatureSchema(self, sSchemaName):
		"""Creates a FDO Feature schema instance"""
		oNewSchema = FdoFeatureSchema.Create(sSchemaName, sSchemaName)
		oClasses = oNewSchema.GetClasses()
		
		# Create the classes to add to the schema
		# Note: if you add more classes here, you must update testApplySchema()
		oClasses.Add(self._createFeatureClass("SewerPipe"))
		oClasses.Add(self._createFeatureClass("WaterPipe"))				
		oClasses.Release()
		
		return oNewSchema	
	
	
	def _createFeatureClass(self, sClassName):
		"""
		Creates an FDO Feature class definition
		
		Note: if you add more properties here, you must update testApplySchema()
		"""
		
		# Initialize a FdoFeatureclass instance
		oNewClass = FdoFeatureClass.Create(sClassName, sClassName)
		oProperties = oNewClass.GetProperties()
		oIdentityProperties = oNewClass.GetIdentityProperties()
		
		# Create the identity property
		oIdProperty = self._createDataProperty("FeatureId", FdoDataType_Int32)
		oProperties.Add(oIdProperty)
		oIdentityProperties.Add(oIdProperty)
		oIdentityProperties.Release()
		
		# Create the properties for the class		
		oProperties.Add(self._createDataProperty("Length", FdoDataType_Double))
		oProperties.Add(self._createDataProperty("Diameter", FdoDataType_Double))
		oProperties.Add(self._createDataProperty("Owner", FdoDataType_String))		
			
		oProperties.Release()
		return oNewClass
		
		
	def _createDataProperty(self, sPropertyName, enumDataType):
		"""
		Creates a simple data property.  Default characteristics, such as length
		and scale are assigned.
		
		Arguments:
			sPropertyName (mandatory input)
				Name of the new property to create
				
			enumDataType (mandatory input)
				A FdoDataType value
				
		Returns:
			A FdoDataPropertyDefinition instance
		
		"""
		oNewProperty = FdoDataPropertyDefinition.Create(sPropertyName, sPropertyName)
		oNewProperty.SetDataType(enumDataType)
		oNewProperty.SetNullable(True)
		
		if enumDataType == FdoDataType_String:
			oNewProperty.SetLength(32)
			
		if enumDataType == FdoDataType_Decimal:
			oNewProperty.SetPrecision(16)
			oNewProperty.SetScale(4)
			
		return oNewProperty


	def tearDown(self):
		"""
		Releases any resources created or connected during the test

		Arguments:
			None

		Returns:
			none
		"""
		# Close any open FdoIConnections
		if not self._oConnection is None:
			if self._oConnection.__class__.__name__ == 'FdoIConnection':
				self._oConnection.Close()
				self._oConnection.Release()
		print "\nTesting ApplySchemaTest..."


