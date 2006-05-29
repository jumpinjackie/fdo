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

