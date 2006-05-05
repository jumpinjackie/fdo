AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libSHPProviderSrc.la

libSHPProviderSrc_la_SOURCES = \
  stdafx.cpp \
  ShpCommandCapabilities.cpp \
  ShpConnectionCapabilities.cpp \
  ShpConnection.cpp \
  ShpConnectionInfo.cpp \
  ShpSchemaUtilities.cpp \
  ShpDescribeSchemaCommand.cpp \
  ShpDescribeSchemaMappingCommand.cpp \
  ShpApplySchemaCommand.cpp \
  ShpDestroySchemaCommand.cpp \
  ShpExpressionCapabilities.cpp \
  ShpFeatureReader.cpp \
  ShpFilterExecutor.cpp \
  ShpFileSet.cpp \
  ShpFilterCapabilities.cpp \
  ShpGeometryCapabilities.cpp \
  ShpQueryOptimizer.cpp \
  ShpFeatIdQueryTester.cpp \
  ShpFeatIdQueryEvaluator.cpp \
  ShpRasterCapabilities.cpp \
  ShpSchemaCapabilities.cpp \
  ShpSelectCommand.cpp \
  ShpSelectAggregates.cpp \
  ShpSpatialExtentsAggregateReader.cpp \
  ShpInsertCommand.cpp \
  ShpUpdateCommand.cpp \
  ShpDeleteCommand.cpp \
  ShpTopologyCapabilities.cpp \
  ShpGetSpatialContextsCommand.cpp \
  ShpSpatialContext.cpp \
  ShpSpatialContextReader.cpp \
  ShpPhysicalSchema.cpp \
  ShpLpClassDefinition.cpp \
  ShpLpFeatureSchema.cpp \
  ShpLpPropertyDefinition.cpp

libSHPProviderSrc_la_LDFLAGS = 

INCLUDES = \
  -I../Message/Inc/  \
  -I../ShpRead   \
  -I../ShpSpatialIndex  \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc  \
  -I../../Inc  \
  -I$(FDO)/Unmanaged/Inc/Fdo/Expression

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

