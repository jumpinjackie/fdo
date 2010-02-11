include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Common
		${CMAKE_CURRENT_SOURCE_DIR}/Spatial
        ${CMAKE_CURRENT_SOURCE_DIR}/Nls
        ${CMAKE_CURRENT_SOURCE_DIR}/Geometry
		${CMAKE_CURRENT_BINARY_DIR}/Message
		)
	
if( UNIX )
	add_definitions( -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -DENABLE_BINRELOC )
endif( UNIX )

add_subdirectory( McToMsf )
add_subdirectory( Message )
add_subdirectory( FileToHdr )
add_subdirectory( Nls )
add_subdirectory( Common )
add_subdirectory( Geometry )
add_subdirectory( Spatial )
add_subdirectory( Fdo )

set( FDOSrc_SRCS
	Fdo/Fdo.cpp
	Fdo/IDisposableCollection.cpp
	Fdo/Expression/BLOBValue.cpp
	Fdo/Expression/BinaryExpression.cpp
	Fdo/Expression/BooleanValue.cpp
	Fdo/Expression/ByteValue.cpp
	Fdo/Expression/CLOBValue.cpp
	Fdo/Expression/DataValue.cpp
	Fdo/Expression/DateTimeValue.cpp
	Fdo/Expression/DecimalValue.cpp
	Fdo/Expression/DoubleValue.cpp
	Fdo/Expression/Expression.cpp
	Fdo/Expression/ExpressionCollection.cpp
	Fdo/Expression/ExpressionException.cpp
	Fdo/Expression/Function.cpp
	Fdo/Expression/GeometryValue.cpp
	Fdo/Expression/Identifier.cpp
	Fdo/Expression/ComputedIdentifier.cpp
	Fdo/Expression/Int16Value.cpp
	Fdo/Expression/Int32Value.cpp
	Fdo/Expression/Int64Value.cpp
	Fdo/Expression/IntBinValue.cpp
	Fdo/Expression/IntHexValue.cpp
	Fdo/Expression/ExpressionInternal.cpp
	Fdo/Expression/Parameter.cpp
	Fdo/Expression/SingleValue.cpp
	Fdo/Expression/StringValue.cpp
	Fdo/Expression/UnaryExpression.cpp
	Fdo/Expression/DataValueCollection.cpp
	Fdo/Expression/LiteralValueCollection.cpp
	Fdo/Filter/BinaryLogicalOperator.cpp
	Fdo/Filter/ComparisonCondition.cpp
	Fdo/Filter/DistanceCondition.cpp
	Fdo/Filter/Filter.cpp
	Fdo/Filter/FilterException.cpp
	Fdo/Filter/GeometricCondition.cpp
	Fdo/Filter/InCondition.cpp
	Fdo/Filter/NullCondition.cpp
	Fdo/Filter/SpatialCondition.cpp
	Fdo/Filter/UnaryLogicalOperator.cpp
	Fdo/Filter/ValueExpressionCollection.cpp
	Fdo/Raster/RasterDataModel.cpp
	Fdo/Schema/AutogenerationException.cpp
	Fdo/Schema/Class.cpp
	Fdo/Schema/ClassCapabilities.cpp
	Fdo/Schema/ClassCollection.cpp
	Fdo/Schema/ClassDefinition.cpp
	Fdo/Schema/DataPropertyDefinition.cpp
	Fdo/Schema/DataPropertyDefinitionCollection.cpp
	Fdo/Schema/DataTypeMapper.cpp
	Fdo/Schema/FeatureClass.cpp
	Fdo/Schema/FeatureClassCollection.cpp
	Fdo/Schema/FeatureSchema.cpp
	Fdo/Schema/FeatureSchemaCollection.cpp
	Fdo/Schema/GeometricPropertyDefinition.cpp
	Fdo/Schema/ObjectPropertyDefinition.cpp
	Fdo/Schema/PropertyDefinition.cpp
	Fdo/Schema/PropertyDefinitionCollection.cpp
	Fdo/Schema/ReadOnlyDataPropertyDefinitionCollection.cpp
	Fdo/Schema/ReadOnlyPropertyDefinitionCollection.cpp
	Fdo/Schema/AssociationPropertyDefinition.cpp
	Fdo/Schema/SchemaAttributeDictionary.cpp
	Fdo/Schema/SchemaElement.cpp
	Fdo/Schema/SchemaException.cpp
	Fdo/Schema/RasterPropertyDefinition.cpp
	Fdo/Schema/MergeContext.cpp
	Fdo/Schema/XmlContext.cpp
	Fdo/Schema/XmlError.cpp
	Fdo/Schema/NetworkClass.cpp
	Fdo/Schema/NetworkFeatureClass.cpp
	Fdo/Schema/NetworkLayerClass.cpp
	Fdo/Schema/NetworkNodeFeatureClass.cpp
	Fdo/Schema/NetworkLinkFeatureClass.cpp
	Fdo/Schema/TopoFeaturePropertyDefinition.cpp
	Fdo/Schema/TopoGeometryPropertyDefinition.cpp
	Fdo/Schema/Topology.cpp
	Fdo/Schema/PropertyValueConstraintList.cpp
	Fdo/Schema/PropertyValueConstraintRange.cpp
	Fdo/Schema/UniqueConstraint.cpp
	Fdo/Schema/UniqueConstraintCollection.cpp
	Fdo/ClientServices/ClientServiceException.cpp
	Fdo/ClientServices/ConnectionManager.cpp
	Fdo/ClientServices/FeatureAccessManager.cpp
	Fdo/ClientServices/Provider.cpp
	Fdo/ClientServices/ProviderCollection.cpp
	Fdo/ClientServices/ProviderNameTokens.cpp
	Fdo/ClientServices/ProviderRegistry.cpp
	Fdo/ClientServices/RegistryUtility.cpp
	Fdo/ClientServices/prefix.cpp
	Fdo/Connections/ConnectionException.cpp
	Fdo/Connections/Capabilities/ArgumentDefinition.cpp
	Fdo/Connections/Capabilities/ArgumentDefinitionCollection.cpp
	Fdo/Connections/Capabilities/FunctionDefinition.cpp
	Fdo/Connections/Capabilities/FunctionDefinitionCollection.cpp
	Fdo/Connections/Capabilities/ReadOnlyArgumentDefinitionCollection.cpp
	Fdo/Connections/Capabilities/ISchemaCapabilities.cpp
	Fdo/Connections/Capabilities/IExpressionCapabilities.cpp
	Fdo/Connections/Capabilities/SignatureDefinition.cpp
	Fdo/Commands/BatchParameterValueCollection.cpp
	Fdo/Commands/CommandException.cpp
	Fdo/Commands/IdentifierCollection.cpp
	Fdo/Commands/ParameterValue.cpp
	Fdo/Commands/ParameterValueCollection.cpp
	Fdo/Commands/PropertyValue.cpp
	Fdo/Commands/PropertyValueCollection.cpp
	Fdo/Commands/Schema/PhysicalSchemaMappingCollection.cpp
	Fdo/Commands/Schema/PhysicalClassMapping.cpp
	Fdo/Commands/Schema/PhysicalElementMapping.cpp
	Fdo/Commands/Schema/PhysicalPropertyMapping.cpp
	Fdo/Commands/Schema/PhysicalSchemaMapping.cpp
	Fdo/Commands//SpatialContext/SpatialContextMismatchException.cpp
	Fdo/Parse/yyExpression.cpp
	Fdo/Parse/yyFilter.cpp
	Fdo/Parse/Lex.cpp
	Fdo/Parse/Parse.cpp
	Fdo/Xml/ClassMapping.cpp
	Fdo/Xml/ClassMappingCollection.cpp
	Fdo/Xml/Context.cpp
	Fdo/Xml/Deserializable.cpp
	Fdo/Xml/ElementMapping.cpp
	Fdo/Xml/ElementMappingCollection.cpp
	Fdo/Xml/Flags.cpp
	Fdo/Xml/NameCollectionHandler.cpp
	Fdo/Xml/SchemaMapping.cpp
	Fdo/Xml/Serializable.cpp
	Fdo/Xml/SCReadHandler.cpp
	Fdo/Xml/SpatialContextFlags.cpp
	Fdo/Xml/SpatialContextReader.cpp
	Fdo/Xml/SpatialContextSerializer.cpp
	Fdo/Xml/SpatialContextWriter.cpp
	Fdo/Xml/FeatureFlags.cpp
	Fdo/Xml/FeatureReader.cpp
	Fdo/Xml/FeatureReaderImpl.cpp
	Fdo/Xml/AssociationProperty.cpp
	Fdo/Xml/Box.cpp
	Fdo/Xml/CoordinateGroup.cpp
	Fdo/Xml/DataProperty.cpp
	Fdo/Xml/FeatureContext.cpp
	Fdo/Xml/FeatureHandler.cpp
	Fdo/Xml/FeaturePropertyReader.cpp
	Fdo/Xml/FeaturePropertyReaderImpl.cpp
	Fdo/Xml/GeometricProperty.cpp
	Fdo/Xml/Geometry.cpp
	Fdo/Xml/GeometryHandler.cpp
	Fdo/Xml/LineString.cpp
	Fdo/Xml/LinearRing.cpp
	Fdo/Xml/MultiGeometry.cpp
	Fdo/Xml/MultiLineString.cpp
	Fdo/Xml/MultiPoint.cpp
	Fdo/Xml/MultiPolygon.cpp
	Fdo/Xml/Point.cpp
	Fdo/Xml/Polygon.cpp
	Fdo/Xml/FeaturePropertyWriter.cpp
	Fdo/Xml/FeatureSerializer.cpp
	Fdo/Xml/GeometrySerializer.cpp
	Fdo/Xml/LpClassDefinition.cpp
	Fdo/Xml/LpGmlElementDefinition.cpp
	Fdo/Xml/LpPropertyDefinition.cpp
	Fdo/Xml/LpSchema.cpp
	Fdo/Xml/SchemaManager.cpp
	Fdo/Xml/FeatureWriter.cpp
)

set( FDOCommon_SRCS
    Common/IDisposable.cpp
	Common/ArrayHelper.cpp
	Common/CommonInternal.cpp
	Common/Context.cpp
	Common/DictionaryElement.cpp
	Common/Exception.cpp
	Common/Semaphore.cpp
	Common/StringP.cpp
	Common/StringCollection.cpp
	Common/StringUtility.cpp
	Common/Vector.cpp
	Common/ConvertUTF.cpp
	Common/utf8_.cpp
	Common/Io/BufferStream.cpp
	Common/Io/ByteStreamReader.cpp
	Common/Io/MemoryStream.cpp
	Common/Io/FileStream.cpp
	Common/Io/TextReader.cpp
	Common/Io/TextWriter.cpp
	Common/Xml/Attribute.cpp
	Common/Xml/CharDataHandler.cpp
	Common/Xml/CopyHandler.cpp
	Common/Xml/Reader.cpp
	Common/Xml/ReaderXrcs.cpp
	Common/Xml/SaxContext.cpp
	Common/Xml/SaxHandler.cpp
	Common/Xml/UtilXrcs.cpp
	Common/Xml/Writer.cpp
	Common/Xml/WriterXrcs.cpp
	Common/Xml/Xml.cpp
	Common/Xml/XmlException.cpp
	Common/Xsl/Transformer.cpp
	Common/Xsl/TransformerXalan.cpp
	Common/Xsl/XML885915Transcoder.cpp
	Common/Gml212/Schema.cpp
	Common/Gml212/Gml212.cpp
)

set( FDOSpatial_SRCS
	Spatial/MathUtility.cpp
	Spatial/SpatialEnvelope.cpp
	Spatial/SpatialUtility.cpp
	Spatial/SpatialUtilityGeometryExtents.cpp
	Spatial/SpatialUtilityCircularArc.cpp
	Spatial/SpatialGeometryConverter.cpp
)

set( FDOGeometry_SRCS
	Geometry/Parse/yyFgft.cpp
	Geometry/Fgf/CircularArcSegment.cpp
	Geometry/Fgf/CurvePolygon.cpp
	Geometry/Fgf/CurveString.cpp
	Geometry/Fgf/GeometryFactory.cpp
	Geometry/Fgf/LineString.cpp
	Geometry/Fgf/LineStringSegment.cpp
	Geometry/Fgf/LinearRing.cpp
	Geometry/Fgf/MultiCurvePolygon.cpp
	Geometry/Fgf/MultiCurveString.cpp
	Geometry/Fgf/MultiGeometry.cpp
	Geometry/Fgf/MultiLineString.cpp
	Geometry/Fgf/MultiPoint.cpp
	Geometry/Fgf/MultiPolygon.cpp
	Geometry/Fgf/Point.cpp
	Geometry/Fgf/Polygon.cpp
	Geometry/Fgf/Ring.cpp
	Geometry/Fgf/Util.cpp
	Geometry/Fgf/GeometryDll.cpp
	Geometry/Fgf/GeometryThreadData.cpp
	Geometry/DirectPositionImpl.cpp
	Geometry/EnvelopeImpl.cpp
	Geometry/GeometryFactoryAbstract.cpp
	Geometry/GeometryUtility.cpp
	Geometry/Parse/LexFgft.cpp
	Geometry/Parse/ParseFgft.cpp
)

set( FDONLS_SRCS
	Nls/msg_get.c
	Nls/cat_open.c
	Nls/locale_s.c
)

set( FDO_SRCS
	${FDOSrc_SRCS}
	${FDOCommon_SRCS}
	${FDOSpatial_SRCS}
	${FDOGeometry_SRCS}
	${FDONLS_SRCS}
	)

add_library( FDO SHARED ${FDO_SRCS} )
add_dependencies( FDO 
	McToMsf 
	FDOMessage 
	SchemaFromInternal 
	ProcessedFiles )

target_link_libraries( FDO
	xalan-c
	xalanMsg
	xerces-c
	pthread
	dl
	)

set_target_properties( FDO PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )

install( TARGETS FDO DESTINATION ${LIB_INSTALL_DIR} )

