include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbms_Lt_STAT_SRCS
   FdoRdbmsActivateLongTransaction.cpp
   FdoRdbmsCommitLongTransaction.cpp
   FdoRdbmsCreateLongTransaction.cpp
   FdoRdbmsDeactivateLongTransaction.cpp
   FdoRdbmsGetLongTransactions.cpp
   FdoRdbmsLongTransactionConflictDirectiveEnumerator.cpp
   FdoRdbmsLongTransactionReader.cpp
   FdoRdbmsRollbackLongTransaction.cpp
)

add_library( FdoRdbms_Lt STATIC ${FdoRdbms_Lt_STAT_SRCS} )
