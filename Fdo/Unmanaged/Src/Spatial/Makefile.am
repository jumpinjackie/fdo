AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libFDOSpatial.la

libFDOSpatial_la_SOURCES = \
  MathUtility.cpp \
  SpatialEnvelope.cpp \
  SpatialUtility.cpp \
  SpatialUtilityGeometryExtents.cpp \
  SpatialUtilityCircularArc.cpp

INCLUDES = -I../../Inc

libFDOSpatial_la_LDFLAGS = 

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE

