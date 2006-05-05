AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libFdoOwsSrc.la

libFdoOwsSrc_la_SOURCES = \
  FdoOwsCapabilities.cpp \
  FdoOwsRequest.cpp \
  FdoOwsDelegate.cpp \
  FdoOwsRequestMetadata.cpp \
  FdoOwsExceptionReport.cpp \
  FdoOwsResponse.cpp \
  FdoOwsServiceIdentification.cpp \
  FdoOwsGetCapabilities.cpp \
  FdoOwsServiceMetadata.cpp \
  FdoOwsHttpHandler.cpp \
  FdoOwsUrlResolver.cpp \
  FdoOwsIHttpHandler.cpp \
  FdoOwsOgcFilterSerializer.cpp \
  FdoOwsGeographicBoundingBox.cpp \
  FdoOwsGeographicBoundingBoxCollection.cpp \
  FdoOwsOgcGeometrySerializer.cpp 

INCLUDES = \
  -I../ \
  -I../Inc \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOTHIRDPARTY)/boost_1_32_0

libFdoOwsSrc_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -pthread

