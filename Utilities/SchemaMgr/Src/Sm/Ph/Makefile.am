##
## Copyright (C) 2004-2006  Autodesk, Inc.
## 
## This library is free software; you can redistribute it and/or
## modify it under the terms of version 2.1 of the GNU Lesser
## General Public License as published by the Free Software Foundation.
## 
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
##

AUTOMAKE_OPTIONS = foreign

SUBDIRS = Cfg Mt Rb Rd

noinst_LTLIBRARIES = libSmPhysical.la

libSmPhysical_la_SOURCES = \
  AssociationReader.cpp \
  AssociationWriter.cpp \
  BaseObject.cpp \
  CharacterSet.cpp \
  ClassPropertyReader.cpp \
  ClassPropertySADReader.cpp \
  ClassReader.cpp \
  ClassSADReader.cpp \
  ClassSOReader.cpp \
  ClassSOWriter.cpp \
  ClassWriter.cpp \
  Collation.cpp \
  Column.cpp \
  ColumnChar.cpp \
  ColumnDate.cpp \
  ColumnDecimal.cpp \
  ColumnGeom.cpp \
  ColumnList.cpp \
  ColumnUnknown.cpp \
  ColumnVarlen.cpp \
  CommandWriter.cpp \
  CoordinateSystem.cpp \
  CoordinateSystemCollection.cpp \
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
  MergeReader.cpp \
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
  SpatialContext.cpp \
  SpatialContextCollection.cpp \
  SpatialContextGeom.cpp \
  SpatialContextGeomReader.cpp \
  SpatialContextGeomWriter.cpp \
  SpatialContextGroupReader.cpp \
  SpatialContextGroupWriter.cpp \
  SpatialContextReader.cpp \
  SpatialContextWriter.cpp \
  SpatialIndex.cpp \
  stdafx.cpp \
  Table.cpp \
  TableComponentReader.cpp \
  TableMergeReader.cpp \
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

