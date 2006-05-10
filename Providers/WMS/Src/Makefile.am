AUTOMAKE_OPTIONS = foreign

SUBDIRS = Message Overrides Provider

DIST_SUBDIRS = Message Overrides Provider

lib_LTLIBRARIES = libWMSOverrides.la libWMSProvider.la

##building libWMSOverrides.so
libWMSOverrides_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libWMSOverrides_la_SOURCES = dummy.cpp

libWMSOverrides_la_LIBADD =  \
  Overrides/libWMSOverridesSrc.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDO)/Unmanaged/Src/libFDO.la

libWMSOverrides_la_LDFLAGS = \
  -shared \
  -release $(VERSION)


##building libWMSProvider.so
libWMSProvider_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libWMSProvider_la_SOURCES = dummy.cpp

libWMSProvider_la_LIBADD =  \
  Provider/libWMSProviderSrc.la \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(FDO)/Unmanaged/Src/Geometry/libFDOGeometry.la  \
  $(FDO)/Unmanaged/Src/Common/libFDOCommon.la  \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDOUTILITIES)/OWS/libFdoOws.la  \
  libWMSOverrides.la \
  -lgdal    \
  -lpthread

libWMSProvider_la_LDFLAGS = \
  -shared \
  -release $(VERSION) \
  -L$(FDOTHIRDPARTY)/GDAL1.3/linux/lib/optimized


