AUTOMAKE_OPTIONS = foreign

noinst_PROGRAMS = UnitTest

UnitTest_SOURCES = \
  app.cpp \
  CapabilitiesTest.cpp \
  ClientServicesTest.cpp \
  CommandTest.cpp \
  CommonTest.cpp \
  ExpressionParseTest.cpp \
  ExpressionTest.cpp \
  FilterParseTest.cpp \
  FilterTest.cpp \
  GeometryTest.cpp \
  GeometryMemPerfTest.cpp \
  IoTest.cpp \
  Pch.cpp \
  SchemaTest.cpp \
  SpatialContextTest.cpp \
  TestProcessor.cpp \
  UnitTestUtil.cpp \
  XmlFormatter.cpp \
  XmlTest.cpp \
  GmlWriteTest.cpp

INCLUDES = \
  -I../Unmanaged/Inc \
  -I$(FDOTHIRDPARTY)/linux/cppunit/include

UnitTest_LDADD = \
  $(FDOTHIRDPARTY)/linux/cppunit/lib/libcppunit.a \
  ../Unmanaged/Src/libFDO.la

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

