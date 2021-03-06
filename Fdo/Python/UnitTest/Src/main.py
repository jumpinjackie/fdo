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
import sys
import traceback
import unittest

# Import unit tests
from StringBufferTest import *
from ClientServicesTest import *
from TypeMapTest import *
from LOBTest import *
from ApplySchemaTest import *
from CommandTest import *
from CapabilitiesTest import *

if __name__ == "__main__":
	suite = unittest.TestSuite()
	suite.addTest(StringBufferTest)
	suite.addTest(ClientServicesTest)
	suite.addTest(TypeMapTest)
	suite.addTest(LOBTest)
	suite.addTest(ApplySchemaTest)
	suite.addTest(CommandTest)
	suite.addTest(CapabilitiesTest)

	unittest.main()

