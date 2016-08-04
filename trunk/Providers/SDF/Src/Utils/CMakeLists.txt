include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	${UNMANAGED_NLS_INCLUDE_DIR}
	)

add_definitions( -DSQLITE )

set( SDFUtils_STAT_SRCS
   StringValue.cpp
   NullValue.cpp
   DoubleValue.cpp
   Int64Value.cpp
   BooleanValue.cpp
   DateTimeValue.cpp
   DataValuePool.cpp
   FilterExecutor.cpp
   PropertyIndex.cpp
   OSUtils.cpp
)

add_library( SDFUtils STATIC ${SDFUtils_STAT_SRCS} )
