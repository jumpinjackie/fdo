AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = \
  UnitTest.vcproj \
  providers.xml

noinst_PROGRAMS = UnitTest

UnitTest_SOURCES = \
  UnitTest.cpp \
  glibpatch.cpp \
  MasterTest.cpp \
  ApplySchemaTest.cpp \
  AssociationSchemaTest.cpp \
  AssociationInsertTest.cpp \
  AssociationSelectTest.cpp \
  AssociationUpdateTest.cpp \
  AssociationDeleteTest.cpp \
  ExtendedSelectTest.cpp \
  NlsTest.cpp \
  SelectTest.cpp \
  UnitTestUtil.cpp

noinst_HEADERS = $(UnitTest_SOURCES) \
  ApplySchemaTest.h \
  AssociationDeleteTest.h \
  AssociationInsertTest.h \
  AssociationSchemaTest.h \
  AssociationSelectTest.h \
  AssociationUpdateTest.h \
  ExtendedSelectTest.h \
  FdoMultiThreadTest.h \
  FgfStreamWriter.h \
  MasterTest.h \
  NlsTest.h \
  SelectTest.h \
  UnitTestUtil.h \
  tester-linux.h

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOTHIRDPARTY)/linux/cppunit/include/ \
  -I../../Inc

UnitTest_LDADD = \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(FDOTHIRDPARTY)/linux/cppunit/lib/libcppunit.a \
  -ldl \
  -lxalan-c \
  -lxalanMsg \
  -lxerces-c

UnitTest_LDFLAGS = \
  -L$(FDOTHIRDPARTY)/apache/xml-xalan/c/lib \
  -L$(FDOTHIRDPARTY)/apache/xml-xerces/c/lib

CXXFLAGS = @CXXFLAGS_SDF@

