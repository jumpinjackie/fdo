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

EXTRA_DIST = ShpRead.vcproj

noinst_LTLIBRARIES = libSHPRead.la

libSHPRead_la_SOURCES = \
  DoublePoint.cpp \
  BoundingBox.cpp \
  BoundingBoxEx.cpp \
  ColumnInfo.cpp \
  MultiPatchShape.cpp \
  MultiPointMShape.cpp \
  MultiPointShape.cpp \
  MultiPointZShape.cpp \
  MZDataBase.cpp \
  MZData.cpp \
  NullShape.cpp \
  PointMShape.cpp \
  PointShape.cpp \
  PointZShape.cpp \
  PolygonMShape.cpp \
  PolygonShape.cpp \
  PolygonZShape.cpp \
  PolylineMShape.cpp \
  PolylineShape.cpp \
  PolylineZShape.cpp \
  PolyShape.cpp \
  RowData.cpp \
  Shape.cpp \
  ShapeCPG.cpp \
  ShapeDBF.cpp \
  ShapePRJ.cpp \
  ShapeFileBase.cpp \
  ShapeFile.cpp \
  ShapeIndex.cpp \
  stdafx.cpp

noinst_HEADERS = $(libSHPRead_la_SOURCES) \
   BoundingBoxEx.h \
   MultiPatchShape.h \
   MZData.h \
   PolygonMShape.h \
   PolylineZShape.h \
   ShapeFile.h \
   BoundingBox.h \
   MultiPointMShape.h \
   NullShape.h \
   PolygonShape.h \
   PolyShape.h \
   Shape.h \
   ColumnInfo.h \
   MultiPointShape.h \
   PointMShape.h \
   PolygonZShape.h \
   RowData.h \
   ShapeIndex.h \
   Constants.h \
   MultiPointZShape.h \
   PointShape.h \
   PolylineMShape.h \
   ShapeDBF.h \
   ShapePRJ.h \
   DoublePoint.h \
   MZDataBase.h \
   PointZShape.h \
   PolylineShape.h \
   ShapeFileBase.h \
   stdafx.h

libSHPRead_la_LDFLAGS = 

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

