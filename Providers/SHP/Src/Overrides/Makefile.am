AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libSHPOverridesSrc.la

libSHPOverridesSrc_la_SOURCES = \
  FdoShpPropertyDefinition.cpp \
  FdoShpColumnDefinition.cpp \
  FdoShpPhysicalSchemaMapping.cpp \
  FdoShpClassDefinition.cpp \
  FdoShpClassCollection.cpp \
  FdoShpPropertyDefinitionCollection.cpp

libSHPOverridesSrc_la_LDFLAGS = 

INCLUDES = \
  -I../../Inc \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

