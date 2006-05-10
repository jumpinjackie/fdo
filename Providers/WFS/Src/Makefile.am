AUTOMAKE_OPTIONS = foreign

SUBDIRS = Message Provider

DIST_SUBDIRS = Message Provider

##building libWFSProvider.so
lib_LTLIBRARIES = libWFSProvider.la

libWFSProvider_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libWFSProvider_la_SOURCES = dummy.cpp

libWFSProvider_la_LIBADD =  \
  Provider/libWFSProviderSrc.la \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(FDO)/Unmanaged/Src/Common/libFDOCommon.la  \
  $(FDO)/Unmanaged/Src/Geometry/libFDOGeometry.la  \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDOUTILITIES)/OWS/libFdoOws.la  \
  -lpthread

libWFSProvider_la_LDFLAGS = \
  -shared \
  -release $(VERSION)

