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

SUBDIRS = Provider UnitTest

DIST_SUBDIRS = Provider UnitTest

EXTRA_DIST = OGRPRovider.sln OGRProvider_vs12.sln

lib_LTLIBRARIES = libOGRProvider.la

##building libOGRPRovider.so
libOGRProvider_la_SOURCES =

# Dummy C++ source to cause C++ linking
nodist_EXTRA_libOGRProvider_la_SOURCES = dummy.cpp

libOGRProvider_la_LIBADD =  \
  Provider/libOGRProviderSrc.la \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDOUTILITIES)/ExpressionEngine/Src/libExpressionEngine.la \
  -lgdal  

libOGRProvider_la_LDFLAGS = \
  -shared \
  -release $(VERSION) \
  -L$(FDOGDAL)/lib
