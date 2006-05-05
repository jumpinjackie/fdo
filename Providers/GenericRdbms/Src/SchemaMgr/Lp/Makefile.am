AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libSmLpGrd.la

libSmLpGrd_la_SOURCES = \
  ClassDefinition.cpp \
  DataPropertyDefinition.cpp \
  GeometricPropertyDefinition.cpp \
  ObjectPropertyClass.cpp \
  ObjectPropertyDefinition.cpp \
  PropertyMappingConcrete.cpp \
  PropertyMappingSingle.cpp \
  Schema.cpp

INCLUDES = \
  -I.. \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I../../.. \
  -I../../../Inc \
  -I$(FDOUTILITIES)/Common/Inc

libSmLpGrd_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

