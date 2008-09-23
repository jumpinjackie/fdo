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

EXTRA_DIST = \
  build.bat \
  build_linux.sh

lib_LTLIBRARIES = libOGRProvider.la

##building libOGRProvider.so
libOGRProvider_la_SOURCES = \
  OgrFdoUtil.cpp \
  OgrProvider.cpp \
  ProjConverter.cpp

noinst_HEADERS = $(libOGRProvider_la_SOURCES) \
  resource.h \
  stdafx.h \
  ProjConverter.h \
  OgrProvider.h \
  OgrFdoUtil.h

libOGRProvider_la_LIBADD =  \
  ../../Fdo/Unmanaged/Src/libFDO.la \
 -lgdal  

libOGRProvider_la_LDFLAGS = \
  -shared \
  -release $(VERSION) \
  -L$(FDOGDAL)/lib \
  -L../../Thirdparty/gdal/lib

INCLUDES = \
  -I../../Fdo/Unmanaged/Inc \
  -I$(FDOGDAL)/ogr \
  -I$(FDOGDAL)/ogr/ogrsf_frmts \
  -I$(FDOGDAL)/port \
  -I../../Thirdparty/gdal/ogr \
  -I../../Thirdparty/gdal/ogr/ogrsf_frmts \
  -I../../Thirdparty/gdal/port

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

