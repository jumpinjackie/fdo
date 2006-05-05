AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libFdoRdbms_Lt.la

libFdoRdbms_Lt_la_SOURCES = \
  FdoRdbmsActivateLongTransaction.cpp \
  FdoRdbmsCommitLongTransaction.cpp \
  FdoRdbmsCreateLongTransaction.cpp \
  FdoRdbmsDeactivateLongTransaction.cpp \
  FdoRdbmsGetLongTransactions.cpp \
  FdoRdbmsLongTransactionConflictDirectiveEnumerator.cpp \
  FdoRdbmsLongTransactionReader.cpp \
  FdoRdbmsRollbackLongTransaction.cpp


INCLUDES = \
  -I../ \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged \
  -I../../../../GenericRdbms/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I$(FDOUTILITIES)/SchemaMgr/Inc/Sm/Lp \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I../../../Inc \
  -I../../../Inc/Rdbi \
  -I../../ \
  -I../../../ \
  -I../../../Inc \
  -I.././Other \
  -I.././Capability \
  -I.././Connection \
  -I.././Custom \
  -I.././FeatureCommands \
  -I.././Filter \
  -I.././Lock \
  -I.././Lt \
  -I.././Schema \
  -I.././SpatialContext \
  -I.././Topo \
  -I$(FDO)/Unmanaged \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/src/geometry \
  -I$(FDOUTILITIES)/SchemaMgr/Inc \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../Gdbi \
  -I../../LongTransactionManager \
  -I../../LockManager

libFdoRdbms_Lt_la_LDFLAGS =

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

