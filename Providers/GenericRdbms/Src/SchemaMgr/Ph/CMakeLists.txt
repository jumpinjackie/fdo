include_directories(
	BEFORE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UTULITIES_SCHEMAMGR_INCLUDE_DIR}
	${UTULITIES_COMMON_INCLUDE_DIR}
	)

add_subdirectory( Cfg )
add_subdirectory( Rd )

set( SmPhGrd_STAT_SRCS
   ClassWriter.cpp
   CommandWriter.cpp
   Fkey.cpp
   Index.cpp
   Mgr.cpp
   NullIndicator.cpp
   Owner.cpp
   SpatialContextGroupWriter.cpp
   SpatialContextWriter.cpp
   Table.cpp
   View.cpp
)

add_library( SmPhGrd STATIC ${SmPhGrd_STAT_SRCS} )
add_dependencies( SmPhGrd fdordbmsmsg )
