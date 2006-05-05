AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libSmOverrides.la

libSmOverrides_la_SOURCES = \
  TableMappingTypeMapper.cpp \
  GeometricColumnTypeMapper.cpp \
  GeometricContentTypeMapper.cpp \
  stdafx.cpp

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../../Inc

libSmOverrides_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

