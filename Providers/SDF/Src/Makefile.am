AUTOMAKE_OPTIONS = foreign

SUBDIRS = Message Utils Provider UnitTest

DIST_SUBDIRS = Message Utils Provider UnitTest

EXTRA_DIST = SDFOS.sln

lib_LTLIBRARIES = libSDFProvider.la

##building libSDFProvider.so
libSDFProvider_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libSDFProvider_la_SOURCES = dummy.cpp

libSDFProvider_la_LIBADD =  \
  ./Utils/libSDFUtils.la \
  ./Provider/libSDFProviderSrc.la  \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(FDO)/Unmanaged/Src/Spatial/libFDOSpatial.la \
  $(FDOUTILITIES)/SQLiteInterface/libSQLiteInterface.la \
  $(FDOTHIRDPARTY)/Sqlite3.1.5/linux/lib/sqlite3.a \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  -lxalan-c \
  -lxalanMsg \
  -lxerces-c


libSDFProvider_la_LDFLAGS = -shared \
  -release $(VERSION) \
  -L$(FDOTHIRDPARTY)/apache/xml-xalan/c/lib \
  -L$(FDOTHIRDPARTY)/apache/xml-xerces/c/lib

