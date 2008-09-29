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

noinst_LTLIBRARIES = libFdoRdbmsODBC_SchemaMgr_Lp.la

libFdoRdbmsODBC_SchemaMgr_Lp_la_SOURCES = \
Class.cpp \
ClassDefinition.cpp \
DataPropertyDefinition.cpp \
FeatureClass.cpp \
GeometricPropertyDefinition.cpp \
SchemaCollection.cpp \
Schema.cpp

noinst_HEADERS = $(libFdoRdbmsODBC_SchemaMgr_Lp_la_SOURCES) \
  Class.h \
  ClassDefinition.h \
  DataPropertyDefinition.h \
  FeatureClass.h \
  GeometricPropertyDefinition.h \
  Schema.h \
  SchemaCollection.h

INCLUDES = \
  -I../../ \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I../../../../../GenericRdbms/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/SchemaMgr/Inc/Sm/Lp \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I../../../../Inc \
  -I../../../../Inc/Rdbi \
  -I../../../ \
  -I../../../../ \
  -I../../../../Inc \
  -I../../../../Src/Fdo/Other \
  -I../../../../Src/Fdo/Capability \
  -I../../../../Src/Fdo/Connection \
  -I../../../../Src/Fdo/Custom \
  -I../../../../Src/Fdo/FeatureCommands \
  -I../../../../Src/Fdo/Filter \
  -I../../../../Src/Fdo/Lock \
  -I../../../../Src/Fdo/Lt \
  -I../../../../Src/Fdo/Schema \
  -I../../../../Src/Fdo/SpatialContext \
  -I$(FDO)/Unmanaged \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/Src/Geometry \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../../Gdbi \
  -I../../../LongTransactionManager  \
  -I../../../LockManager 

libFdoRdbmsODBC_SchemaMgr_Lp_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX



