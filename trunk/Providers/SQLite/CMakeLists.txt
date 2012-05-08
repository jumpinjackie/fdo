project(fdosqlite)

# Make CPack available to easy generate binary packages
include(CPack)

if( NOT FDO_CORE_BUILD )
	find_package( FDO REQUIRED )
endif( NOT FDO_CORE_BUILD )

add_subdirectory( Src )
