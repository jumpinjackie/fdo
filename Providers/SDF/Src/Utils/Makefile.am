AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = FDOUtils.vcproj

noinst_LTLIBRARIES = libSDFUtils.la

libSDFUtils_la_SOURCES = \
  StringValue.cpp \
  NullValue.cpp \
  DoubleValue.cpp \
  Int64Value.cpp \
  BooleanValue.cpp \
  DateTimeValue.cpp \
  DataValuePool.cpp \
  FilterExecutor.cpp \
  PropertyIndex.cpp \
  OSUtils.cpp

noinst_HEADERS = $(libSDFUtils_la_SOURCES) \
   BooleanValue.h \
   DataValue.h \
   DataValuePool.h \
   DataValueStack.h \
   DateTimeValue.h \
   DoubleValue.h \
   FilterExecutor.h \
   Int64Value.h \
   NullValue.h \
   OSUtils.h \
   PropertyIndex.h \
   stdafx.h \
   StringValue.h

libSDFUtils_la_LDFLAGS = 

INCLUDES = \
  -I./   \
  -I../../Inc \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/Src/Nls \
  -I/usr/include 

CXXFLAGS = @CXXFLAGS_SDF@ -DSQLITE -O

