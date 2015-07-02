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

from ConfigParser import *

class SdfFileCreationParams(object):
	fileName = ""
	coordinateSystemWKT = ""
	spatialContextDescription  = ""
	spatialContextName  = ""
	XYTolerance = 0
	ZTolerance = 0
	
	def __init__(self, sFileName=None, sSectionName=None):
		if sFileName is None:
			return none

		# Initialze the object through the .ini file
		oConfig = ConfigParser()

		try:
			oConfig.read([sFileName])
			self.fileName = oConfig.get(sSectionName, "FileName")
			self.coordinateSystemWKT = oConfig.get(sSectionName, "CoordinateSystemWKT")
			self.spatialContextName = oConfig.get(sSectionName, "SpatialContextName")
			self.spatialContextDescription = oConfig.get(sSectionName, "SpatialContextDescription")
			self.XYTolerance = float(oConfig.get(sSectionName, "XYTolerance"))
			self.ZTolerance = float(oConfig.get(sSectionName, "ZTolerance"))

		finally:
			del oConfig


