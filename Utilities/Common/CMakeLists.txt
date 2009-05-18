include_directories(
	${UNMANAGED_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/Inc
	)

if( LINUX )
	add_definitions( -DLINUX_IA32 -pthread -DHAVE_GEOM_INFO_TYPE )
endif( LINUX )

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
   Src/Parse/Lex.cpp
   Src/Parse/Parse.cpp
   Src/Parse/yyConstraint.cpp
)

add_library( ProvidersCommon SHARED ${ProvidersCommon_SRCS} )
add_dependencies( ProvidersCommon FDO FDOMessage )
target_link_libraries( ProvidersCommon
	FDO
	)

set_target_properties( ProvidersCommon PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )
install( TARGETS ProvidersCommon DESTINATION ${LIB_INSTALL_DIR} )
