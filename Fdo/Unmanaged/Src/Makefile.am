AUTOMAKE_OPTIONS = foreign

SUBDIRS = McToMsf Message FileToHdr Nls Common Geometry Spatial Fdo

DIST_SUBDIRS = McToMsf Message FileToHdr Nls Common Geometry Spatial Fdo

lib_LTLIBRARIES = libFDO.la

libFDO_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libFDO_la_SOURCES = dummy.cpp

libFDO_la_LIBADD =  \
  Fdo/libFDOSrc.la \
  Common/libFDOCommon.la \
  Spatial/libFDOSpatial.la \
  Geometry/libFDOGeometry.la \
  Nls/libFDONLS.la  \
  -lxalan-c -lxalanMsg \
  -lxerces-c \
  -lpthread \
  -ldl

libFDO_la_LDFLAGS = -shared \
  -release $(VERSION) \
  -L$(FDOTHIRDPARTY)/apache/xml-xalan/c/lib \
  -L$(FDOTHIRDPARTY)/apache/xml-xerces/c/lib

CXXFLAGS = @CXXFLAGS@ -shared

