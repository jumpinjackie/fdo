AUTOMAKE_OPTIONS = foreign

noinst_PROGRAMS = UnitTest

UnitTest_SOURCES = \
  UnitTest.cpp \
  glibpatch.cpp \
  ShpTests.cpp \
  FileAccessTests.cpp \
  ConnectTests.cpp \
  IndexTests.cpp \
  SelectTests.cpp \
  SelectAggregatesTests.cpp \
  InsertTests.cpp \
  DeleteTests.cpp \
  SchemaTests.cpp \
  OverridesTest.cpp \
  FilterTests.cpp \
  SpatialFilterTests.cpp \
  XmlFormatter.cpp \
  PerformanceTests.cpp \
  UpdateTests.cpp \
  FidelityTests.cpp \
  UnitTestUtil.cpp \
  BigPerformanceTests.cpp

INCLUDES = \
  -I../ShpRead \
  -I../ShpSpatialIndex \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOTHIRDPARTY)/linux/cppunit/include \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../Inc  \
  -I../Provider

UnitTest_LDADD = \
  ../ShpRead/libSHPRead.la \
  ../ShpSpatialIndex/libSHPSpatialIndex.la \
  ../Overrides/libSHPOverridesSrc.la \
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

