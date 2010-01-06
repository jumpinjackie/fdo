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

class LOBTest(unittest.TestCase):
	"""
	Unit test for handling binary LOB values in Python
	"""
	def testLOBHandling(self):
		fileStream1 = FdoIoFileStream.Create( "Input/clock.avi", "rb" )
		fileReader1 = FdoIoByteStreamReader.Create( fileStream1 )	

		# Initialize the 'data' property value with the filestream reader
		propertyValue = FdoPropertyValue.Create()
		propertyValue.SetName("data")
		propertyValue.SetStreamReader( FdoIoByteStreamReaderToFdoIStreamReader(fileReader1) );

		streamReader = propertyValue.GetStreamReader()
		assert streamReader.GetType() == FdoStreamReaderType_Byte
		print "\nTesting LOBTest..."
