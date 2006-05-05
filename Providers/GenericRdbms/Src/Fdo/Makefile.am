AUTOMAKE_OPTIONS = foreign

SUBDIRS = Capability Connection Custom DataStore FeatureCommands Filter Lock Lt Other Pvc Schema SpatialContext

noinst_LTLIBRARIES = libFdoRdbms.la

libFdoRdbms_la_SOURCES =


# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libFdoRdbms_la_SOURCES = dummy.cpp

libFdoRdbms_la_LIBADD = \
  Capability/libFdoRdbms_Capability.la \
  Connection/libFdoRdbms_Connection.la \
  Custom/libFdoRdbms_Custom.la \
  DataStore/libFdoRdbms_DataStore.la \
  FeatureCommands/libFdoRdbms_FeatureCommands.la  \
  Filter/libFdoRdbms_Filter.la  \
  Lock/libFdoRdbms_Lock.la \
  Lt/libFdoRdbms_Lt.la \
  Other/libFdoRdbms_Other.la \
  Pvc/libFdoRdbms_Pvc.la \
  Schema/libFdoRdbms_Schema.la   \
  SpatialContext/libFdoRdbms_SpatialContext.la

libFdoRdbms_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

