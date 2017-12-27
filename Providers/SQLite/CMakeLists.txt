project(fdosqlite)

if( NOT FDO_CORE_BUILD )
    find_package( FDO REQUIRED )
endif( NOT FDO_CORE_BUILD )

add_subdirectory( Src )

# Needed for unit tests
file(COPY "TestData" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})