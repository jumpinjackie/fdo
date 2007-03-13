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
import traceback
import os.path
import re

# Import FDO libs
from FDO import *

class SdfConnectionFactory(object):
	"""
	A factory class used to create FdoIConnection instances
	of SDF Provider connections
	"""
	def create():
		"""
		Creates an SDF connection object

		Arguments:
			None

		Returns:	
			An FdoIConnection object instance
		"""		

		oProvider = SdfConnectionFactory._getProvider("OSGeo.SDF.3.3")
		oManager = FdoFeatureAccessManager.GetConnectionManager()
		oConnection = oManager.CreateConnection(oProvider.GetName())
		return oConnection

	# Make create() as static method
	create = staticmethod(create)


	def _getProvider(sProviderName):
		"""
		Retrieves a FdoProvider instance for a FDO provider.
		The provider name can be specified as a name, or
		a regular expression

		Arguments:
			sProviderName (mandatory input)
				- Name of the provider to get. Can also be a regular expression

		Returns:
			A list of FdoProvider instances that matches
			the provider name or the regular expression
		"""
			
		oRegistry = FdoFeatureAccessManager.GetProviderRegistry()
		oRePattern = re.compile(sProviderName)
		
		try:
			oProviders = oRegistry.GetProviders()
			oSdfProvider = None
	
			# Interrogate the Provider oRegistry for the SDF oProvider
			for index in range(oProviders.GetCount()):
				oProvider = oProviders.GetItem(index)
				if not oRePattern.match(oProvider.GetName()) is None:
					oSdfProvider = oProvider
					break
	
			if oSdfProvider is None:
				raise RuntimeError("Cannot find SDF oProvider in the oProvider oRegistry")
	
			return oSdfProvider	
		
		finally:
			del oRePattern

	# Make _getProvider a static method
	_getProvider = staticmethod(_getProvider)


