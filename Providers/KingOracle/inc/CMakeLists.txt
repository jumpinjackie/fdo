include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

install(
    DIRECTORY KingOracle
    DESTINATION include
    COMPONENT ${FDO_COMPONENT}
    PATTERN *.h
    PATTERN .svn EXCLUDE
)