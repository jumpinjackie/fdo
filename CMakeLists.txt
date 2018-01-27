project(fdo)

# CMake base requires and policy tweaks
cmake_minimum_required( VERSION 2.6.2 FATAL_ERROR )
# CMP0005: keep escaping behaviour for definitions added via add_definitions()
cmake_policy( SET CMP0005 OLD )

# Look first in internal modules
set(CMAKE_MODULE_PATH 
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
)

# Set configuration if not defined
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif (NOT CMAKE_BUILD_TYPE)

message(STATUS "CMake Build Type is: ${CMAKE_BUILD_TYPE}")

# Determine bitness
if ((NOT FDO_CPU EQUAL 32) AND (NOT FDO_CPU EQUAL 64))
    message(STATUS "FDO_CPU not defined, determining what we're building for based on system bitness")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(FDO_CPU 64)
    else(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(FDO_CPU 32)
    endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message (STATUS "FDO_CPU is ${FDO_CPU}")
endif ((NOT FDO_CPU EQUAL 32) AND (NOT FDO_CPU EQUAL 64))

# Set FDO version macros
set(FDO_VERSION_MAJOR 4)
set(FDO_VERSION_MINOR 2)
set(FDO_VERSION_RELEASE 0)
if (NOT FDO_REV)
    set (FDO_REV 0)
endif (NOT FDO_REV)
set(FDO_VERSION "${FDO_VERSION_MAJOR}.${FDO_VERSION_MINOR}.${FDO_VERSION_RELEASE}")
set(FDO_DISTRIBUTION_TEXT "Self compiled sources" CACHE STRING "Indicate compilation origin")
set(FDO_VERSION_SUFFIX "-${FDO_VERSION}")

# Set CPack vars
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Feature Data Objects SDK")
set(CPACK_PACKAGE_VENDOR "OSGeo")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CMake__README.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License_README.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${FDO_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${FDO_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${FDO_VERSION_RELEASE}")
set(CPACK_PACKAGE_VERSION "${FDO_VERSION_MAJOR}.${FDO_VERSION_MINOR}.${FDO_VERSION_RELEASE}.${FDO_REV}")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)

# Collect distro name for so we know how to name these packages
# NOTE: This uses lsb_release to detect the name, so this tool must already be installed first
execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/cmake/tools/getdistro.sh OUTPUT_VARIABLE DISTRO)
message(STATUS "Detected Distro: ${DISTRO}")
if(FDO_CPU EQUAL 64)
    set(CPACK_SYSTEM_NAME "${DISTRO}-amd64")
else(FDO_CPU EQUAL 64)
    set(CPACK_SYSTEM_NAME "${DISTRO}-i386")
endif(FDO_CPU EQUAL 64)
message(STATUS "CPack system name: ${CPACK_SYSTEM_NAME}")

# Default to plain tarballs for packaging
if (NOT CPACK_GENERATOR)
    set(CPACK_GENERATOR "TGZ")
endif (NOT CPACK_GENERATOR)
message(STATUS "Using CPack generator: ${CPACK_GENERATOR}")

# If making tarballs, force monolithic packaging
if (CPACK_GENERATOR MATCHES "TGZ")
    set(CPACK_MONOLITHIC_INSTALL 1)
else (CPACK_GENERATOR MATCHES "TGZ")
    set(CPACK_MONOLITHIC_INSTALL 0)
endif (CPACK_GENERATOR MATCHES "TGZ")
message(STATUS "CPack monolithic install: ${CPACK_MONOLITHIC_INSTALL}")

# For monolithic packages, 
if (CPACK_MONOLITHIC_INSTALL)
    set (CPACK_PACKAGE_FILE_NAME "fdosdk-${FDO_VERSION}.${FDO_REV}-${CPACK_SYSTEM_NAME}")
endif (CPACK_MONOLITHIC_INSTALL)

if (CMAKE_BUILD_TYPE MATCHES Release)
    message(STATUS "Binaries will be stripped as part of packaging")
endif (CMAKE_BUILD_TYPE MATCHES Release)

# Make CPack available to easy generate binary packages
include(CPack)

# Some helper macros
macro(install_symlink target linkname component)
    # The link target doesn't have to exist at this point in time, the symlink will be valid at install time
    execute_process(COMMAND ln -sf ${target} ${CMAKE_CURRENT_BINARY_DIR}/${linkname})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${linkname} DESTINATION ${LIB_INSTALL_DIR} COMPONENT ${component})
endmacro(install_symlink)

# Override install directory to match what the automake build does (/usr/local, rather than /usr/local/lib)
set( INSTALL_PREFIX "/usr/local/fdo-${FDO_VERSION}" )

# FDO Build Directories
set( FDO ${CMAKE_CURRENT_SOURCE_DIR}/Fdo )
if (NOT INTERNAL_THIRDPARTY_DIR)
    set( FDOTHIRDPARTY ${CMAKE_CURRENT_SOURCE_DIR}/Thirdparty )
else (NOT INTERNAL_THIRDPARTY_DIR)
    set( FDOTHIRDPARTY ${INTERNAL_THIRDPARTY_DIR}/Thirdparty )
endif (NOT INTERNAL_THIRDPARTY_DIR)
set( FDOUTILITIES ${CMAKE_CURRENT_SOURCE_DIR}/Utilities )

message(STATUS "FDOTHIRDPARTY is: ${FDOTHIRDPARTY}")

if( UNIX )
    add_definitions( -DFULLPROTO -fPIC -DPIC )
endif( UNIX )

if (CMAKE_SYSTEM_NAME MATCHES Linux)
    add_definitions( -DLINUX -DLINUX_IA32 -pthread )
endif (CMAKE_SYSTEM_NAME MATCHES Linux)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions( -D__USE_GNU -Wno-write-strings -Wno-deprecated )
    set ( CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed -Wl,--no-undefined -lc ${CMAKE_SHARED_LINKER_FLAGS}")
    if(FDO_CPU EQUAL 64)
        set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64" )
        set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64" )
        message(STATUS "Setting CFLAGS for 64-bit build")
    else(FDO_CPU EQUAL 64)
        set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32" )
        set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32" )
        message(STATUS "Setting CFLAGS for 32-bit build")
    endif(FDO_CPU EQUAL 64)
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        add_definitions(-DDEBUG -D_DEBUG)
        set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3" )
        set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3" )
        message(STATUS "Adding standard compiler flags for Debug")
    endif (CMAKE_BUILD_TYPE MATCHES Debug)
endif( CMAKE_COMPILER_IS_GNUCXX )

if (USE_LD_GOLD)
    execute_process(COMMAND ${CMAKE_C_COMPILER} -fuse-ld=gold -Wl,--version ERROR_QUIET OUTPUT_VARIABLE LD_VERSION)
    if ("${LD_VERSION}" MATCHES "GNU gold")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fuse-ld=gold")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
        message(STATUS "Using GNU gold linker instead of ld")
    else ()
        message(WARNING "GNU gold linker isn't available, using the default system linker.")
    endif ()
endif ()

find_package(Sanitizers)
find_package(FDOThirdparty REQUIRED)
find_package(FDOInternal REQUIRED)

# Check Python
find_package(PythonLibs)

add_subdirectory(Thirdparty)
add_subdirectory(Fdo)
add_subdirectory(Utilities)

# Providers compiled internally are optional and is not added if
# Providers directory not exists
# Correct syntax is: -DWITH_<provider>
# Ex. -DWITH_GDAL
macro_optional_add_subdirectory(Providers)

# Copy providers.cmake.linux.xml to our build staging area as providers.xml so it can be installed from there
execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/providers.cmake.linux.xml ${CMAKE_CURRENT_BINARY_DIR}/providers.xml)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/providers.xml DESTINATION ${LIB_INSTALL_DIR})
configure_file(cmake/modules/FindFDO.cmake.tmpl ${CMAKE_CURRENT_BINARY_DIR}/cmake/modules/FindFDO.cmake @ONLY)
# Install the global template, not the internal one used for whole source build
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/modules/FindFDO.cmake DESTINATION ${CMAKE_ROOT}/Modules)

# If we're using internal xerces/xalan/gdal we must make sure these libs are also installed
if (INTERNAL_XERCESC)
    install(FILES ${FDOTHIRDPARTY}/apache/xerces/lib/libxerces-c-3.1.so DESTINATION ${LIB_INSTALL_DIR})
    install(FILES ${FDOTHIRDPARTY}/apache/xerces/lib/libxerces-c.so DESTINATION ${LIB_INSTALL_DIR})
endif (INTERNAL_XERCESC)
if (INTERNAL_XALANC)
    install(FILES ${FDOTHIRDPARTY}/apache/xalan/lib/libxalan-c.so DESTINATION ${LIB_INSTALL_DIR})
    install(FILES ${FDOTHIRDPARTY}/apache/xalan/lib/libxalan-c.so.111 DESTINATION ${LIB_INSTALL_DIR})
    install(FILES ${FDOTHIRDPARTY}/apache/xalan/lib/libxalan-c.so.111.0 DESTINATION ${LIB_INSTALL_DIR})
    install(FILES ${FDOTHIRDPARTY}/apache/xalan/lib/libxalanMsg.so DESTINATION ${LIB_INSTALL_DIR})
    install(FILES ${FDOTHIRDPARTY}/apache/xalan/lib/libxalanMsg.so.111 DESTINATION ${LIB_INSTALL_DIR})
    install(FILES ${FDOTHIRDPARTY}/apache/xalan/lib/libxalanMsg.so.111.0 DESTINATION ${LIB_INSTALL_DIR})
endif (INTERNAL_XALANC)
if (INTERNAL_GDAL)
    # The link target doesn't have to exist at this point in time, the symlink will be valid at install time
    execute_process(COMMAND ln -sf ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libgdal.so.20.3.2 ${CMAKE_CURRENT_BINARY_DIR}/libgdal.so.20)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libgdal.so.20 DESTINATION ${LIB_INSTALL_DIR} COMPONENT ${component})
    execute_process(COMMAND ln -sf ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/libgdal.so.20.3.2 ${CMAKE_CURRENT_BINARY_DIR}/libgdal.so)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libgdal.so DESTINATION ${LIB_INSTALL_DIR} COMPONENT ${component})
    install(FILES ${FDOTHIRDPARTY}/gdal/lib/libgdal.so.20.3.2 DESTINATION ${LIB_INSTALL_DIR})
endif (INTERNAL_GDAL)
