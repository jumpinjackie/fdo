AUTOMAKE_OPTIONS = foreign

SUBDIRS = Message Provider 

DIST_SUBDIRS = Message Provider

lib_LTLIBRARIES = libArcSDEProvider.la

##building libArcSDEProvider.so
libArcSDEProvider_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libArcSDEProvider_la_SOURCES = dummy.cpp

libArcSDEProvider_la_LIBADD =  \
  Provider/libArcSDEProviderSrc.la \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  -lpe91 \
  -lsde91 \
  -lsg91

libArcSDEProvider_la_LDFLAGS = \
  -shared -version-info 3:2:0 \
  -L$(SDEHOME)/lib 

