include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${FDO}/Unmanaged
	${UTILITIES_COMMON_INCLUDE_DIR}
	)

set( SmPhMt_SRCS
	AssociationReader.cpp
	ClassReader.cpp
	ClassTableJoin.cpp
	PropertyReader.cpp
	SchemaReader.cpp
	SpatialContextGeomReader.cpp
	SpatialContextGroupReader.cpp
	SpatialContextReader.cpp
)


add_library( SmPhMt STATIC ${SmPhMt_SRCS} )
add_dependencies( SmPhMt SMMessage )
