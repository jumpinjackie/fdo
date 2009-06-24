include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

set( LTManager_STAT_SRCS
   Disposable.cpp
   LongTransactionEnvInfo.cpp
   LongTransactionConflict.cpp
   LongTransactionInfo.cpp
   LongTransactionConflictInfo.cpp
   LongTransactionInfos.cpp
   LongTransactionConflictInfos.cpp
   stdafx.cpp
   LongTransactionConflicts.cpp
)

add_library( LTManager STATIC ${LTManager_STAT_SRCS} )
