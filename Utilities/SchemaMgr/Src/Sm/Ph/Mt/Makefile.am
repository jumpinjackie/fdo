AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libSmPhMt.la

libSmPhMt_la_SOURCES = \
  AssociationReader.cpp \
  ClassReader.cpp \
  PropertyReader.cpp \
  SchemaReader.cpp \
  SpatialContextGeomReader.cpp \
  SpatialContextGroupReader.cpp \
  SpatialContextReader.cpp


INCLUDES = \
  -I.. \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../../../Inc
  
libSmPhMt_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

