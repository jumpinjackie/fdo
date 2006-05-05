AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libWMSOverridesSrc.la

libWMSOverridesSrc_la_SOURCES = \
  FdoWmsOvPhysicalSchemaMapping.cpp \
  FdoWmsOvClassCollection.cpp \
  FdoWmsOvClassDefinition.cpp \
  FdoWmsOvLayerCollection.cpp \
  FdoWmsOvLayerDefinition.cpp \
  FdoWmsOvStyleDefinition.cpp \
  FdoWmsOvRasterDefinition.cpp \
  FdoWmsOverrides.cpp

libWMSOverridesSrc_la_LDFLAGS =

INCLUDES = \
  -I./ \
  -I../ \
  -I../Message/Inc \
  -I../../Inc \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

