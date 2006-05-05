AUTOMAKE_OPTIONS = foreign

noinst_PROGRAMS = UnitTest

UnitTest_SOURCES = \
  UnitTest.cpp \
  glibpatch.cpp \
  MasterTest.cpp \
  AssociationSchemaTest.cpp \
  AssociationInsertTest.cpp \
  AssociationSelectTest.cpp \
  AssociationUpdateTest.cpp \
  AssociationDeleteTest.cpp \
  SelectTest.cpp

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOTHIRDPARTY)/linux/cppunit/include/ \
  -I../../Inc

UnitTest_LDADD = \
  $(FDO)/Unmanaged/Src/libFDO.la \
  -lcppunit \
  -ldl \
  -lxalan-c \
  -lxalanMsg \
  -lxerces-c

UnitTest_LDFLAGS = \
  -L$(FDOTHIRDPARTY)/linux/cppunit/lib \
  -L$(FDOTHIRDPARTY)/apache/xml-xalan/c/lib \
  -L$(FDOTHIRDPARTY)/apache/xml-xerces/c/lib

CXXFLAGS = @CXXFLAGS_SDF@

