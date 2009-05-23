# extra macros 
include(MacroOptionalAddSubdirectory)

# Set a core build flag to enable Providers know that building are 
# done inside main core source, and not standalone
set( FDO_CORE_BUILD TRUE )

# Global definitions ( used by all sources )
# Check if is a 64 bits install and use user defined LIB_SUFFIX
if( UNIX )
	set( LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)" )
	include( CheckTypeSize )
	check_type_size( "void*" VOID_SIZE )
	if( VOID_SIZE EQUAL 8 )
		message( STATUS "Compiling in 64 bits machine" )
	endif( VOID_SIZE EQUAL 8 )
	set( LIB_INSTALL_DIR "lib${LIB_SUFFIX}" )
endif( UNIX )

# Define the instalation prefix 
if( UNIX )
	if( INSTALL_PREFIX )
		set( CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX} )
	else( INSTALL_PREFIX )
		set( CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/fdo-${FDO_VERSION} )
	endif( INSTALL_PREFIX )
endif( UNIX )

# Make global include directories, even binary build dir 
# to match build generated files
set( UNMANAGED_INCLUDE_DIRS 
	${FDO}/Unmanaged/Inc 
	${FDO}/Unmanaged/Src/Common
	# Needed for Messages
	${CMAKE_CURRENT_BINARY_DIR}/Fdo/Unmanaged/Inc 
)
mark_as_advanced( UNMANAGED_INCLUDE_DIRS )
