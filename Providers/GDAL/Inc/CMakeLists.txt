include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

install(
    DIRECTORY GdalFile
    DESTINATION include
    COMPONENT ${FDO_COMPONENT}
    PATTERN *.h
    PATTERN .svn EXCLUDE
)
