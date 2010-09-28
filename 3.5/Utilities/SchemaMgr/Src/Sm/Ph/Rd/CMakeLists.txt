include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${FDO}/Unmanaged
	${UTILITIES_COMMON_INCLUDE_DIR}
	)

set( SmPhRd_SRCS
	AssociationReader.cpp
	BaseObjectReader.cpp
	CharacterSetReader.cpp
	ClassReader.cpp
	CollationReader.cpp
	ColumnReader.cpp
	ConstraintReader.cpp
	CoordSysReader.cpp
	DbObjectBinds.cpp
	DbObjectReader.cpp
	FkeyReader.cpp
	IndexReader.cpp
	Join.cpp
	OwnerReader.cpp
	PkeyReader.cpp
	PropertyReader.cpp
	QueryReader.cpp
	SchemaDbObjectBinds.cpp
	SchemaReader.cpp
	SpatialContextGeomReader.cpp
	SpatialContextGroupReader.cpp
	SpatialContextReader.cpp
	TableJoin.cpp
	ViewReader.cpp
)


add_library( SmPhRd STATIC ${SmPhRd_SRCS} )
add_dependencies( SmPhRd SMMessage )
