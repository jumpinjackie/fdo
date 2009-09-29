include_directories(
	BEFORE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${UNMANAGED_DIR}
	${UTULITIES_SCHEMAMGR_INCLUDE_DIR}
	${UTULITIES_COMMON_INCLUDE_DIR}
	)

set( SmPhRdGrd_STAT_SRCS
   FieldArray.cpp
   QueryReader.cpp
   RowArray.cpp
)

add_library( SmPhRdGrd STATIC ${SmPhRdGrd_STAT_SRCS} )
add_dependencies( SmPhRdGrd fdordbmsmsg )
