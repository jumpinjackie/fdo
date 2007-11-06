##
## Copyright (C) 2004-2007  Autodesk, Inc.
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

SUBDIRS = Message Overrides Provider

DIST_SUBDIRS = Message Overrides Provider

EXTRA_DIST = PostGis.sln PostGis.vsprops providers.xml README.txt TODO.txt

lib_LTLIBRARIES = libPostGISOverrides.la libPostGISProvider.la

##building libPostGISOverrides.so
libPostGISOverrides_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libPostGISOverrides_la_SOURCES = dummy.cpp

libPostGISOverrides_la_LIBADD =  \
  Overrides/libPostGISOverridesSrc.la \
  ../../../Utilities/Common/libProvidersCommon.la \
  ../../../Fdo/Unmanaged/Src/libFDO.la

libPostGISOverrides_la_LDFLAGS = \
  -shared \
  -release $(VERSION)

##building libPostGISProvider.so
libPostGISProvider_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libPostGISProvider_la_SOURCES = dummy.cpp

libPostGISProvider_la_LIBADD =  \
  Provider/libPostGISProviderSrc.la \
  libPostGISOverrides.la \
  ../../../Utilities/Common/libProvidersCommon.la \
  ../../../Fdo/Unmanaged/Src/libFDO.la \
  ../../../Thirdparty/boost/stage/lib/libboost_thread-mt.a \
  ../../../Thirdparty/boost/stage/lib/libboost_date_time-mt.a \
  -lpq

libPostGISProvider_la_LDFLAGS = \
  -shared \
  -release $(VERSION) \
  -L../../../Thirdparty/pgsql/lib
