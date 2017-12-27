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

set ( FDO_VERSION_MAJOR 4 )
set ( FDO_VERSION_MINOR 2 )
set ( FDO_VERSION_RELEASE 0 )
set ( FDO_VERSION "${FDO_VERSION_MAJOR}.${FDO_VERSION_MINOR}.${FDO_VERSION_RELEASE}" )
set ( FDO_DISTRIBUTION_TEXT "Self compiled sources" CACHE STRING "Indicate compilation origin" )

macro(install_symlink filepath sympath)
    install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${filepath} ${sympath})")
    install(CODE "message(\"-- Created symlink: ${sympath} -> ${filepath}\")")
endmacro(install_symlink)

# Override install directory to match what the automake build does (/usr/local, rather than /usr/local/lib)
set( INSTALL_PREFIX "/usr/local/fdo-${FDO_VERSION}" )

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

message(STATUS "CMake Build Type is: ${CMAKE_BUILD_TYPE}")

if( CMAKE_COMPILER_IS_GNUCXX )
    add_definitions( -D__USE_GNU -Wno-deprecated )
    set ( CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed -Wl,--no-undefined -lc ${CMAKE_SHARED_LINKER_FLAGS}")
    if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64" )
        set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64" )
        message(STATUS "Setting CFLAGS for 64-bit build")
    else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32" )
        set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32" )
        message(STATUS "Setting CFLAGS for 32-bit build")
    endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        add_definitions(-DDEBUG -D_DEBUG)
        set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3" )
        set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3" )
        message(STATUS "Adding standard compiler flags for Debug")
    endif (CMAKE_BUILD_TYPE MATCHES Debug)
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
# Ex. -DWITH_GDAL
macro_optional_add_subdirectory( Providers )

install(FILES providers.xml DESTINATION ${LIB_INSTALL_DIR} )
configure_file( cmake/modules/FindFDO.cmake.tmpl ${CMAKE_CURRENT_BINARY_DIR}/cmake/modules/FindFDO.cmake @ONLY )
# Install the global template, not the internal one used for whole source build
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/modules/FindFDO.cmake DESTINATION ${CMAKE_ROOT}/Modules )
