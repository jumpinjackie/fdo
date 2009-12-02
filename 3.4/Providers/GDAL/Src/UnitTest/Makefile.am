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

EXTRA_DIST = \
  UnitTest.vcproj \
  1.xml \
  4_4.xml \
  example1.xml \
  ManySpatialContextTest.xml \
  providers.xml \
  RfpOverridesConfig.xml \
  thoroughTest.xml \
  UnitTestWin.cpp \
  stdafx.cpp

noinst_PROGRAMS = UnitTest

UnitTest_SOURCES = \
  XmlFormatter.cpp \
  UnitTestUtil.cpp \
  RfpNoConfigTest.cpp \
  RfpOverridesSerializeTest.cpp \
  RfpRasterConversion.cpp \
  RfpTestExample1.cpp \
  RfpTestGeotiff.cpp \
  RfpTestGetRaster.cpp \
  RfpTestMosaic.cpp \
  RfpTestSpatialContext.cpp \
  RfpTestThorough.cpp \
  RfpUnitTest.cpp \
  UnitTest.cpp \
  UnitTestLinux.cpp \
  RfpTestMrSID.cpp \
  RfpTestBand.cpp \
  RfpTestJp2.cpp \
  RfpTestEcw.cpp \
  RfpTestDem.cpp \
  RfpTestBandConfig.cpp \
  RfpTestPng.cpp \
  RfpTestJpg.cpp \
  RfpTestAAIGrid.cpp \
  RfpTestSpatialQuery.cpp

noinst_HEADERS = $(UnitTest_SOURCES) \
  RfpNoConfigTest.h \
  RfpOverridesSerializeTest.h \
  RfpTestBand.h \
  RfpTestBandConfig.h \
  RfpTestCase.h \
  RfpTestDem.h \
  RfpTestEcw.h \
  RfpTestExample1.h \
  RfpTestGeotiff.h \
  RfpTestGetRaster.h \
  RfpTestJp2.h \
  RfpTestLogOutputter.h \
  RfpTestMosaic.h \
  RfpTestMrSID.h \
  RfpTestRasterConversion.h \
  RfpTestSpatialContext.h \
  RfpTestThorough.h \
  stdafx.h \
  UnitTest.h \
  UnitTestUtil.h \
  XmlFormatter.h \
  RfpTestPng.h \
  RfpTestJpg.h \
  RfpTestAAIGrid.h \
  RfpTestSpatialQuery.h

INCLUDES = \
  -I.. \
  -I../../Inc \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOTHIRDPARTY)/linux/cppunit/include

UnitTest_LDADD = \
  ../Overrides/libGRFPOverridesSrc.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDOTHIRDPARTY)/linux/cppunit/lib/libcppunit.a \
  $(FDO)/Unmanaged/Src/libFDO.la \
  -lxerces-c \
  -lxalan-c \
  -lxalanMsg

UnitTest_LDFLAGS = \
  -L$(FDOTHIRDPARTY)/apache/xml-xalan/c/lib \
  -L$(FDOTHIRDPARTY)/apache/xml-xerces/c/lib

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -D__STDC_LIMIT_MACROS

