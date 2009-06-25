include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbmsMySQL_SchemaMgr_Lp_STAT_SRCS
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

add_library( FdoRdbmsMySQL_SchemaMgr_Lp STATIC ${FdoRdbmsMySQL_SchemaMgr_Lp_STAT_SRCS} )
