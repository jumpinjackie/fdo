AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libSmPhRd.la

libSmPhRd_la_SOURCES = \
  AssociationReader.cpp \
  ClassReader.cpp \
  ColumnReader.cpp \
  ConstraintReader.cpp \
  CoordSysReader.cpp \
  DbObjectBinds.cpp \
  DbObjectReader.cpp \
  FkeyReader.cpp \
  IndexReader.cpp \
  Join.cpp \
  OwnerReader.cpp \
  PkeyReader.cpp \
  PropertyReader.cpp \
  QueryReader.cpp \
  SchemaReader.cpp \
  SpatialContextGeomReader.cpp \
  SpatialContextGroupReader.cpp \
  SpatialContextReader.cpp \
  TableJoin.cpp


INCLUDES = \
  -I.. \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../../../Inc

libSmPhRd_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

