
include_directories (${SQLiteProvider_SOURCE_DIR}/sqlite-3.6.2 
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
SltGeomUtils.cpp
SltMetadata.cpp
SltProvider.cpp
SltQueryTranslator.cpp
SltReader.cpp
SltSpatialContextReader.cpp
SpatialIndex.cpp
) 

target_link_libraries (SQLiteProvider sqlite-fdo ProvidersCommon FDO ) 
