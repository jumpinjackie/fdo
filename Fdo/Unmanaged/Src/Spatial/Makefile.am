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

noinst_LTLIBRARIES = libFDOSpatial.la

EXTRA_DIST = resource.h Spatial.rc

libFDOSpatial_la_SOURCES = \
  MathUtility.cpp \
  SpatialEnvelope.cpp \
  SpatialUtility.cpp \
  SpatialUtilityGeometryExtents.cpp \
  SpatialUtilityCircularArc.cpp

noinst_HEADERS = $(libFDOSpatial_la_SOURCES) \
  MathUtility.h \
  SpatialEnvelope.h\
  SpatialUtilityCircularArc.h \
  SpatialUtilityGeometryExtents.h \
  SpatialUtilityVectorAngle.h

INCLUDES = -I../../Inc

libFDOSpatial_la_LDFLAGS = 

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE

