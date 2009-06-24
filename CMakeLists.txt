project(fdo)

# CMake base requires and policy tweaks
cmake_minimum_required( VERSION 2.6.2 FATAL_ERROR )
# CMP0005: keep escaping behaviour for definitions added via add_definitions()
cmake_policy( SET CMP0005 OLD )

# Look first in internal modules
set(CMAKE_MODULE_PATH 
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
	)

# Make CPack available to easy generate binary packages
include(CPack)


# Set FDO version macros

set ( FDO_VERSION_MAJOR 3 )
set ( FDO_VERSION_MINOR 5 )
set ( FDO_VERSION_RELEASE 0 )
set ( FDO_VERSION "${FDO_VERSION_MAJOR}.${FDO_VERSION_MINOR}.${FDO_VERSION_RELEASE}" )
set ( FDO_DISTRIBUTION_TEXT "Self compiled sources" CACHE STRING "Indicate compilation origin" )


# FDO Build Directories
set( FDO ${CMAKE_CURRENT_SOURCE_DIR}/Fdo )
set( FDOTHIRDPARTY ${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty )
set( FDOUTILITIES ${CMAKE_CURRENT_SOURCE_DIR}/Utilities )

if( UNIX )
	add_definitions( -DFULLPROTO -fPIC -DPIC )
endif( UNIX )

if (CMAKE_SYSTEM_NAME MATCHES Linux)
	add_definitions( -DLINUX -DLINUX_IA32 -pthread )
endif (CMAKE_SYSTEM_NAME MATCHES Linux)

if( CMAKE_COMPILER_IS_GNUCXX )
	add_definitions( -D__USE_GNU -Wno-deprecated )
endif( CMAKE_COMPILER_IS_GNUCXX )

find_package( FDOThirdparty REQUIRED )
find_package( FDOInternal REQUIRED )

# Check Python
find_package( PythonLibs REQUIRED )

add_subdirectory( Thirdparty )
add_subdirectory( Fdo )
add_subdirectory( Utilities )

# Providers compiled internally are optional and is not added if
# Providers directory not exists
# Correct syntax is: -DWITH_<provider>
# Ex. -DWITH_POSTGIS -DWITH_GDAL
macro_optional_add_subdirectory( Providers )

install(FILES providers.xml DESTINATION ${LIB_INSTALL_DIR} )
configure_file( cmake/modules/FindFDO.cmake.tmpl ${CMAKE_CURRENT_BINARY_DIR}/cmake/modules/FindFDO.cmake @ONLY )
# Install the global template, not the internal one used for whole source build
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/modules/FindFDO.cmake DESTINATION ${CMAKE_ROOT}/Modules )
