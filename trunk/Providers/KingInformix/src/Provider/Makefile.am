AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libShpProviderSrc.la

libShpProviderSrc_la_SOURCES = \
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

libShpProviderSrc_la_LDFLAGS = 

INCLUDES = \
  -I../Message/inc/  \
  -I../ShpRead   \
  -I../ShpSpatialIndex  \
  -I../../../../fdo/Unmanaged/inc \
  -I../../../FdoCommon/inc  \
  -I../../inc  \
  -I../../../../fdo/Unmanaged/inc/fdo/Expression

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

