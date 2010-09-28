include_directories( 
	BEFORE
	${UNMANAGED_INCLUDE_DIR}
	${UNMANAGED_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
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

target_link_libraries( SchemaMgr
	ProvidersCommon
	FDO )

add_dependencies( SchemaMgr SMMessage )

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


set_target_properties( SchemaMgr PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )
install( TARGETS SchemaMgr DESTINATION ${LIB_INSTALL_DIR} )
