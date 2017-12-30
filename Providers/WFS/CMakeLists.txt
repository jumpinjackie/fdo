project(fdowfs)

set(FDO_COMPONENT "fdo-wfs")

if( NOT FDO_CORE_BUILD )
    find_package( FDO REQUIRED )
endif( NOT FDO_CORE_BUILD )

if( UNIX )
    add_definitions( -DFULLPROTO -D__USE_GNU -DLINUX -fPIC )
endif( UNIX )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/Inc )

add_subdirectory( Src )
#add_subdirectory( Docs )
