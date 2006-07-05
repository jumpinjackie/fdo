AUTOMAKE_OPTIONS = foreign

noinst_PROGRAMS = FileToHdr

EXTRA_DIST = stdafx.h stdafx.cpp

FileToHdr_SOURCES = FileToHdr.cpp

CXXFLAGS = @CXXFLAGS@ -DFULLPROTO -D__USE_GNU -DLINUX

