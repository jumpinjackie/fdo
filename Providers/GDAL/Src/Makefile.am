AUTOMAKE_OPTIONS = foreign

SUBDIRS = Message Overrides Provider UnitTest

DIST_SUBDIRS = Message Overrides Provider UnitTest

EXTRA_DIST = RFP.sln

lib_LTLIBRARIES = libGRFPOverrides.la libGRFPProvider.la

##building libGRFPOverrides.so
libGRFPOverrides_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libGRFPOverrides_la_SOURCES = dummy.cpp

libGRFPOverrides_la_LIBADD =  \
  Overrides/libGRFPOverridesSrc.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDO)/Unmanaged/Src/libFDO.la

libGRFPOverrides_la_LDFLAGS = \
  -shared \
  -release $(VERSION)

##building libGRFPProvider.so
libGRFPProvider_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libGRFPProvider_la_SOURCES = dummy.cpp

libGRFPProvider_la_LIBADD =  \
  Provider/libGRFPProviderSrc.la \
  libGRFPOverrides.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDO)/Unmanaged/Src/libFDO.la

libGRFPProvider_la_LDFLAGS = \
  -shared \
  -release $(VERSION)
