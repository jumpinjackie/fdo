# This file is strictly to check and enable systrm or internal Thirdparty
# libraries

# Check for Postgresql or ThirdParty
if( NOT INTERNAL_POSTGRESQL OR NOT FDO_CORE_BUILD )
	find_package( PostgreSQL REQUIRED )
else( NOT INTERNAL_POSTGRESQL OR NOT FDO_CORE_BUILD )
	set( LOCAL_POSTGRESQL TRUE )
	set( POSTGRESQL_INCLUDE_DIR ${FDOTHIRDPARTY}/pgsql/include )
	if( UNIX )
		set( POSTGRESQL_LIBRARIES ${FDOTHIRDPARTY}/pgsql/lib )
	endif( UNIX )
	mark_as_advanced( POSTGRESQL_INCLUDE_DIR POSTGRESQL_LIBRARIES )
endif( NOT INTERNAL_POSTGRESQL OR NOT FDO_CORE_BUILD )
