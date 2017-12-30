project(fdogenericrdbms)

# TODO: Eventually we should split this into its respective ODBC, MySQL, PostgreSQL, ... sub-components
# but for now (just like our existing build system) this will remain a monolithic component group
set(FDO_COMPONENT "fdo-rdbms")

find_package( MySQL REQUIRED )
find_package( UnixODBC REQUIRED )

if( NOT FDO_CORE_BUILD )
    find_package( FDO REQUIRED )
endif( NOT FDO_CORE_BUILD )

include_directories( 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc 
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc/Rdbi 
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/Inc )

add_definitions( ${ODBC_DEFINITIONS} )

add_subdirectory( Nls )
add_subdirectory( Src )
add_subdirectory( Inc )
add_subdirectory( com )
add_subdirectory( Docs )

# NOTE: Doesn't include ExtendedCoordSys.txt, but that's okay as SQL Server Spatial provider is
# not currently built for Linux
install (DIRECTORY com/ 
    DESTINATION ${LIB_INSTALL_DIR}/com 
    FILES_MATCHING PATTERN "*.sql")
