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

SUBDIRS = Custom/Gdbi Driver Fdo SchemaMgr
DIST_SUBDIRS = Custom/Gdbi Driver Fdo SchemaMgr

EXTRA_DIST = \
   PostGis.sln \
   PostGis.vcproj \
   build.bat \
   resource.h \
   PostGis.rc \
   ReadMe.txt

noinst_LTLIBRARIES = libFdoRdbmsPostgreSQL.la

libFdoRdbmsPostgreSQL_la_SOURCES  = \
  PostGis.cpp \
  stdafx.cpp

noinst_HEADERS = $(libFdoRdbmsPostgreSQL_la_SOURCES) \
  stdafx.h

libFdoRdbmsPostgreSQL_la_LDFLAGS = \
  Fdo/libFdoRdbmsPostGis_Fdo.la \
  SchemaMgr/libFdoRdbmsPostGis_SchemaMgr.la \
  SchemaMgr/Lp/libFdoRdbmsPostGis_SchemaMgr_Lp.la \
  SchemaMgr/Ov/libFdoRdbmsPostGis_SchemaMgr_Ov.la \
  SchemaMgr/Ph/libFdoRdbmsPostGis_SchemaMgr_Ph.la

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I../../Inc/Rdbi \
  -I../ \
  -I../../ \
  -I../../Inc \
  -I../Fdo/Connection \
  -I../Fdo/Other \
  -I../Fdo/FeatureCommands \
  -I../Fdo/Custom \
  -I../Fdo/Schema \
  -I../Fdo/Lt \
  -I../Fdo/Lock  \
  -I../Fdo/SpatialContext  \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../Gdbi \
  -I../LongTransactionManager  \
  -I../LockManager

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

