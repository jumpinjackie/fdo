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

SUBDIRS = SpatialIndex sqlite Provider UnitTest

DIST_SUBDIRS = SpatialIndex sqlite Provider UnitTest

EXTRA_DIST = SQLiteProvider.sln

lib_LTLIBRARIES = libSQLiteProvider.la

##building libSQLiteProvider.so
libSQLiteProvider_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libSQLiteProvider_la_SOURCES = dummy.cpp

libSQLiteProvider_la_LIBADD =  \
  ./sqlite/libSqlite3Engine.la \
  ./SpatialIndex/libSpatialIndex.la \
  ./Provider/libSQLiteProviderSrc.la  \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(FDOUTILITIES)/ExpressionEngine/Src/libExpressionEngine.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  -lxalan-c \
  -lxalanMsg \
  -lxerces-c


libSQLiteProvider_la_LDFLAGS = -shared \
  -release $(VERSION) \
  -L$(FDOTHIRDPARTY)/apache/xml-xalan/c/lib \
  -L$(FDOTHIRDPARTY)/apache/xml-xerces/c/lib \
  -Wl,-Bsymbolic
