AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libSDFProviderSrc.la

libSDFProviderSrc_la_SOURCES = \
  RTree.cpp \
  BinaryReader.cpp \
  BinaryWriter.cpp \
  DataDb.cpp \
  DataIO.cpp \
  KeyDb.cpp \
  SchemaDb.cpp \
  utf8_.cpp \
  SdfApplySchema.cpp \
  SdfCommandCapabilities.cpp \
  SdfConnection.cpp \
  SdfConnectionCapabilities.cpp \
  SdfConnectionInfo.cpp \
  SdfConnectionPropertyDictionary.cpp \
  SdfCreateSDFFile.cpp \
  SdfCreateSpatialContext.cpp \
  SdfDataValidator.cpp \
  SdfDelete.cpp \
  SdfDeletingFeatureReader.cpp \
  SdfDescribeSchema.cpp \
  SdfExpressionCapabilities.cpp \
  SdfFilterCapabilities.cpp \
  SdfGeometryCapabilities.cpp \
  SdfGetSpatialContexts.cpp \
  SdfInsert.cpp \
  SdfRasterCapabilities.cpp \
  SdfSchemaCapabilities.cpp \
  SdfSelect.cpp \
  SdfSelectAggregates.cpp \
  SdfSimpleFeatureReader.cpp \
  SdfSpatialContextReader.cpp \
  SdfTopologyCapabilities.cpp \
  SdfUpdate.cpp \
  SdfUpdatingFeatureReader.cpp \
  SdfDataReader.cpp \
  SdfDistinctDataReader.cpp \
  SdfQueryOptimizer.cpp \
  SdfCreateDataStore.cpp \
  SdfDataStorePropertyDictionary.cpp \
  SdfDeleteDataStore.cpp \
  SdfPropertyDictionary.cpp \
  SdfSpatialExtentsAggregateReader.cpp \
  SDF.cpp

libSDFProviderSrc_la_LDFLAGS = 

INCLUDES = \
  -I../../Inc \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/Src/Nls \
  -I/usr/include \
  -I../Message/Inc \
  -I../Utils \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/SQLiteInterface

CXXFLAGS = @CXXFLAGS_SDF@ -DSQLITE -O

