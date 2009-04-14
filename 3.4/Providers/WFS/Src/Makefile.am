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

SUBDIRS = Message Provider

DIST_SUBDIRS = Message Provider

EXTRA_DIST = WFSOS.sln

##building libWFSProvider.so
lib_LTLIBRARIES = libWFSProvider.la

libWFSProvider_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libWFSProvider_la_SOURCES = dummy.cpp

libWFSProvider_la_LIBADD =  \
  Provider/libWFSProviderSrc.la \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(FDO)/Unmanaged/Src/Common/libFDOCommon.la  \
  $(FDO)/Unmanaged/Src/Geometry/libFDOGeometry.la  \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDOUTILITIES)/OWS/libFdoOws.la  \
  -lpthread

libWFSProvider_la_LDFLAGS = \
  -shared \
  -release $(VERSION)

