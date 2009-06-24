include_directories( 
	BEFORE	
	Capability
	Connection
	Custom
	DataStore
	FeatureCommands
    Filter
    Lock
    Lt
    Other
    Pvc
    Schema
    SpatialContext
	${CMAKE_CURRENT_SOURCE_DIR} )

set( FdoRdbms_Capability_STAT_SRCS
   Capability/FdoRdbmsCommandCapabilities.cpp
   Capability/FdoRdbmsFilterCapabilities.cpp
   Capability/FdoRdbmsConnectionCapabilities.cpp
   Capability/FdoRdbmsSchemaCapabilities.cpp
   Capability/FdoRdbmsExpressionCapabilities.cpp
   Capability/FdoRdbmsTopologyCapabilities.cpp
)

set( FdoRdbms_Connection_STAT_SRCS
   Connection/DbiConnection.cpp
   Connection/FdoRdbmsConnectionPropertyDictionary.cpp
   Connection/FdoRdbmsConnection.cpp
)

set( FdoRdbms_Custom_STAT_SRCS
   Custom/FdoRdbmsCreateSpatialIndex.cpp
   Custom/FdoRdbmsGetSpatialIndexes.cpp
   Custom/FdoRdbmsDestroySpatialIndex.cpp
   Custom/FdoRdbmsSpatialIndexReader.cpp
)

set( FdoRdbms_DataStore_STAT_SRCS
   DataStore/FdoRdbmsGetDataStores.cpp
   DataStore/FdoRdbmsDataStoreReader.cpp
)

set( FdoRdbms_FeatureCommands_STAT_SRCS
   FeatureCommands/FdoRdbmsDeleteCommand.cpp
   FeatureCommands/FdoRdbmsInsertCommand.cpp
   FeatureCommands/FdoRdbmsFeatureInfoReader.cpp
   FeatureCommands/FdoRdbmsSelectAggregates.cpp
   FeatureCommands/FdoRdbmsSelectCommand.cpp
   FeatureCommands/FdoRdbmsFeatureReader.cpp
   FeatureCommands/FdoRdbmsUpdateCommand.cpp
   FeatureCommands/FdoRdbmsFeatureSubsetReader.cpp
)

set( FdoRdbms_Filter_STAT_SRCS
   Filter/FdoRdbmsFilterProcessor.cpp
   Filter/FdoRdbmsFilterUtil.cpp
)

set( FdoRdbms_Lock_STAT_SRCS
   Lock/FdoRdbmsAcquireLock.cpp
   Lock/FdoRdbmsLockedObjectsReader.cpp
   Lock/FdoRdbmsGetLockedObjects.cpp
   Lock/FdoRdbmsLockInfoReader.cpp
   Lock/FdoRdbmsGetLockInfo.cpp
   Lock/FdoRdbmsLockOwnersReader.cpp
   Lock/FdoRdbmsGetLockOwners.cpp
   Lock/FdoRdbmsReleaseLock.cpp
   Lock/FdoRdbmsLockConflictReader.cpp
   Lock/LockUtility.cpp
   Lock/FdoRdbmsLockConflicts.cpp
)

set( FdoRdbms_Lt_STAT_SRCS
   Lt/FdoRdbmsActivateLongTransaction.cpp
   Lt/FdoRdbmsCommitLongTransaction.cpp
   Lt/FdoRdbmsCreateLongTransaction.cpp
   Lt/FdoRdbmsDeactivateLongTransaction.cpp
   Lt/FdoRdbmsGetLongTransactions.cpp
   Lt/FdoRdbmsLongTransactionConflictDirectiveEnumerator.cpp
   Lt/FdoRdbmsLongTransactionReader.cpp
   Lt/FdoRdbmsRollbackLongTransaction.cpp
)

set( FdoRdbms_Other_STAT_SRCS
   Other/FdoRdbmsBLOBStreamReader.cpp
   Other/FdoRdbmsPrimaryKey.cpp
   Other/FdoRdbmsException.cpp
   Other/FdoRdbmsSQLCommand.cpp
   Other/FdoRdbmsFeatureTransaction.cpp
   Other/FdoRdbmsSQLDataReader.cpp
   Other/FdoRdbmsLobUtility.cpp
   Other/FdoRdbmsUtil.cpp
   Other/FdoRdbmsPrimaryKeyColumn.cpp
)

set( FdoRdbms_Pvc_STAT_SRCS
   Pvc/FdoRdbmsPvcHandler.cpp
   Pvc/FdoRdbmsPvcInsertHandler.cpp
   Pvc/FdoRdbmsPvcProcessor.cpp
   Pvc/FdoRdbmsPvcOperation.cpp
   Pvc/FdoRdbmsPvcUpdateHandler.cpp
)

set( FdoRdbms_Schema_STAT_SRCS
   Schema/FdoRdbmsApplySchemaCommand.cpp
   Schema/FdoRdbmsDestroySchemaCommand.cpp
   Schema/FdoRdbmsDescribeSchemaCommand.cpp
   Schema/FdoRdbmsGetClassNamesCommand.cpp
   Schema/FdoRdbmsGetSchemaNamesCommand.cpp
   Schema/FdoRdbmsSchemaUtil.cpp
   Schema/FdoRdbmsDescribeSchemaMappingCommand.cpp
)

set( FdoRdbms_SpatialContext_STAT_SRCS
   SpatialContext/FdoRdbmsActivateSpatialContext.cpp
   SpatialContext/FdoRdbmsGetSpatialContexts.cpp
   SpatialContext/FdoRdbmsCreateSpatialContext.cpp
   SpatialContext/FdoRdbmsSpatialContextReader.cpp
   SpatialContext/FdoRdbmsDestroySpatialContext.cpp
)

set( FdoRdbms_STAT_SRCS
   ${FdoRdbms_Capability_STAT_SRCS}
   ${FdoRdbms_Connection_STAT_SRCS}
   ${FdoRdbms_Custom_STAT_SRCS}
   ${FdoRdbms_DataStore_STAT_SRCS}
   ${FdoRdbms_FeatureCommands_STAT_SRCS}
   ${FdoRdbms_Filter_STAT_SRCS}
   ${FdoRdbms_Lock_STAT_SRCS}
   ${FdoRdbms_Lt_STAT_SRCS}
   ${FdoRdbms_Other_STAT_SRCS}
   ${FdoRdbms_Pvc_STAT_SRCS}
   ${FdoRdbms_Schema_STAT_SRCS}
   ${FdoRdbms_SpatialContext_STAT_SRCS}
)

add_library( FdoRdbms STATIC ${FdoRdbms_STAT_SRCS} )

