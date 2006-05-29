AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = ShpOverrides.vcproj FdoShpOverrides.rc resource.h 

noinst_LTLIBRARIES = libSHPOverridesSrc.la

libSHPOverridesSrc_la_SOURCES = \
  FdoShpPropertyDefinition.cpp \
  FdoShpColumnDefinition.cpp \
  FdoShpPhysicalSchemaMapping.cpp \
  FdoShpClassDefinition.cpp \
  FdoShpClassCollection.cpp \
  FdoShpPropertyDefinitionCollection.cpp

noinst_HEADERS = $(libSHPOverridesSrc_la_SOURCES) \
  FdoShpXmlGlobals.h \
  stdafx.h

libSHPOverridesSrc_la_LDFLAGS = 

INCLUDES = \
  -I../../Inc \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

