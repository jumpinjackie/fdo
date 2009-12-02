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

EXTRA_DIST = KingOracle.sln

lib_LTLIBRARIES = libKingOracleOverrides.la libKingOracleProvider.la

##building libKingOracleOverrides.so
libKingOracleOverrides_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libKingOracleOverrides_la_SOURCES = dummy.cpp

libKingOracleOverrides_la_LIBADD =  \
  Overrides/libKingOracleOverridesSrc.la \
  ../../../Utilities/Common/libProvidersCommon.la \
  ../../../Fdo/Unmanaged/Src/libFDO.la

libKingOracleOverrides_la_LDFLAGS = \
  -shared \
  -release $(VERSION)

##building libKingOracleProvider.so
libKingOracleProvider_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libKingOracleProvider_la_SOURCES = dummy.cpp

libKingOracleProvider_la_LIBADD =  \
  Provider/libKingOracleProviderSrc.la \
  libKingOracleOverrides.la \
  ../../../Utilities/Common/libProvidersCommon.la \
  ../../../Fdo/Unmanaged/Src/libFDO.la \
  -locci \
  -lclntsh \
  -lociei \
  -lnnz10

libKingOracleProvider_la_LDFLAGS = \
  -shared \
  -release $(VERSION) \
  -L/usr/lib/oracle/10.2.0.3/client/lib \
  -L$(FDOORACLE)/lib
