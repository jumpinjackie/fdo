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

noinst_LTLIBRARIES = libFDOGeometry.la

EXTRA_DIST = \
  Geometry.rc \
  resource.h \
  Parse/script \
  Parse/script_linux \
  Parse/yyFgftWin.cpp \
  Parse/yyFgftWin.h \
  Parse/yyFgft.y \
  Parse/script_h

libFDOGeometry_la_SOURCES = \
  Parse/yyFgft.cpp \
  Fgf/CircularArcSegment.cpp \
  Fgf/CurvePolygon.cpp \
  Fgf/CurveString.cpp \
  Fgf/GeometryFactory.cpp \
  Fgf/LineString.cpp \
  Fgf/LineStringSegment.cpp \
  Fgf/LinearRing.cpp \
  Fgf/MultiCurvePolygon.cpp \
  Fgf/MultiCurveString.cpp \
  Fgf/MultiGeometry.cpp \
  Fgf/MultiLineString.cpp \
  Fgf/MultiPoint.cpp \
  Fgf/MultiPolygon.cpp \
  Fgf/Point.cpp \
  Fgf/Polygon.cpp \
  Fgf/Ring.cpp \
  Fgf/Util.cpp \
  Fgf/GeometryDll.cpp \
  Fgf/GeometryThreadData.cpp \
  DirectPositionImpl.cpp \
  EnvelopeImpl.cpp \
  GeometryFactoryAbstract.cpp \
  GeometryUtility.cpp \
  Parse/LexFgft.cpp \
  Parse/ParseFgft.cpp

noinst_HEADERS = $(libFDOGeometry_la_SOURCES) \
  GeometryImpl.h \
  GeometryUtility.h \
  Fgf/GeometryFactory2.h \
  Fgf/GeometryImpl.h \
  Fgf/CircularArcSegment.h \
  Fgf/CurvePolygon.h \
  Fgf/CurveString.h \
  Fgf/LinearRing.h \
  Fgf/LineString.h \
  Fgf/LineStringSegment.h \
  Fgf/MultiCurvePolygon.h \
  Fgf/MultiCurveString.h \
  Fgf/MultiGeometry.h \
  Fgf/MultiLineString.h \
  Fgf/MultiPoint.h \
  Fgf/MultiPolygon.h \
  Fgf/Point.h \
  Fgf/Polygon.h \
  Fgf/Ring.h \
  Fgf/Util.h \
  Fgf/GeometryThreadData.h \
  Fgf/Pool.h \
  Parse/LexFgft.h \
  Parse/ParseFgft.h \
  Parse/yyFgft.h

INCLUDES = \
  -I../../Inc \
  -I../Common

libFDOGeometry_la_LDFLAGS = 

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU

