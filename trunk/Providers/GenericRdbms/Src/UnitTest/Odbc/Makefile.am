##
## Copyright (C) 2004-2006  Autodesk, Inc.
## 
## This library is free software; you can redistribute it and/or
## modify it under the terms of version 2.1 of the GNU Lesser
## General Public License as published by the Free Software Foundation.
## 
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
##

AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = 

noinst_LTLIBRARIES = libOdbcUnitTests.la

libOdbcUnitTests_la_SOURCES = \
OdbcBaseSetup.cpp \
OdbcConnectionUtil.cpp \
OdbcDescribeSchemaTest.cpp \
OdbcFdoAdvancedSelectTest.cpp \
OdbcFdoConnectionInfoTest.cpp \
OdbcFdoConnectTest.cpp \
OdbcFdoDeleteTest.cpp \
OdbcFdoInsertTest.cpp \
OdbcFdoMultiThreadTest.cpp \
OdbcFdoSchemaTest.cpp \
OdbcFdoSelectTest.cpp \
OdbcFdoSqlCmdTest.cpp \
OdbcFdoUpdateTest.cpp

noinst_HEADERS = $(libOdbcUnitTests_la_SOURCES) \
OdbcBaseSetup.h \
OdbcConnectionUtil.h \
OdbcDescribeSchemaTest.h \
OdbcFdoAdvancedSelectTest.h \
OdbcFdoConnectionInfoTest.h \
OdbcFdoConnectTest.h \
OdbcFdoDeleteTest.h \
OdbcFdoInsertTest.h \
OdbcFdoMultiThreadTest.h \
OdbcFdoSchemaTest.h \
OdbcFdoSelectTest.h \
OdbcFdoSqlCmdTest.h \
OdbcFdoUpdateTest.h

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOTHIRDPARTY)/linux/cppunit/include \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../LockManager \
  -I../../LongTransactionManager \
  -I../../Gdbi \
  -I../../../Inc \
  -I../../.. \
  -I../../Fdo/Connection \
  -I../../Fdo/SpatialContext \
  -I../../Fdo/Other \
  -I../../ \
  -I../Common \
  -I../ \
  -I../../Fdo/FeatureCommands \
  -I$(FDOUTILITIES)/TestCommon/Inc

libOdbcUnitTests_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -DRDBI_STATIC 

