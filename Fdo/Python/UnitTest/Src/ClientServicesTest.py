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
import os

from FDO import *

import unittest

class ClientServicesTest(unittest.TestCase):
	"""
	Unit test for the ClientServices classes.  The Provider Registry and
	the FdoIConnections are tested.

	Additionally, the FdoProxyConnectionManager utility class is tested
	"""

	def testClientServices(self):
		print "\nTesting ClientServicesTest:ClientServices..."
		"""FeatureAccessManager accessor functions should return the correct type"""
		manager = FdoFeatureAccessManager.GetConnectionManager()
		registry = FdoFeatureAccessManager.GetProviderRegistry()
		providerCollection = registry.GetProviders()
        
		# Verify the instance classnames
		self.assert_(manager.__class__.__name__ == "IConnectionManager")
		self.assert_(registry.__class__.__name__ == "IProviderRegistry")
		self.assert_(providerCollection.__class__.__name__ == "FdoProviderCollection")


	def testConnectionCreation(self):
		print "\nTesting ClientServicesTest:ConnectionCreation..."
		"""Check that FdoIConnections can be created correctly"""
		manager = FdoFeatureAccessManager.GetConnectionManager()
		registry = FdoFeatureAccessManager.GetProviderRegistry()
		providerCollection = registry.GetProviders()
		
		# Iterate through each provider; instantiate the provider
		for index in range(providerCollection.GetCount()):
			provider = providerCollection.GetItem(index)

			name = provider.GetName()
			self.assert_( provider.__class__.__name__ == 'FdoProvider')
			
			# Unable to load the SDF provider for some reason.
			if name == "OSGeo.SDF.3.3":
				connection = manager.CreateConnection(name)
				self.assert_(connection.__class__.__name__ == 'FdoIConnection')
				self.assert_(connection.Release() == 0)
				
				connection = FdoProxyConnectionManager.CreateConnection(name)
				self.assert_(connection.__class__.__name__ == 'FdoIConnection')
				self.assert_(connection.Release() == 0)
					
				# Check if the library exists in the path
				self.assert_(provider.GetLibraryPath() == '.\SDFProvider.dll')
				