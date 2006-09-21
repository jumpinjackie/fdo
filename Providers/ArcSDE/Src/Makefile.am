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

SUBDIRS = Message Provider UnitTest

DIST_SUBDIRS = Message Provider UnitTest

EXTRA_DIST = ArcSDE.sln

lib_LTLIBRARIES = libArcSDEProvider.la

##building libArcSDEProvider.so
libArcSDEProvider_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libArcSDEProvider_la_SOURCES = dummy.cpp

libArcSDEProvider_la_LIBADD =  \
  Provider/libArcSDEProviderSrc.la \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  -lpe91 \
  -lsde91 \
  -lsg91

libArcSDEProvider_la_LDFLAGS = \
  -shared \
  -release $(VERSION) \
  -L$(SDEHOME)/lib

