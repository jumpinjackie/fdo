include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

install(
    DIRECTORY KingOracle
    DESTINATION include
    PATTERN *.h
    PATTERN .svn EXCLUDE
)