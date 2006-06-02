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

noinst_HEADERS = $(libSmLpGrd_la_SOURCES) \
  ClassDefinition.h \
  DataPropertyDefinition.h \
  GeometricPropertyDefinition.h \
  ObjectPropertyClass.h \
  ObjectPropertyDefinition.h \
  PropertyMappingConcrete.h \
  PropertyMappingSingle.h \
  Schema.h

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

