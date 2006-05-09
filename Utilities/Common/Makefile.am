AUTOMAKE_OPTIONS = foreign

noinst_LTLIBRARIES = libProvidersCommon.la

libProvidersCommon_la_SOURCES = \
  Src/FdoCommonNlsUtil.cpp \
  Src/FdoCommonOSUtil.cpp \
  Src/FdoCommonSchemaUtil.cpp \
  Src/FdoCommonStringUtil.cpp \
  Src/FdoCommonMiscUtil.cpp \
  Src/FdoCommonFile.cpp \
  Src/FdoCommonFilterExecutor.cpp \
  Src/FdoCommonConnPropDictionary.cpp \
  Src/FdoCommonConnStringParser.cpp \
  Src/FdoCommonBinaryReader.cpp \
  Src/FdoCommonBinaryWriter.cpp \
  Src/FdoCommonPropertyIndex.cpp \
  Src/FdoCommonQueryAggregator.cpp \
  Src/FdoCommonDataReader.cpp \
  Src/FdoCommonSelectAggregatesCommand.cpp \
  Src/FdoCommonThreadMutex.cpp \
  Src/FdoCommonThreadUtil.cpp \
  Src/stdafx.cpp \
  Src/Parse/Lex.cpp \
  Src/Parse/Parse.cpp \
  Src/Parse/yyConstraint.cpp

INCLUDES = \
  -I$(FDO)/Unmanaged/Inc \
  -I./Inc \
  -I$(FDO)/Unmanaged/Src/Common

libProvidersCommon_la_LDFLAGS = 

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -pthread

