include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbmsODBC_SchemaMgr_Lp_STAT_SRCS
   Class.cpp
   ClassDefinition.cpp
   DataPropertyDefinition.cpp
   FeatureClass.cpp
   GeometricPropertyDefinition.cpp
   SchemaCollection.cpp
   Schema.cpp
)

add_library( FdoRdbmsODBC_SchemaMgr_Lp STATIC ${FdoRdbmsODBC_SchemaMgr_Lp_STAT_SRCS} )
