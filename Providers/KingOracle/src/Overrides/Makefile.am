AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libShpOverridesSrc.la

libShpOverridesSrc_la_SOURCES = \
  FdoShpPropertyDefinition.cpp \
  FdoShpColumnDefinition.cpp \
  FdoShpPhysicalSchemaMapping.cpp \
  FdoShpClassDefinition.cpp \
  FdoShpClassCollection.cpp \
  FdoShpPropertyDefinitionCollection.cpp

libShpOverridesSrc_la_LDFLAGS = 

INCLUDES = \
  -I../../inc \
  -I../../../../fdo/Unmanaged/inc \
  -I../../../FdoCommon/inc

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

