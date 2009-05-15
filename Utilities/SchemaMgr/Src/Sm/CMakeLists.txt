include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${FDO}/Unmanaged/Inc
	${FDO}/Unmanaged
	${FDOUTILITIES}/Common/Inc
	)

add_subdirectory( Ov )
add_subdirectory( Lp )
add_subdirectory( Ph )

set( SchemaMgr_STAT_SRCS
   Base.cpp
   Disposable.cpp
   Error.cpp
   SchemaElement.cpp
   SchemaManager.cpp
   stdafx.cpp
)

add_library( SchemaMgr STATIC ${SchemaMgr_STAT_SRCS} )
