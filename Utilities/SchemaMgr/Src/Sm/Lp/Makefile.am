AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libSmLogicalPhysical.la

libSmLogicalPhysical_la_SOURCES = \
  AssociationPropertyDefinition.cpp \
  Class.cpp \
  ClassDefinition.cpp \
  ClassTypeMapper.cpp \
  DataPropertyDefinition.cpp \
  DataPropertyDefinitionCollection.cpp \
  DataTypeMapper.cpp \
  DbObject.cpp \
  FeatureClass.cpp \
  GeometricPropertyDefinition.cpp \
  ObjectPropertyClass.cpp \
  ObjectPropertyDefinition.cpp \
  PropertyDefinition.cpp \
  PropertyMappingClass.cpp \
  PropertyMappingConcrete.cpp \
  PropertyMappingDefinition.cpp \
  PropertyMappingRelation.cpp \
  PropertyMappingSingle.cpp \
  PropertyTypeMapper.cpp \
  QClassCollection.cpp \
  Schema.cpp \
  SchemaCollection.cpp \
  SchemaElement.cpp \
  SimplePropertyDefinition.cpp \
  SpatialContext.cpp \
  SpatialContextCollection.cpp \
  UniqueConstraint.cpp \
  stdafx.cpp

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../../Inc

libSmLogicalPhysical_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

