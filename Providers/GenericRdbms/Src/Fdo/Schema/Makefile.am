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

noinst_LTLIBRARIES = libFdoRdbms_Schema.la

libFdoRdbms_Schema_la_SOURCES = \
  FdoRdbmsApplySchemaCommand.cpp \
  FdoRdbmsDestroySchemaCommand.cpp \
  FdoRdbmsDescribeSchemaCommand.cpp \
  FdoRdbmsSchemaUtil.cpp \
  FdoRdbmsDescribeSchemaMappingCommand.cpp

noinst_HEADERS = $(libFdoRdbms_Schema_la_SOURCES) \
  FdoRdbmsApplySchemaCommand.h \
  FdoRdbmsDescribeSchemaCommand.h \
  FdoRdbmsDescribeSchemaMappingCommand.h \
  FdoRdbmsDestroySchemaCommand.h \
  FdoRdbmsSchemaUtil.h

INCLUDES = \
  -I../ \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I../../../../GenericRdbms/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/SchemaMgr/Inc/Sm/Lp \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I../../../Inc \
  -I../../../Inc/Rdbi \
  -I../../ \
  -I../../../ \
  -I../../../Inc \
  -I.././Other \
  -I.././Capability \
  -I.././Connection \
  -I.././Custom \
  -I.././FeatureCommands \
  -I.././Filter \
  -I.././Lock \
  -I.././Lt \
  -I.././Schema \
  -I.././SpatialContext \
  -I$(FDO)/Unmanaged \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/src/geometry \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../Gdbi \
  -I../../LongTransactionManager \
  -I../../LockManager

libFdoRdbms_Schema_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

