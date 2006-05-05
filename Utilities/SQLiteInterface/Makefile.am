AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libSQLiteInterface.la

libSQLiteInterface_la_SOURCES = \
  SQLiteBTree.cpp \
  SQLiteCursor.cpp \
  SQLiteDataBase.cpp \
  SQLiteFileDataBase.cpp \
  SQLiteMemoryDataBase.cpp \
  SQLiteQueryResult.cpp \
  SQLiteSqlUpdateCache.cpp \
  SQLiteTable.cpp \
  SQLiteTransaction.cpp \
  stdafx.cpp

INCLUDES = \
  -I$(FDOTHIRDPARTY)/Sqlite3.1.5/Src \
  -I$(FDOTHIRDPARTY)/ZLib/src

libSQLiteInterface_la_LDFLAGS = 

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

