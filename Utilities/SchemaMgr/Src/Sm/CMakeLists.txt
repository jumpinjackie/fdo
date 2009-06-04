include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/Ov
	${UNMANAGED_INCLUDE_DIR}
	${FDO}/Unmanaged
	${UTILITIES_COMMON_INCLUDE_DIR}
	)

add_subdirectory( Ov )
add_subdirectory( Lp )
add_subdirectory( Ph )

set( SmOverrides_STAT_SRCS
   Ov/TableMappingTypeMapper.cpp
   Ov/GeometricColumnTypeMapper.cpp
   Ov/GeometricContentTypeMapper.cpp
   Ov/stdafx.cpp
)

set( SmLogicalPhysical_SRCS
	Lp/AssociationPropertyDefinition.cpp
	Lp/Class.cpp
	Lp/ClassDefinition.cpp
	Lp/ClassTypeMapper.cpp
	Lp/DataPropertyDefinition.cpp
	Lp/DataPropertyDefinitionCollection.cpp
	Lp/DataTypeMapper.cpp
	Lp/DbObject.cpp
	Lp/FeatureClass.cpp
	Lp/GeometricPropertyDefinition.cpp
	Lp/ObjectPropertyClass.cpp
	Lp/ObjectPropertyDefinition.cpp
	Lp/PropertyDefinition.cpp
	Lp/PropertyMappingClass.cpp
	Lp/PropertyMappingConcrete.cpp
	Lp/PropertyMappingDefinition.cpp
	Lp/PropertyMappingRelation.cpp
	Lp/PropertyMappingSingle.cpp
	Lp/PropertyTypeMapper.cpp
	Lp/QClassCollection.cpp
	Lp/Schema.cpp
	Lp/SchemaCollection.cpp
	Lp/SchemaElement.cpp
	Lp/SimplePropertyDefinition.cpp
	Lp/SpatialContext.cpp
	Lp/SpatialContextCollection.cpp
	Lp/SpatialContextMgr.cpp
	Lp/UniqueConstraint.cpp
	Lp/stdafx.cpp
)

set( SmPhysical_SRCS
	Ph/AssociationReader.cpp
	Ph/AssociationWriter.cpp
	Ph/BaseObject.cpp
	Ph/CharacterSet.cpp
	Ph/ClassPropertyReader.cpp
	Ph/ClassPropertySADReader.cpp
	Ph/ClassReader.cpp
	Ph/ClassSADReader.cpp
	Ph/ClassSOReader.cpp
	Ph/ClassSOWriter.cpp
	Ph/ClassWriter.cpp
	Ph/Collation.cpp
	Ph/Column.cpp
	Ph/ColumnChar.cpp
	Ph/ColumnDate.cpp
	Ph/ColumnDecimal.cpp
	Ph/ColumnGeom.cpp
	Ph/ColumnList.cpp
	Ph/ColumnUnknown.cpp
	Ph/ColumnVarlen.cpp
	Ph/CommandWriter.cpp
	Ph/CoordinateSystem.cpp
	Ph/CoordinateSystemCollection.cpp
	Ph/Database.cpp
	Ph/DbElement.cpp
	Ph/DbObject.cpp
	Ph/Dependency.cpp
	Ph/DependencyReader.cpp
	Ph/DependencyWriter.cpp
	Ph/Field.cpp
	Ph/Fkey.cpp
	Ph/GroupReader.cpp
	Ph/Index.cpp
	Ph/LockTypes.cpp
	Ph/MergeReader.cpp
	Ph/Mgr.cpp
	Ph/OptionsReader.cpp
	Ph/OptionsWriter.cpp
	Ph/Owner.cpp
	Ph/PropertyReader.cpp
	Ph/PropertyWriter.cpp
	Ph/Reader.cpp
	Ph/ReadWrite.cpp
	Ph/Row.cpp
	Ph/SADReader.cpp
	Ph/SADWriter.cpp
	Ph/SchemaElement.cpp
	Ph/SchemaReader.cpp
	Ph/SchemaSOReader.cpp
	Ph/SchemaSOWriter.cpp
	Ph/SchemaWriter.cpp
	Ph/SOReader.cpp
	Ph/SOWriter.cpp
	Ph/SpatialContext.cpp
	Ph/SpatialContextCollection.cpp
	Ph/SpatialContextGeom.cpp
	Ph/SpatialContextGeomReader.cpp
	Ph/SpatialContextGeomWriter.cpp
	Ph/SpatialContextGroupReader.cpp
	Ph/SpatialContextGroupWriter.cpp
	Ph/SpatialContextReader.cpp
	Ph/SpatialContextWriter.cpp
	Ph/SpatialIndex.cpp
	Ph/stdafx.cpp
	Ph/Table.cpp
	Ph/TableComponentReader.cpp
	Ph/TableMergeReader.cpp
	Ph/TempObject.cpp
	Ph/View.cpp
	Ph/Writer.cpp
)


set( SmPhCfg_SRCS
	Ph/Cfg/ClassReader.cpp
	Ph/Cfg/PropertyReader.cpp
	Ph/Cfg/SchemaReader.cpp
)

set( SmPhMt_SRCS
	Ph/Mt/AssociationReader.cpp
	Ph/Mt/ClassReader.cpp
	Ph/Mt/ClassTableJoin.cpp
	Ph/Mt/PropertyReader.cpp
	Ph/Mt/SchemaReader.cpp
	Ph/Mt/SpatialContextGeomReader.cpp
	Ph/Mt/SpatialContextGroupReader.cpp
	Ph/Mt/SpatialContextReader.cpp
)

set( SmPhRb_SRCS
	Ph/Rb/Cache.cpp
	Ph/Rb/Column.cpp
	Ph/Rb/Table.cpp
)

set( SmPhRd_SRCS
	Ph/Rd/AssociationReader.cpp
	Ph/Rd/BaseObjectReader.cpp
	Ph/Rd/CharacterSetReader.cpp
	Ph/Rd/ClassReader.cpp
	Ph/Rd/CollationReader.cpp
	Ph/Rd/ColumnReader.cpp
	Ph/Rd/ConstraintReader.cpp
	Ph/Rd/CoordSysReader.cpp
	Ph/Rd/DbObjectBinds.cpp
	Ph/Rd/DbObjectReader.cpp
	Ph/Rd/FkeyReader.cpp
	Ph/Rd/IndexReader.cpp
	Ph/Rd/Join.cpp
	Ph/Rd/OwnerReader.cpp
	Ph/Rd/PkeyReader.cpp
	Ph/Rd/PropertyReader.cpp
	Ph/Rd/QueryReader.cpp
	Ph/Rd/SchemaReader.cpp
	Ph/Rd/SpatialContextGeomReader.cpp
	Ph/Rd/SpatialContextGroupReader.cpp
	Ph/Rd/SpatialContextReader.cpp
	Ph/Rd/TableJoin.cpp
	Ph/Rd/ViewReader.cpp
)

set( SchemaMgr_SRCS
   Base.cpp
   Disposable.cpp
   Error.cpp
   SchemaElement.cpp
   SchemaManager.cpp
   stdafx.cpp
   ${SmOverrides_SRCS}
   ${SmLogicalPhysical_SRCS}
   ${SmPhysical_SRCS}
   ${SmPhCfg_SRCS}
   ${SmPhMt_SRCS}
   ${SmPhRb_SRCS}
   ${SmPhRd_SRCS}
)

add_library( SchemaMgr SHARED ${SchemaMgr_SRCS} )
set_target_properties( SchemaMgr PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )
target_link_libraries( SchemaMgr
	ProvidersCommon
	FDO )
install( TARGETS SchemaMgr DESTINATION ${LIB_INSTALL_DIR} )
