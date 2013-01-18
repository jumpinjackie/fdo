include_directories( BEFORE	${CMAKE_CURRENT_SOURCE_DIR}	)

set( LockManager_STAT_SRCS
	Disposable.cpp
	LockConflict.cpp
	LockInfo.cpp
	LockType.cpp
	LockTypes.cpp
	OwnerInfo.cpp
	stdafx.cpp
	TableInfo.cpp
)

add_library( LockManager STATIC ${LockManager_STAT_SRCS} )
add_dependencies( LockManager fdordbmsmsg )
