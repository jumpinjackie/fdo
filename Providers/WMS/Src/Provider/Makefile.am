AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libWMSProviderSrc.la

libWMSProviderSrc_la_SOURCES = \
  FdoWmsCommandCapabilities.cpp \
  FdoWmsConnectionCapabilities.cpp \
  FdoWmsConnection.cpp \
  FdoWmsConnectionInfo.cpp \
  FdoWmsDataReader.cpp \
  FdoWmsDescribeSchemaCommand.cpp \
  FdoWmsExpressionCapabilities.cpp \
  FdoWmsFeatureReader.cpp \
  FdoWmsFilterCapabilities.cpp \
  FdoWmsGeometryCapabilities.cpp \
  FdoWmsRasterCapabilities.cpp \
  FdoWmsSchemaCapabilities.cpp \
  FdoWmsSelectCommand.cpp \
  FdoWmsTopologyCapabilities.cpp \
  FdoWmsUtils.cpp \
  FdoWmsActivateSpatialContextCommand.cpp \
  FdoWmsGetSpatialContextsCommand.cpp \
  FdoWmsSpatialContextReader.cpp \
  FdoWmsBandRasterGdal.cpp \
  FdoWmsRasterGdal.cpp \
  FdoWmsImage.cpp \
  FdoWmsRasterPropertyDictionaryGdal.cpp \
  FdoWmsStreamReaderGdalByRow.cpp \
  FdoWmsDescribeSchemaMappingCommand.cpp \
  FdoWmsDimension.cpp \
  FdoWmsLayerCollection.cpp \
  FdoWmsBoundingBoxCollection.cpp \
  FdoWmsLayer.cpp \
  FdoWmsBoundingBox.cpp \
  FdoWmsServiceMetadata.cpp \
  FdoWmsCapabilities.cpp \
  FdoWmsStyleCollection.cpp \
  FdoWmsDelegate.cpp \
  FdoWmsStyle.cpp \
  FdoWmsDimensionCollection.cpp \
  FdoWmsGetMap.cpp 

libWMSProviderSrc_la_LDFLAGS =

INCLUDES = \
  -I./ \
  -I../ \
  -I../Message/Inc \
  -I../../Inc \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/OWS/Inc \
  -I$(FDOTHIRDPARTY)/GDAL1.3/linux/include

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -pthread

