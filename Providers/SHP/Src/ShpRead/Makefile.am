AUTOMAKE_OPTIONS = foreign

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

libSHPRead_la_LDFLAGS = 

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

