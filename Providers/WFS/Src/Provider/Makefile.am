AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libWFSProviderSrc.la

libWFSProviderSrc_la_SOURCES = \
  ./FdoWfsCommandCapabilities.cpp \
  ./FdoWfsConnectionCapabilities.cpp \
  ./FdoWfsConnection.cpp \
  ./FdoWfsConnectionInfo.cpp \
  ./FdoWfsDataReader.cpp \
  ./FdoWfsDelegate.cpp \
  ./FdoWfsDescribeFeatureType.cpp \
  ./FdoWfsDescribeSchemaCommand.cpp \
  ./FdoWfsExpressionCapabilities.cpp \
  ./FdoWfsFeatureReader.cpp \
  ./FdoWfsFeatureType.cpp \
  ./FdoWfsFeatureTypeList.cpp \
  ./FdoWfsFilterCapabilities.cpp \
  ./FdoWfsGeometryCapabilities.cpp \
  ./FdoWfsGetFeature.cpp \
  ./FdoWfsGetSpatialContexts.cpp \
  ./FdoWfsOgcFilterCapabilities.cpp \
  ./FdoWfsOgcScalarCapabilities.cpp \
  ./FdoWfsOgcSpatialCapabilities.cpp \
  ./FdoWfsRasterCapabilities.cpp \
  ./FdoWfsSchemaCapabilities.cpp \
  ./FdoWfsSchemaCopyHandler.cpp \
  ./FdoWfsSchemaMerger.cpp \
  ./FdoWfsSchemaRefHandler.cpp \
  ./FdoWfsSelectAggregatesCommand.cpp \
  ./FdoWfsSelectCommand.cpp \
  ./FdoWfsServiceMetadata.cpp \
  ./FdoWfsSpatialContextReader.cpp \
  ./FdoWfsTopologyCapabilities.cpp \
  ./FdoWfsUtils.cpp \
  ./stdafx.cpp

libWFSProviderSrc_la_LDFLAGS =

INCLUDES = \
  -I./   \
  -I$(FDO)/Unmanaged/Inc  \
  -I./inc -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/OWS/Inc

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -pthread

