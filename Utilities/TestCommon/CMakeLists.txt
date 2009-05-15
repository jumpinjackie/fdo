include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${FDO}/Unmanaged/Inc
	${CMAKE_CURRENT_SOURCE_DIR}/Inc
	${FDOTHIRDPARTY}/linux/cppunit/include
	${FDOUTILITIES}/Common/Inc
	${FDO}/Unmanaged/Src/Common
	)

if( UNIX )
	add_definitions( -DLINUX_IA32 -pthread -DHAVE_GEOM_INFO_TYPE )
endif( UNIX )

set( TestCommon_STAT_SRCS
   Src/TestCommonConstraints.cpp
   Src/TestCommonFeatureCommands.cpp
   Src/TestCommonInsert.cpp
   Src/TestCommonMiscUtil.cpp
   Src/TestCommonFileUtil.cpp
   Src/TestCommonGeomUtil.cpp
   Src/TestCommonSchemaUtil.cpp
   Src/stdafx.cpp
)

add_library( TestCommon STATIC ${TestCommon_STAT_SRCS} )
