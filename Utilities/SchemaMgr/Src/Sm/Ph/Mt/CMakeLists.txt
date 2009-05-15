include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR} 
	)

set( SmPhMt_STAT_SRCS
   AssociationReader.cpp
   ClassReader.cpp
   ClassTableJoin.cpp
   PropertyReader.cpp
   SchemaReader.cpp
   SpatialContextGeomReader.cpp
   SpatialContextGroupReader.cpp
   SpatialContextReader.cpp
)

add_library( SmPhMt STATIC ${SmPhMt_STAT_SRCS} )

