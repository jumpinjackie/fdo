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

noinst_LTLIBRARIES = libSmLogicalPhysical.la

libSmLogicalPhysical_la_SOURCES = \
  AssociationPropertyDefinition.cpp \
  Class.cpp \
  ClassDefinition.cpp \
  ClassTypeMapper.cpp \
  DataPropertyDefinition.cpp \
  DataPropertyDefinitionCollection.cpp \
  DataTypeMapper.cpp \
  DbObject.cpp \
  FeatureClass.cpp \
  GeometricPropertyDefinition.cpp \
  ObjectPropertyClass.cpp \
  ObjectPropertyDefinition.cpp \
  PropertyDefinition.cpp \
  PropertyMappingClass.cpp \
  PropertyMappingConcrete.cpp \
  PropertyMappingDefinition.cpp \
  PropertyMappingRelation.cpp \
  PropertyMappingSingle.cpp \
  PropertyTypeMapper.cpp \
  QClassCollection.cpp \
  Schema.cpp \
  SchemaCollection.cpp \
  SchemaElement.cpp \
  SimplePropertyDefinition.cpp \
  SpatialContext.cpp \
  SpatialContextCollection.cpp \
  UniqueConstraint.cpp \
  stdafx.cpp

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../../Inc

libSmLogicalPhysical_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

