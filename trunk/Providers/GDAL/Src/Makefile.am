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

SUBDIRS = Message Overrides Provider UnitTest

DIST_SUBDIRS = Message Overrides Provider UnitTest

EXTRA_DIST = RFP.sln

lib_LTLIBRARIES = libGRFPOverrides.la libGRFPProvider.la

##building libGRFPOverrides.so
libGRFPOverrides_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libGRFPOverrides_la_SOURCES = dummy.cpp

libGRFPOverrides_la_LIBADD =  \
  Overrides/libGRFPOverridesSrc.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDO)/Unmanaged/Src/libFDO.la

libGRFPOverrides_la_LDFLAGS = \
  -shared \
  -release $(VERSION)

##building libGRFPProvider.so
libGRFPProvider_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libGRFPProvider_la_SOURCES = dummy.cpp

libGRFPProvider_la_LIBADD =  \
  Provider/libGRFPProviderSrc.la \
  libGRFPOverrides.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDO)/Unmanaged/Src/libFDO.la \
  -lgdal

libGRFPProvider_la_LDFLAGS = \
  -shared \
  -release $(VERSION) \
  -L$(FDOGDAL)/lib
