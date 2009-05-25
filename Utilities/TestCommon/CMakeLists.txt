include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/Inc
	${FDOUTILITIES}/Common/Inc
	${UNMANAGED_INCLUDE_DIRS}
	${CPPUNIT_INCLUDE_DIRS}
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

add_library( TestCommon SHARED ${TestCommon_STAT_SRCS} )

set_target_properties( TestCommon PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )
install( TARGETS TestCommon DESTINATION ${LIB_INSTALL_DIR} )
target_link_libraries( TestCommon 
	FDO
	ProvidersCommon
	${CPPUNIT_LIBRARIES}
	)

