##
## Copyright (C) 2004-2007  Autodesk, Inc.
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

EXTRA_DIST = \
  UnitTest.vcproj \
  UnitTest.vcxproj \
  providers.xml

noinst_PROGRAMS = UnitTest

UnitTest_SOURCES = \
  ConnectTests.cpp \
  InsertTests.cpp \
  UpdateTests.cpp \
  DeleteTests.cpp \
  SelectTests.cpp \
  FdoExpressionFunctionTest.cpp \
  UnitTest.cpp \
  glibpatch.cpp

noinst_HEADERS = $(UnitTest_SOURCES) \
  ConnectTests.h \
  InsertTests.h \
  UpdateTests.h \
  DeleteTests.h \
  SelectTests.h \
  FdoExpressionFunctionTest.h

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOTHIRDPARTY)/linux/cppunit/include \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/ExpressionEngine/Inc \
  -I$(FDOUTILITIES)/TestCommon/Inc \
  -I../Provider

UnitTest_LDADD = \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDOUTILITIES)/TestCommon/libTestCommon.la \
  $(FDOUTILITIES)/ExpressionEngine/Src/libExpressionEngine.la \
  $(FDOTHIRDPARTY)/linux/cppunit/lib/libcppunit.a \
  $(FDO)/Unmanaged/Src/libFDO.la \
  -lxerces-c \
  -lxalan-c \
  -lxalanMsg

UnitTest_LDFLAGS = \
  -L$(FDOTHIRDPARTY)/apache/xalan/lib \
  -L$(FDOTHIRDPARTY)/apache/xerces/lib

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -D__STDC_LIMIT_MACROS


