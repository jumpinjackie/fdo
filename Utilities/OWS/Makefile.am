AUTOMAKE_OPTIONS = foreign

SUBDIRS = Src

##building libFdoOws.so
lib_LTLIBRARIES = libFdoOws.la

libFdoOws_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libFdoOws_la_SOURCES = dummy.cpp

libFdoOws_la_LIBADD =  \
  Src/libFdoOwsSrc.la \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(FDO)/Unmanaged/Src/Geometry/libFDOGeometry.la  \
  $(FDO)/Unmanaged/Src/Common/libFDOCommon.la  \
  -lcurl  \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  $(FDOTHIRDPARTY)/boost_1_32_0/bin/boost/libs/thread/build/libboost_thread.a/gcc/release/runtime-link-static/threading-multi/libboost_thread-gcc-mt-s-1_32.a \
  -lssl \
  -lpthread

libFdoOws_la_LDFLAGS = -shared \
  -release $(VERSION) \
  -L$(FDOTHIRDPARTY)/libcurl/lib/linux \
  -L$(FDOTHIRDPARTY)/openssl/lib/linux


