AUTOMAKE_OPTIONS = foreign

SUBDIRS = Message ShpRead ShpSpatialIndex Overrides Provider UnitTest

DIST_SUBDIRS = Message ShpRead ShpSpatialIndex Overrides Provider

EXTRA_DIST = UnitTest

lib_LTLIBRARIES = libSHPOverrides.la libSHPProvider.la

##building libSHPOverrides.so
libSHPOverrides_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libSHPOverrides_la_SOURCES = dummy.cpp

libSHPOverrides_la_LIBADD =  \
  Overrides/libSHPOverridesSrc.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDO)/Unmanaged/Src/libFDO.la

libSHPOverrides_la_LDFLAGS = \
  -shared \
  -release $(VERSION)


##building libSHPProvider.so
libSHPProvider_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libSHPProvider_la_SOURCES = dummy.cpp

libSHPProvider_la_LIBADD =  \
  Provider/libSHPProviderSrc.la \
  ShpRead/libSHPRead.la \
  ShpSpatialIndex/libSHPSpatialIndex.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  Overrides/libSHPOverridesSrc.la \
  $(FDO)/Unmanaged/Src/libFDO.la

libSHPProvider_la_LDFLAGS = \
  -shared \
  -release $(VERSION)

