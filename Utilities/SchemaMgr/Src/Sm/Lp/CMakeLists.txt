include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${FDO}/Unmanaged
	${UTILITIES_COMMON_INCLUDE_DIR}
	)

set( SmLogicalPhysical_SRCS
	AssociationPropertyDefinition.cpp
	Class.cpp
	ClassDefinition.cpp
	ClassTypeMapper.cpp
	DataPropertyDefinition.cpp
	DataPropertyDefinitionCollection.cpp
	DataTypeMapper.cpp
	DbObject.cpp
	FeatureClass.cpp
	GeometricPropertyDefinition.cpp
	ObjectPropertyClass.cpp
	ObjectPropertyDefinition.cpp
	PropertyDefinition.cpp
	PropertyMappingClass.cpp
	PropertyMappingConcrete.cpp
	PropertyMappingDefinition.cpp
	PropertyMappingRelation.cpp
	PropertyMappingSingle.cpp
	PropertyTypeMapper.cpp
	QClassCollection.cpp
	Schema.cpp
	SchemaCollection.cpp
	SchemaElement.cpp
	SimplePropertyDefinition.cpp
	SpatialContext.cpp
	SpatialContextCollection.cpp
	SpatialContextMgr.cpp
	UniqueConstraint.cpp
	stdafx.cpp
)

add_library( SmLogicalPhysical STATIC ${SmLogicalPhysical_SRCS} )
