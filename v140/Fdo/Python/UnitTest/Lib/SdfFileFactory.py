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
from ConfigParser import *

# Import FDO libs
from SdfConnectionFactory import *
from SdfFileCreationParams import *


class SdfFileFactory(object):
	def create(oParams, bOverwrite=False):	
		"""
		Creates an SDF file.

		Arguments:
			oParams (mandatory input):
				A SdfFileCreationParams instance

			bOverwrite (optional input):
				If True, overwrites the SDF file, if it exists.
				If False, any existing SDF file will not be overwritten.
					If a SDF file exists, an exception is thrown.

		Returns:
			None
		"""

		# Delete the existing SDF file, if it exists
		if bOverwrite:
			if os.path.exists(oParams.fileName):
				os.remove(oParams.fileName)			

		# Use the SDF connection to create the SDF file
		oConnection = SdfConnectionFactory.create()
		oCmd = oConnection.CreateCommand(FdoCommandType_CreateDataStore)
		oSCCmd = oConnection.CreateCommand(FdoCommandType_CreateSpatialContext)
		
		try:
			oCmd = FdoICommandToFdoICreateDataStore(oCmd)
			
			oDictionary = oCmd.GetDataStoreProperties()

			oPropertyNames, iPropertyNamesCount = oDictionary.GetPropertyNames()
			for i in range (iPropertyNamesCount):
				if oPropertyNames[i] == 'File': 
					oDictionary.SetProperty(oPropertyNames[i], oParams.fileName)

			oCmd.Execute()
			
			oDictionary.Release()
			
			oConnectionInfo = oConnection.GetConnectionInfo()
			cConnectionProperties = oConnectionInfo.GetConnectionProperties()
			cConnectionProperties.SetProperty('File', oParams.fileName)
			oConnection.Open()

			cConnectionProperties.Release()
			oConnectionInfo.Release()
			
			oSCCmd = FdoICommandToFdoICreateSpatialContext(oSCCmd)
			
			oSCCmd.SetName(oParams.spatialContextName)
			oSCCmd.SetDescription(oParams.spatialContextDescription)
			oSCCmd.SetCoordinateSystemWkt(oParams.coordinateSystemWKT)
			oSCCmd.SetXYTolerance(oParams.XYTolerance)
			oSCCmd.SetZTolerance(oParams.ZTolerance)
			
			oSCCmd.Execute()

		finally:
			oCmd.Release()
			oSCCmd.Release()
			oConnection.Release()

	create = staticmethod(create)


	def createFromIni(sFileName, bOverwrite=False, sSectionName="SDFFile"):
		"""
		Creates an SDF file based on the parameters
		found in a .ini file

		Arguments:
			sFileName (mandatory input):
				The relative pathname of the .ini file

			sSectionName (optional input):
				The name of the .ini file section where the connection
				params exist

		Returns:
			None
		"""
		params = SdfFileCreationParams(sFileName, sSectionName)
		try:
			SdfFileFactory.create(params, bOverwrite)	

		finally:
			del params 
		

	createFromIni = staticmethod(createFromIni)

