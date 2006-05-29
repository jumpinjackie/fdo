AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = ShpSpatialIndex.vcproj

noinst_LTLIBRARIES = libSHPSpatialIndex.la

libSHPSpatialIndex_la_SOURCES = \
  ShpSpatialIndexFile.cpp \
  ShpSpatialIndexCallBack.cpp \
  ShpSpatialIndexHeader.cpp \
  ShpSpatialIndexNode.cpp \
  ShpSpatialIndexHandle.cpp \
  ShpSpatialIndex.cpp \
  stdafx.cpp

noinst_HEADERS = $(libSHPSpatialIndex_la_SOURCES) \
  ShpSpatialIndexCallBack.h \
  ShpSpatialIndex.h \
  ShpSpatialIndexHeader.h \
  stdafx.h \
  ShpSpatialIndexFile.h \
  ShpSpatialIndexHandle.h \
  ShpSpatialIndexNode.h

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../ShpRead

libSHPSpatialIndex_la_LDFLAGS = 

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

