
include_directories (${SQLiteProvider_SOURCE_DIR}/sqlite-3.6.2 
                     ${SQLiteProvider_SOURCE_DIR}/SpatialIndex
                     ${SQLiteProvider_SOURCE_DIR}/../../Fdo/Unmanaged/Inc
                     ${SQLiteProvider_SOURCE_DIR}/../../Utilities/Common/Inc
                     ) 
link_directories (${SQLiteProvider_BINARY_DIR}/sqlite-3.6.2 
                  ${SQLiteProvider_SOURCE_DIR}/../../Fdo/Unmanaged/Src/.libs
                  ${SQLiteProvider_SOURCE_DIR}/../../Utilities/Common/.libs
                 ) 

add_library (SQLiteProvider SHARED
SltConversionUtils.cpp
slt.cpp
SltExprExtensions.cpp
SltMetadata.cpp
SltProvider.cpp
SltQueryTranslator.cpp
SltReader.cpp
SltSpatialContextReader.cpp
) 

target_link_libraries (SQLiteProvider SpatialIndex sqlite-fdo ProvidersCommon FDO ) 

INSTALL (TARGETS SQLiteProvider
         RUNTIME DESTINATION /usr/local/fdo-3.5.0/lib
         LIBRARY DESTINATION /usr/local/fdo-3.5.0/lib
         )
