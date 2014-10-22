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

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
  OGRProvider.rc \
  OGRProvider.vcproj \
  OGRProvider.vcproj

noinst_LTLIBRARIES = libOGRProviderSrc.la

libOGRProviderSrc_la_SOURCES = \
  OgrFdoUtil.cpp \
  OgrFilterIdentifierExtractor.cpp \
  OgrProvider.cpp \
  OgrSpatialExtentsDataReader.cpp \
  ProjConverter.cpp

noinst_HEADERS = $(libOGRProviderSrc_la_SOURCES) \
  resource.h \
  stdafx.h \
  OgrFilterIdentifierExtractor.h \
  ProjConverter.h \
  OgrProvider.h \
  OgrFdoUtil.h \
  OgrSpatialExtentsDataReader.h

libOGRProviderSrc_la_LDFLAGS = 

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/ExpressionEngine/Inc \
  -I$(FDOGDAL)/include \
  -I$(FDOGDAL)/ogr \
  -I$(FDOGDAL)/ogr/ogrsf_frmts \
  -I$(FDOGDAL)/port \
  -I../../../Thirdparty/gdal/include \
  -I../../../Thirdparty/gdal/ogr \
  -I../../../Thirdparty/gdal/ogr/ogrsf_frmts \
  -I../../../Thirdparty/gdal/port

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

