include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbmsPostGis_SchemaMgr_Lp_STAT_SRCS
   AssociationPropertyDefinition.cpp
   Class.cpp
   ClassDefinition.cpp
   DataPropertyDefinition.cpp
   FeatureClass.cpp
   GeometricPropertyDefinition.cpp
   ObjectPropertyClass.cpp
   ObjectPropertyDefinition.cpp
   SchemaCollection.cpp
   Schema.cpp
)

add_library( FdoRdbmsPostGis_SchemaMgr_Lp STATIC ${FdoRdbmsPostGis_SchemaMgr_Lp_STAT_SRCS} )
add_dependencies( FdoRdbmsPostGis_SchemaMgr_Lp fdordbmsmsg )
