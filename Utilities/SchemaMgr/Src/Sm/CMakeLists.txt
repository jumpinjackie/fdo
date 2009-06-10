include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${UNMANAGED_INCLUDE_DIR}
	${FDO}/Unmanaged
	${UTILITIES_COMMON_INCLUDE_DIR}
	)

add_subdirectory( Ov )
add_subdirectory( Lp )
add_subdirectory( Ph )

set( SchemaMgr_SRCS
   Base.cpp
   Disposable.cpp
   Error.cpp
   SchemaElement.cpp
   SchemaManager.cpp
   stdafx.cpp
)

add_library( SchemaMgr SHARED ${SchemaMgr_SRCS} )
set_target_properties( SchemaMgr PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )
target_link_libraries( SchemaMgr
	ProvidersCommon
	FDO )

if( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( SchemaMgr
		-Wl,-whole-archive
		SmOverrides
		SmLogicalPhysical
		SmPhysical
		SmPhCfg
		SmPhMt
		SmPhRb
		SmPhRd
		-Wl,-no-whole-archive
		)
else( CMAKE_COMPILER_IS_GNUCXX )
	target_link_libraries( SchemaMgr
		SmOverrides
		SmLogicalPhysical
		SmPhysical
		SmPhCfg
		SmPhMt
		SmPhRb
		SmPhRd
		)
endif( CMAKE_COMPILER_IS_GNUCXX )



install( TARGETS SchemaMgr DESTINATION ${LIB_INSTALL_DIR} )
