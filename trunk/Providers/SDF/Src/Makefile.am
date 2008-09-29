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

SUBDIRS = Message Utils Provider UnitTest

DIST_SUBDIRS = Message Utils Provider UnitTest

EXTRA_DIST = SDFOS.sln

lib_LTLIBRARIES = libSDFProvider.la

##building libSDFProvider.so
libSDFProvider_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libSDFProvider_la_SOURCES = dummy.cpp

libSDFProvider_la_LIBADD =  \
  ./Utils/libSDFUtils.la \
  ./Provider/libSDFProviderSrc.la  \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(FDOUTILITIES)/SQLiteInterface/libSQLiteInterface.la \
  $(FDOTHIRDPARTY)/Sqlite3.3.13/linux/lib/sqlite3.a \
  $(FDOUTILITIES)/ExpressionEngine/Src/libExpressionEngine.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  -lxalan-c \
  -lxalanMsg \
  -lxerces-c


libSDFProvider_la_LDFLAGS = -shared \
  -release $(VERSION) \
  -L$(FDOTHIRDPARTY)/apache/xml-xalan/c/lib \
  -L$(FDOTHIRDPARTY)/apache/xml-xerces/c/lib \
  -Wl,-Bsymbolic
