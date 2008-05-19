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

SUBDIRS = Message ShpRead ShpSpatialIndex Overrides Provider UnitTest

DIST_SUBDIRS = Message ShpRead ShpSpatialIndex Overrides Provider UnitTest

EXTRA_DIST = SHP.sln

lib_LTLIBRARIES = libSHPOverrides.la libSHPProvider.la

##building libSHPOverrides.so
libSHPOverrides_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libSHPOverrides_la_SOURCES = dummy.cpp

libSHPOverrides_la_LIBADD =  \
  Overrides/libSHPOverridesSrc.la \
  $(FDOUTILITIES)/ExpressionEngine/Src/libExpressionEngine.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDO)/Unmanaged/Src/libFDO.la

libSHPOverrides_la_LDFLAGS = \
  -shared \
  -release $(VERSION)


##building libSHPProvider.so
libSHPProvider_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libSHPProvider_la_SOURCES = dummy.cpp

libSHPProvider_la_LIBADD =  \
  Provider/libSHPProviderSrc.la \
  ShpRead/libSHPRead.la \
  ShpSpatialIndex/libSHPSpatialIndex.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDOUTILITIES)/ExpressionEngine/Src/libExpressionEngine.la \
  Overrides/libSHPOverridesSrc.la \
  $(FDO)/Unmanaged/Src/libFDO.la

libSHPProvider_la_LDFLAGS = \
  -shared \
  -release $(VERSION)

