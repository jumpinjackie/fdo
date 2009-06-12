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

EXTRA_DIST = SpatialIndex_standalone.sln  \
  SpatialIndex.vcproj \
  SpatialIndex_standalone.vcproj


noinst_LTLIBRARIES = libSpatialIndex.la

libSpatialIndex_la_SOURCES = \
  SltGeomUtils.cpp \
  SpatialIndex.cpp

noinst_HEADERS = $(libSpatialIndex_la_SOURCES) \
   si_api.h \
   SltGeomUtils.h \
   SpatialIndex.h \
   stdafx.h

libSpatialIndex_la_LDFLAGS = 

INCLUDES = \
  -I./   \
  -I/usr/include 

CXXFLAGS = @CXXFLAGS_SQLITE@

