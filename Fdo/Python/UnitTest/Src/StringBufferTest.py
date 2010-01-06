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

from FDO import *
import unittest

class StringBufferTest(unittest.TestCase):
	"""
	Unit test for the StringBuffer utility class
	"""
	def testStringBufferBasics(self):
		buffer = StringBuffer()
		self.assert_( buffer.getLength() == 0)
		self.assert_( buffer.getCapacity() == 32)

		buffer.ensureCapacity(1000)
		self.assert_( buffer.getCapacity() == 1000)

		buffer.append("Hello world")
		self.assert_( buffer.toString() == "Hello world")
		self.assert_( buffer.getLength() == 11)

		buffer.insert(6, "Happy ")
		self.assert_( buffer.toString() == "Hello Happy world")
		self.assert_( buffer.getLength() == 17)

		del buffer
		print "\nTesting StringBufferTest..."
	
