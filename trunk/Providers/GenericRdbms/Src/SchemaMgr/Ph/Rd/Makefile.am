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

noinst_LTLIBRARIES = libSmPhRdGrd.la

libSmPhRdGrd_la_SOURCES = \
  FieldArray.cpp \
  QueryReader.cpp \
  RowArray.cpp

noinst_HEADERS = $(libSmPhRdGrd_la_SOURCES) \
  FieldArray.h \
  FieldArrayCollection.h \
  QueryReader.h \
  RowArray.h \
  RowArrayCollection.h

INCLUDES = \
  -I../.. \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I../../../.. \
  -I../../../../Inc \
  -I$(FDOUTILITIES)/Common/Inc

libSmPhRdGrd_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

