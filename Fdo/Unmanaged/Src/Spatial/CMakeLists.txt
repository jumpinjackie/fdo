include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)

add_definitions( -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE )

set( FDOSpatial_STAT_SRCS
   MathUtility.cpp
   SpatialEnvelope.cpp
   SpatialUtility.cpp
   SpatialUtilityGeometryExtents.cpp
   SpatialUtilityCircularArc.cpp
   SpatialGeometryConverter.cpp
)

add_library( FDOSpatial STATIC ${FDOSpatial_STAT_SRCS} )
