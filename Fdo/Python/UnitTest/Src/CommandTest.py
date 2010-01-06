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


class CommandTest(unittest.TestCase):
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
		self.assertEquals(self._oConnection.__class__.__name__, "FdoIConnection")
		
		# Read the .ini file and configure the connection
		sConnectString = self._oConfig.get("Connection", "ConnectString")
		self._oConnection.SetConnectionString(sConnectString)
		self._oConnection.Open()

	
	def testSelectCommandCreation(self):
		"""Test the creation of the select command"""
		cmd = self._oConnection.CreateCommand(FdoCommandType_Select)
		self.assertEquals(cmd.__class__.__name__, "FdoICommand")
		cmd = FdoICommandToFdoISelect(cmd)
		self.assertEquals(cmd.__class__.__name__, "FdoISelect")
		self.assertEquals(cmd.Release(), 0)
		print "\nTesting CommandTest:SelectCommandCreation..."


	def testApplySchemaCommandCreation(self):
		"""Test the creation of the Apply Schema command"""
		cmd = self._oConnection.CreateCommand(FdoCommandType_ApplySchema)
		self.assertEquals(cmd.__class__.__name__, "FdoICommand")
		cmd = FdoICommandToFdoIApplySchema(cmd)
		self.assertEquals(cmd.__class__.__name__, "FdoIApplySchema")
		self.assertEquals(cmd.Release(), 0)
		print "\nTesting CommandTest:ApplySchemaCommandCreation..."

	def testInsertCommandCreation(self):
		"""Test the creation of the Insert command"""
		cmd = self._oConnection.CreateCommand(FdoCommandType_Insert)
		self.assertEquals(cmd.__class__.__name__, "FdoICommand")
		cmd = FdoICommandToFdoIInsert(cmd)
		self.assertEquals(cmd.__class__.__name__, "FdoIInsert")
		self.assertEquals(cmd.Release(), 0)
		print "\nTesting CommandTest:InsertCommandCreation..."


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
			self._oConnection.Close()
			self._oConnection.Release()


