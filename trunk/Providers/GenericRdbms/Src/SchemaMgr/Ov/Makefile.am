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

noinst_LTLIBRARIES = libSchemaMgr_OV.la

libSchemaMgr_OV_la_SOURCES = \
  RdbmsOvAssociationPropertyDefinition.cpp \
  RdbmsOvClassCollection.cpp \
  RdbmsOvClassDefinition.cpp \
  RdbmsOvColumn.cpp \
  RdbmsOvDataPropertyDefinition.cpp \
  RdbmsOvGeometricColumn.cpp \
  RdbmsOvGeometricPropertyDefinition.cpp \
  RdbmsOvObjectPropertyDefinition.cpp \
  RdbmsOvPhysicalSchemaMapping.cpp \
  RdbmsOvPropertyDefinition.cpp \
  RdbmsOvPropertyDefinitionCollection.cpp \
  RdbmsOvPropertyMappingClass.cpp \
  RdbmsOvPropertyMappingConcrete.cpp \
  RdbmsOvPropertyMappingDefinition.cpp \
  RdbmsOvPropertyMappingRelation.cpp \
  RdbmsOvPropertyMappingSingle.cpp \
  RdbmsOvSchemaAutoGeneration.cpp \
  RdbmsOvTable.cpp \
  stdafx.cpp

noinst_HEADERS = $(libSchemaMgr_OV_la_SOURCES) \
  stdafx.h

INCLUDES = \
  -I../../../Inc \
  -I$(FDO)/Unmanaged/Inc  \
  -I$(FDOUTILITIES)/SchemaMgr/Inc

libSchemaMgr_OV_la_LIBADD =  \
  $(FDO)/Unmanaged/Src/libFDO.la

libSchemaMgr_OV_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

