include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbms_Lock_STAT_SRCS
   FdoRdbmsAcquireLock.cpp
   FdoRdbmsLockedObjectsReader.cpp
   FdoRdbmsGetLockedObjects.cpp
   FdoRdbmsLockInfoReader.cpp
   FdoRdbmsGetLockInfo.cpp
   FdoRdbmsLockOwnersReader.cpp
   FdoRdbmsGetLockOwners.cpp
   FdoRdbmsReleaseLock.cpp
   FdoRdbmsLockConflictReader.cpp
   LockUtility.cpp
   FdoRdbmsLockConflicts.cpp
)

add_library( FdoRdbms_Lock STATIC ${FdoRdbms_Lock_STAT_SRCS} )
