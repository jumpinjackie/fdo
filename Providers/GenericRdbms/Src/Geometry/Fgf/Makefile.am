AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libgeometry_fgf.la

libgeometry_fgf_la_SOURCES = \
  AllGeometry_c.cpp

INCLUDES = \
  -I../../.. \
  -I$(FDO)/Unmanaged/Inc

libgeometry_fgf_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

