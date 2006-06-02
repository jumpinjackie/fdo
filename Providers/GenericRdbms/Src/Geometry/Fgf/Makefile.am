AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = geometry_fgf.vcproj

noinst_LTLIBRARIES = libgeometry_fgf.la

libgeometry_fgf_la_SOURCES = \
  AllGeometry_c.cpp

noinst_HEADERS = $(libgeometry_fgf_la_SOURCES)

INCLUDES = \
  -I../../.. \
  -I$(FDO)/Unmanaged/Inc

libgeometry_fgf_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

