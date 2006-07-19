AUTOMAKE_OPTIONS = foreign

noinst_PROGRAMS = UnitTest

EXTRA_DIST = \
  UnitTest.vcproj \
  CubeServer_WMS_Config.xml \
  MultiLayers_Config.xml \
  NASA_WMS_Config.xml \
  providers.xml \
  WMSOverrideClass2.xml \
  WmsSchemaConfig.xml \
  WmsOverridesConfig.xml \
  WmsSchemaConfig_Jake_MapServer.xml \
  stdafx.h \
  Pch.cpp

UnitTest_SOURCES = \
  WmsTests.cpp \
  glibpatch.cpp \
  ConnectTests.cpp \
  CapabilityTest.cpp \
  UnitTest.cpp \
  OverridesTest.cpp \
  XmlFormatter.cpp \
  UnitTestUtil.cpp \
  WmsTestSelect.cpp \
  WmsTestDescribeSchema.cpp \
  WmsTestSpatialContexts.cpp

noinst_HEADERS = $(UnitTest_SOURCES) \
  CapabilityTest.h \
  ConnectTests.h \
  OverridesTest.h \
  Pch.h \
  UnitTestUtil.h \
  WmsTestDescribeSchema.h \
  WmsTests.h \
  WmsTestSelect.h \
  WmsTestSpatialContexts.h \
  XmlFormatter.h

INCLUDES = \
  -I./ \
  -I../ \
  -I../Message/Inc \
  -I../../Inc \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOTHIRDPARTY)/linux/cppunit/include

UnitTest_LDADD = \
  ../Provider/libWMSProviderSrc.la \
  ../Overrides/libWMSOverridesSrc.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDOTHIRDPARTY)/linux/cppunit/lib/libcppunit.a \
  $(FDO)/Unmanaged/Src/libFDO.la \
  -lxerces-c \
  -lxalan-c \
  -lxalanMsg \
  -lpthread_nonshared \
  -lpthread

UnitTest_LDFLAGS = \
  -L$(FDOTHIRDPARTY)/apache/xml-xalan/c/lib \
  -L$(FDOTHIRDPARTY)/apache/xml-xerces/c/lib

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -D__STDC_LIMIT_MACROS

