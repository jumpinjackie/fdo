AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = RFPOverrides.vcproj RFPOverrides.rc resource.h 

noinst_LTLIBRARIES = libGRFPOverridesSrc.la

libGRFPOverridesSrc_la_SOURCES = \
 RFPOverrides.cpp \
 FdoGrfpClassDefinition.cpp \
 FdoGrfpPhysicalSchemaMapping.cpp \
 FdoGrfpRasterDefinition.cpp \
 FdoGrfpRasterImageDefinition.cpp \
 FdoGrfpRasterLocation.cpp \
 FdoGrfpRasterFeatureDefinition.cpp \
 FdoGrfpRasterBandDefinition.cpp \
 FdoGrfpRasterGeoreferenceLocation.cpp

noinst_HEADERS = $(libGRFPOverridesSrc_la_SOURCES) \
 FdoGrfpXmlGlobals.h \
 Overrides.h \
 stdafx.h

libGRFPOverridesSrc_la_LDFLAGS = 

INCLUDES = \
  -I../ \
  -I../../Inc \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDOUTILITIES)/Common/Inc

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32
