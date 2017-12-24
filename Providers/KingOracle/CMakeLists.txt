project(fdokingoracle)

find_package( Oracle REQUIRED CLNTSH OCCI OCIEI )

if( NOT FDO_CORE_BUILD )
    find_package( FDO REQUIRED )
endif( NOT FDO_CORE_BUILD )

add_subdirectory( inc )
add_subdirectory( src )