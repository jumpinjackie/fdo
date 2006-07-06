AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libSQLiteInterface.la

EXTRA_DIST = \
  stdafx.cpp \
  SQLiteInterface.vcproj \
  ReadMe.txt

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

noinst_HEADERS = $(libSQLiteInterface_la_SOURCES) \
  SQLiteBTree.h \
  SQLiteData.h \
  SQLiteQueryResult.h \
  SQLiteTransaction.h \
  SQLiteCursor.h\
  SQLiteFileDataBase.h \
  SQLiteSqlUpdateCache.h \
  SQLiteUpdateCache.h \
  SQLiteDataBase.h \
  SQLiteMemoryDataBase.h \
  SQLiteTable.h \
  stdafx.h


INCLUDES = \
  -I$(FDOTHIRDPARTY)/Sqlite3.1.5/Src \
  -I$(FDOTHIRDPARTY)/ZLib/src

libSQLiteInterface_la_LDFLAGS = 

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

