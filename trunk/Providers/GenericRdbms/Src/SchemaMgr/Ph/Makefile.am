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

SUBDIRS = Cfg Rd

DIST_SUBDIRS = Cfg Rd

noinst_LTLIBRARIES = libSmPhGrd.la

libSmPhGrd_la_SOURCES = \
  ClassWriter.cpp \
  CommandWriter.cpp \
  Fkey.cpp \
  Index.cpp \
  Mgr.cpp \
  NullIndicator.cpp \
  Owner.cpp \
  SpatialContextGroupWriter.cpp \
  SpatialContextWriter.cpp \
  Table.cpp \
  View.cpp

noinst_HEADERS = $(libSmPhGrd_la_SOURCES) \
  ClassWriter.h \
  CommandWriter.h \
  Fkey.h \
  Index.h \
  Mgr.h \
  NullIndicator.h \
  Owner.h \
  SpatialContextGroupWriter.h \
  SpatialContextWriter.h \
  Table.h \
  View.h

INCLUDES = \
  -I.. \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I../../.. \
  -I../../../Inc \
  -I$(FDOUTILITIES)/Common/Inc

libSmPhGrd_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

