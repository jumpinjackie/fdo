##
## Copyright (C) 2004-2006  Autodesk, Inc.
## 
## This library is free software; you can redistribute it and/or
## modify it under the terms of version 2.1 of the GNU Lesser
## General Public License as published by the Free Software Foundation.
## 
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
##

AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libFDOSrc.la

EXTRA_DIST = \
  Fdo.rc \
  resource.h \
  Parse/Lex.h \
  Parse/Parse.h \
  Parse/script \
  Parse/script2 \
  Parse/script2_linux \
  Parse/script_linux \
  Parse/yyExpression.y \
  Parse/yyFilter.y \
  Parse/script_h \
  Parse/script2_h \
  Parse/yyExpression.cpp \
  Parse/yyExpression.h \
  Parse/yyExpressionWin.cpp \
  Parse/yyExpressionWin.h \
  Parse/yyFilter.cpp \
  Parse/yyFilter.h \
  Parse/yyFilterWin.h \
  Parse/yyFilterWin.cpp \
  Schema/SchemaFromExternal.xslt \
  Schema/SchemaFromInternal.xslt

libFDOSrc_la_SOURCES = \
  Fdo.cpp \
  IDisposableCollection.cpp \
  Expression/BLOBValue.cpp \
  Expression/BinaryExpression.cpp \
  Expression/BooleanValue.cpp \
  Expression/ByteValue.cpp \
  Expression/CLOBValue.cpp \
  Expression/DataValue.cpp \
  Expression/DateTimeValue.cpp \
  Expression/DecimalValue.cpp \
  Expression/DoubleValue.cpp \
  Expression/Expression.cpp \
  Expression/ExpressionCollection.cpp \
  Expression/ExpressionException.cpp \
  Expression/Function.cpp \
  Expression/GeometryValue.cpp \
  Expression/Identifier.cpp \
  Expression/ComputedIdentifier.cpp \
  Expression/Int16Value.cpp \
  Expression/Int32Value.cpp \
  Expression/Int64Value.cpp \
  Expression/IntBinValue.cpp \
  Expression/IntHexValue.cpp \
  Expression/ExpressionInternal.cpp \
  Expression/Parameter.cpp \
  Expression/SingleValue.cpp \
  Expression/StringValue.cpp \
  Expression/UnaryExpression.cpp \
  Expression/DataValueCollection.cpp \
  Expression/LiteralValueCollection.cpp \
  Expression/SubSelectExpression.cpp \
  Expression/JoinCriteria.cpp \
  Expression/JoinCriteriaCollection.cpp \
  Filter/BinaryLogicalOperator.cpp \
  Filter/ComparisonCondition.cpp \
  Filter/DistanceCondition.cpp \
  Filter/Filter.cpp \
  Filter/FilterException.cpp \
  Filter/GeometricCondition.cpp \
  Filter/InCondition.cpp \
  Filter/NullCondition.cpp \
  Filter/SpatialCondition.cpp \
  Filter/UnaryLogicalOperator.cpp \
  Filter/ValueExpressionCollection.cpp \
  Raster/RasterDataModel.cpp \
  Schema/AutogenerationException.cpp \
  Schema/Class.cpp \
  Schema/ClassCapabilities.cpp \
  Schema/ClassCollection.cpp \
  Schema/ClassDefinition.cpp \
  Schema/DataPropertyDefinition.cpp \
  Schema/DataPropertyDefinitionCollection.cpp \
  Schema/DataTypeMapper.cpp \
  Schema/FeatureClass.cpp \
  Schema/FeatureClassCollection.cpp \
  Schema/FeatureSchema.cpp \
  Schema/FeatureSchemaCollection.cpp \
  Schema/GeometricPropertyDefinition.cpp \
  Schema/ObjectPropertyDefinition.cpp \
  Schema/PropertyDefinition.cpp \
  Schema/PropertyDefinitionCollection.cpp \
  Schema/ReadOnlyDataPropertyDefinitionCollection.cpp \
  Schema/ReadOnlyPropertyDefinitionCollection.cpp \
  Schema/AssociationPropertyDefinition.cpp \
  Schema/SchemaAttributeDictionary.cpp \
  Schema/SchemaElement.cpp \
  Schema/SchemaException.cpp \
  Schema/RasterPropertyDefinition.cpp \
  Schema/MergeContext.cpp \
  Schema/XmlContext.cpp \
  Schema/XmlError.cpp \
  Schema/NetworkClass.cpp \
  Schema/NetworkFeatureClass.cpp \
  Schema/NetworkLayerClass.cpp \
  Schema/NetworkNodeFeatureClass.cpp \
  Schema/NetworkLinkFeatureClass.cpp \
  Schema/TopoFeaturePropertyDefinition.cpp \
  Schema/TopoGeometryPropertyDefinition.cpp \
  Schema/Topology.cpp \
  Schema/PropertyValueConstraintList.cpp \
  Schema/PropertyValueConstraintRange.cpp \
  Schema/UniqueConstraint.cpp \
  Schema/UniqueConstraintCollection.cpp \
  ClientServices/ClientServiceException.cpp \
  ClientServices/ConnectionManager.cpp \
  ClientServices/FeatureAccessManager.cpp \
  ClientServices/Provider.cpp \
  ClientServices/ProviderCollection.cpp \
  ClientServices/ProviderNameTokens.cpp \
  ClientServices/ProviderRegistry.cpp \
  ClientServices/RegistryUtility.cpp \
  ClientServices/prefix.cpp \
  Connections/ConnectionException.cpp \
  Connections/Capabilities/ArgumentDefinition.cpp \
  Connections/Capabilities/ArgumentDefinitionCollection.cpp \
  Connections/Capabilities/FunctionDefinition.cpp \
  Connections/Capabilities/FunctionDefinitionCollection.cpp \
  Connections/Capabilities/ReadOnlyArgumentDefinitionCollection.cpp \
  Connections/Capabilities/ISchemaCapabilities.cpp \
  Connections/Capabilities/IExpressionCapabilities.cpp \
  Connections/Capabilities/SignatureDefinition.cpp \
  Commands/BatchParameterValueCollection.cpp \
  Commands/CommandException.cpp \
  Commands/IdentifierCollection.cpp \
  Commands/ParameterValue.cpp \
  Commands/ParameterValueCollection.cpp \
  Commands/PropertyValue.cpp \
  Commands/PropertyValueCollection.cpp \
  Commands/Schema/PhysicalSchemaMappingCollection.cpp \
  Commands/Schema/PhysicalClassMapping.cpp \
  Commands/Schema/PhysicalElementMapping.cpp \
  Commands/Schema/PhysicalPropertyMapping.cpp \
  Commands/Schema/PhysicalSchemaMapping.cpp \
  Commands//SpatialContext/SpatialContextMismatchException.cpp \
  Parse/yyExpression.cpp \
  Parse/yyFilter.cpp \
  Parse/Lex.cpp \
  Parse/Parse.cpp \
  Xml/ClassMapping.cpp \
  Xml/ClassMappingCollection.cpp \
  Xml/Context.cpp \
  Xml/Deserializable.cpp \
  Xml/ElementMapping.cpp \
  Xml/ElementMappingCollection.cpp \
  Xml/Flags.cpp \
  Xml/NameCollectionHandler.cpp \
  Xml/SchemaMapping.cpp \
  Xml/Serializable.cpp \
  Xml/SCReadHandler.cpp \
  Xml/SpatialContextFlags.cpp \
  Xml/SpatialContextReader.cpp \
  Xml/SpatialContextSerializer.cpp \
  Xml/SpatialContextWriter.cpp \
  Xml/FeatureFlags.cpp \
  Xml/FeatureReader.cpp \
  Xml/FeatureReaderImpl.cpp \
  Xml/AssociationProperty.cpp \
  Xml/Box.cpp \
  Xml/CoordinateGroup.cpp \
  Xml/DataProperty.cpp \
  Xml/FeatureContext.cpp \
  Xml/FeatureHandler.cpp \
  Xml/FeaturePropertyReader.cpp \
  Xml/FeaturePropertyReaderImpl.cpp \
  Xml/GeometricProperty.cpp \
  Xml/Geometry.cpp \
  Xml/GeometryHandler.cpp \
  Xml/LineString.cpp \
  Xml/LinearRing.cpp \
  Xml/MultiGeometry.cpp \
  Xml/MultiLineString.cpp \
  Xml/MultiPoint.cpp \
  Xml/MultiPolygon.cpp \
  Xml/Point.cpp \
  Xml/Polygon.cpp \
  Xml/FeaturePropertyWriter.cpp \
  Xml/FeatureSerializer.cpp \
  Xml/GeometrySerializer.cpp \
  Xml/LpClassDefinition.cpp \
  Xml/LpGmlElementDefinition.cpp \
  Xml/LpPropertyDefinition.cpp \
  Xml/LpSchema.cpp \
  Xml/SchemaManager.cpp \
  Xml/FeatureWriter.cpp

noinst_HEADERS = $(libFDOSrc_la_SOURCES) \
  ClientServices/ProviderDef.h \
  ClientServices/RegistryUtility.h \
  ClientServices/prefix.h \
  Schema/DataTypeMapper.h \
  Schema/XmlContext.h \
  Schema/XmlError.h \
  Schema/SchemaFromExternal.h \
  Schema/SchemaFromInternal.h \
  Xml/AssociationProperty.h \
  Xml/BLOBProperty.h \
  Xml/Box.h \
  Xml/CoordinateGroup.h \
  Xml/DataProperty.h \
  Xml/FeaturePropertyReaderImpl.h \
  Xml/FeatureReaderImpl.h \
  Xml/GeometricProperty.h \
  Xml/Geometry.h \
  Xml/GeometryHandler.h \
  Xml/GeometrySerializer.h \
  Xml/LinearRing.h \
  Xml/LineString.h \
  Xml/LpClassDefinition.h \
  Xml/LpCollection.h \
  Xml/LpGmlElementDefinition.h \
  Xml/LpPropertyDefinition.h \
  Xml/LpSchema.h \
  Xml/LpSchemaElement.h \
  Xml/MultiGeometry.h \
  Xml/MultiLineString.h \
  Xml/MultiPoint.h \
  Xml/MultiPolygon.h \
  Xml/Point.h \
  Xml/Polygon.h \
  Xml/SchemaManager.h \
  Xml/SCReadHandler.h

INCLUDES = \
  -I../../Inc \
  -I../Nls  \
  -I../Common \
  -I../Geometry \
  -I$(FDOTHIRDPARTY)/apache/xerces/src

libFDOSrc_la_LDFLAGS = 

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX -DENABLE_BINRELOC

Schema/FeatureSchema.cpp : Schema/SchemaFromInternal.h

Schema/SchemaFromInternal.h : Schema/SchemaFromInternal.xslt
	@../FileToHdr/FileToHdr Schema/SchemaFromInternal.xslt Schema/SchemaFromInternal.h schemaFromInternal

Schema/FeatureSchemaCollection.cpp : Schema/SchemaFromExternal.h

Schema/SchemaFromExternal.h : Schema/SchemaFromExternal.xslt
	@../FileToHdr/FileToHdr Schema/SchemaFromExternal.xslt Schema/SchemaFromExternal.h schemaFromExternal

