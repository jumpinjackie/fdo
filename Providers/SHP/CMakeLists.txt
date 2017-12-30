project(fdoshp)

set(FDO_COMPONENT "fdo-shp")

if( NOT FDO_CORE_BUILD )
    find_package( FDO REQUIRED )
endif( NOT FDO_CORE_BUILD )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Inc )

add_subdirectory( Src )
add_subdirectory( Inc )

# Needed for unit tests
file(COPY "TestData" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})