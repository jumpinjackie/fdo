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

noinst_LTLIBRARIES = libFdoRdbmsPostGis_SchemaMgr_Lp.la

libFdoRdbmsPostGis_SchemaMgr_Lp_la_SOURCES = \
  AssociationPropertyDefinition.cpp \
  Class.cpp \
  ClassDefinition.cpp \
  DataPropertyDefinition.cpp \
  FeatureClass.cpp \
  GeometricPropertyDefinition.cpp \
  ObjectPropertyClass.cpp \
  ObjectPropertyDefinition.cpp \
  SchemaCollection.cpp \
  Schema.cpp

noinst_HEADERS = $(libFdoRdbmsPostGis_SchemaMgr_Lp_la_SOURCES) \
  AssociationPropertyDefinition.h \
  Class.h \
  ClassDefinition.h \
  DataPropertyDefinition.h \
  FeatureClass.h \
  GeometricPropertyDefinition.h \
  ObjectPropertyClass.h \
  ObjectPropertyDefinition.h \
  Schema.h \
  SchemaCollection.h

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I../../../../Inc \
  -I../../../../ 

libFdoRdbmsPostGis_SchemaMgr_Lp_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

