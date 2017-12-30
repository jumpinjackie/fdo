include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Inc )

set(FDO_COMPONENT "fdo-sdf")

add_subdirectory( Src )
add_subdirectory( Inc )

# Needed for unit tests
file(COPY "TestData" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})