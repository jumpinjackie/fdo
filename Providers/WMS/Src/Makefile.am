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

EXTRA_DIST = WMSOS.sln

lib_LTLIBRARIES = libWMSOverrides.la libWMSProvider.la

##building libWMSOverrides.so
libWMSOverrides_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libWMSOverrides_la_SOURCES = dummy.cpp

libWMSOverrides_la_LIBADD =  \
  Overrides/libWMSOverridesSrc.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDO)/Unmanaged/Src/libFDO.la

libWMSOverrides_la_LDFLAGS = \
  -shared \
  -release $(VERSION)


##building libWMSProvider.so
libWMSProvider_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libWMSProvider_la_SOURCES = dummy.cpp

libWMSProvider_la_LIBADD =  \
  Provider/libWMSProviderSrc.la \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(FDO)/Unmanaged/Src/Geometry/libFDOGeometry.la  \
  $(FDO)/Unmanaged/Src/Common/libFDOCommon.la  \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDOUTILITIES)/OWS/libFdoOws.la  \
  libWMSOverrides.la \
  -lgdal    \
  -lpthread

libWMSProvider_la_LDFLAGS = \
  -shared \
  -release $(VERSION) \
  -L$(FDOGDAL)/.libs


