include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${UNMANAGED_INCLUDE_DIR}
	)

set( geometry_fgf_STAT_SRCS
   AllGeometry_c.cpp
)

add_library( geometry_fgf STATIC ${geometry_fgf_STAT_SRCS} )

add_dependencies( geometry_fgf fdordbmsmsg )
