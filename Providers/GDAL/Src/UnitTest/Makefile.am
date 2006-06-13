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
  RfpTestBandConfig.cpp

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
  XmlFormatter.h

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

