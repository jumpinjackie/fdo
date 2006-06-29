AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = \
  UnitTest.vcproj \
  Pch.cpp \
  providers.xml

noinst_PROGRAMS = UnitTest

UnitTest_SOURCES = \
  UnitTest.cpp \
  ABasicOpenTests.cpp \
  BasicConnectionTests.cpp \
  BasicSchemaTests.cpp \
  glibpatch.cpp \
  LongTransactionTests.cpp \
  ArcSDETests.cpp \
  BasicDeleteTests.cpp \
  BasicTransactionTests.cpp \
  LockTests.cpp \
  SelectTests.cpp \
  BasicArcSDETests.cpp \
  BasicInsertTests.cpp \
  BasicUpdateTests.cpp \
  LongTransactionExternalTests.cpp \
  SpatialContextTests.cpp \
  SQLTests.cpp \
  RecommitTest.cpp \
  SelectAggregatesTests.cpp \
  ThreadingTests.cpp

noinst_HEADERS = $(UnitTest_SOURCES) \
  ABasicOpenTests.h \
  ArcSDETests.h \
  BasicArcSDETests.h \
  BasicConnectionTests.h \
  BasicDeleteTests.h \
  BasicInsertTests.h \
  BasicSchemaTests.h \
  BasicTransactionTests.h \
  BasicUpdateTests.h \
  LockTests.h \
  LongTransactionExternalTests.h \
  LongTransactionTests.h \
  Pch.h \
  RecommitTest.h \
  SelectAggregatesTests.h \
  SelectTests.h \
  SpatialContextTests.h \
  SQLTests.h \
  ThreadingTests.h

INCLUDES = \
  -I../Provider \
  -I. \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOTHIRDPARTY)/ESRI/ArcSDEClient91/Linux/include \
  -I$(SDEHOME)/include \
  -I$(FDOTHIRDPARTY)/linux/cppunit/include \
  -I$(FDOUTILITIES)/Common/Inc \
  -I../../Inc


UnitTest_LDADD = \
  ../Provider/libArcSDEProviderSrc.la \
  $(FDOTHIRDPARTY)/linux/cppunit/lib/libcppunit.a \
  $(FDO)/Unmanaged/Src/libFDO.la \
  $(FDOUTILITIES)/Common/libProvidersCommon.la \
  -lpe91 \
  -lsde91 \
  -lsg91  \
  -lpthread_nonshared \
  -lpthread

UnitTest_LDFLAGS = \
  -L$(FDOTHIRDPARTY)/ESRI/ArcSDEClient91/Linux/lib \
  -L$(SDEHOME)/lib

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32 -D__STDC_LIMIT_MACROS

