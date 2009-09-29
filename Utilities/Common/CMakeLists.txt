include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/Inc
	${UNMANAGED_INCLUDE_DIR}
	${UNMANAGED_COMMON_INCLUDE_DIR}
	)

if( UNIX )
	add_definitions( -DHAVE_GEOM_INFO_TYPE )
endif( UNIX )

set( ProvidersCommon_SRCS
   Src/FdoCommonNlsUtil.cpp
   Src/FdoCommonOSUtil.cpp
   Src/FdoCommonSchemaUtil.cpp
   Src/FdoCommonStringUtil.cpp
   Src/FdoCommonMiscUtil.cpp
   Src/FdoCommonFile.cpp
   Src/FdoCommonGeometryUtil.cpp
   Src/FdoCommonConnPropDictionary.cpp
   Src/FdoCommonConnStringParser.cpp
   Src/FdoCommonBinaryReader.cpp
   Src/FdoCommonBinaryWriter.cpp
   Src/FdoCommonPropertyIndex.cpp
   Src/FdoCommonThreadMutex.cpp
   Src/FdoCommonThreadUtil.cpp
   Src/stdafx.cpp
   Src/Parse/yyConstraint.cpp
   Src/Parse/Lex.cpp
   Src/Parse/Parse.cpp
   Src/FdoDefaultDataReader.cpp
   Src/FdoDefaultFeatureReader.cpp
   Src/FdoDefaultScrollableFeatureReader.cpp
   Src/FdoDefaultSqlDataReader.cpp
)

add_library( ProvidersCommon SHARED ${ProvidersCommon_SRCS} )
add_dependencies( ProvidersCommon FDO FDOMessage )
target_link_libraries( ProvidersCommon FDO )

set_target_properties( ProvidersCommon PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )
install( TARGETS ProvidersCommon DESTINATION ${LIB_INSTALL_DIR} )
