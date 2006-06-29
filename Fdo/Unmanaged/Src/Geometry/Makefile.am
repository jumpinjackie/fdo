AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libFDOGeometry.la

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

INCLUDES = \
  -I../../Inc \
  -I../Common

libFDOGeometry_la_LDFLAGS = 

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU

