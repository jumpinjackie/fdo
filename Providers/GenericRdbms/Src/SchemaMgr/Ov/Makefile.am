AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libSchemaMgr_OV.la

libSchemaMgr_OV_la_SOURCES = \
  RdbmsOvAssociationPropertyDefinition.cpp \
  RdbmsOvClassCollection.cpp \
  RdbmsOvClassDefinition.cpp \
  RdbmsOvColumn.cpp \
  RdbmsOvDataPropertyDefinition.cpp \
  RdbmsOvGeometricColumn.cpp \
  RdbmsOvGeometricPropertyDefinition.cpp \
  RdbmsOvObjectPropertyDefinition.cpp \
  RdbmsOvPhysicalSchemaMapping.cpp \
  RdbmsOvPropertyDefinition.cpp \
  RdbmsOvPropertyDefinitionCollection.cpp \
  RdbmsOvPropertyMappingClass.cpp \
  RdbmsOvPropertyMappingConcrete.cpp \
  RdbmsOvPropertyMappingDefinition.cpp \
  RdbmsOvPropertyMappingRelation.cpp \
  RdbmsOvPropertyMappingSingle.cpp \
  RdbmsOvSchemaAutoGeneration.cpp \
  RdbmsOvTable.cpp \
  stdafx.cpp

noinst_HEADERS = $(libSchemaMgr_OV_la_SOURCES) \
  stdafx.h

INCLUDES = \
  -I../../../Inc \
  -I$(FDO)/Unmanaged/Inc  \
  -I$(FDOUTILITIES)/SchemaMgr/Inc

libSchemaMgr_OV_la_LIBADD =  \
  $(FDO)/Unmanaged/Src/libFDO.la

libSchemaMgr_OV_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

