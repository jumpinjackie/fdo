include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${SQLITE_INCLUDE_DIR}
	)

set( SQLiteInterface_STAT_SRCS
   SQLiteBTree.cpp
   SQLiteCursor.cpp
   SQLiteDataBase.cpp
   SQLiteFileDataBase.cpp
   SQLiteMemoryDataBase.cpp
   SQLiteQueryResult.cpp
   SQLiteSqlUpdateCache.cpp
   SQLiteTable.cpp
   SQLiteTransaction.cpp
   stdafx.cpp
)

add_library( SQLiteInterface STATIC ${SQLiteInterface_STAT_SRCS} )
target_link_libraries( SQLiteInterface sqlite3fdo )

