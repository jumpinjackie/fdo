AUTOMAKE_OPTIONS = foreign

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

libSDFUtils_la_LDFLAGS = 

INCLUDES = \
  -I./   \
  -I../../Inc \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/Src/Nls \
  -I/usr/include 

CXXFLAGS = @CXXFLAGS_SDF@ -DSQLITE -O

