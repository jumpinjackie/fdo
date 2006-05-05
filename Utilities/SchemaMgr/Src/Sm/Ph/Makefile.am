AUTOMAKE_OPTIONS = foreign

SUBDIRS = Cfg Mt Rb Rd

noinst_LTLIBRARIES = libSmPhysical.la

libSmPhysical_la_SOURCES = \
  AssociationReader.cpp \
  AssociationWriter.cpp \
  ClassPropertyReader.cpp \
  ClassPropertySADReader.cpp \
  ClassReader.cpp \
  ClassSADReader.cpp \
  ClassSOReader.cpp \
  ClassSOWriter.cpp \
  ClassWriter.cpp \
  Column.cpp \
  ColumnChar.cpp \
  ColumnDate.cpp \
  ColumnDecimal.cpp \
  ColumnGeom.cpp \
  ColumnUnknown.cpp \
  ColumnVarlen.cpp \
  CommandWriter.cpp \
  Database.cpp \
  DbElement.cpp \
  DbObject.cpp \
  Dependency.cpp \
  DependencyReader.cpp \
  DependencyWriter.cpp \
  Field.cpp \
  Fkey.cpp \
  GroupReader.cpp \
  Index.cpp \
  LockTypes.cpp \
  Mgr.cpp \
  OptionsReader.cpp \
  OptionsWriter.cpp \
  Owner.cpp \
  PropertyReader.cpp \
  PropertyWriter.cpp \
  Reader.cpp \
  ReadWrite.cpp \
  Row.cpp \
  SADReader.cpp \
  SADWriter.cpp \
  SchemaElement.cpp \
  SchemaReader.cpp \
  SchemaSOReader.cpp \
  SchemaSOWriter.cpp \
  SchemaWriter.cpp \
  SOReader.cpp \
  SOWriter.cpp \
  SpatialContextGeomReader.cpp \
  SpatialContextGeomWriter.cpp \
  SpatialContextGroupReader.cpp \
  SpatialContextGroupWriter.cpp \
  SpatialContextReader.cpp \
  SpatialContextWriter.cpp \
  SpatialIndex.cpp \
  stdafx.cpp \
  Table.cpp \
  TempObject.cpp \
  View.cpp \
  Writer.cpp


INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../../Inc

libSmPhysical_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

