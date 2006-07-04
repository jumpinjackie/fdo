#
# Makefile - building fdogdal tests
#
# Mateusz Loskot <mateusz@loskot.net>
#
CXX = g++
CXXFLAGS = -g
#-Wall -pedantic

GDALPREFIX = ${HOME}/usr

FDOGDAL = ${FDOCORE}/Providers/GDAL
FDOGDALPREFIX = ${HOME}/usr

FDO = ${FDOCORE}/Fdo
FDOUTILITIES = ${FDOCORE}/Utilities
FDOTHIRDPARTY = ${FDOCORE}/ThirdParty
FDOCOREPREFIX = /usr/local/fdo-3.2.0

INCLUDES = \
	-I${FDOGDAL}/Inc \
	-I${FDOGDAL}/Src \
	-I$(FDO)/Unmanaged/Inc \
	-I$(FDOUTILITIES)/Common/Inc \
	-I$(FDOTHIRDPARTY)/linux/cppunit/include \
	-I${GDALPREFIX}/include

LIBDIRS = \
	-L${FDOCOREPREFIX}/lib \
	-L${FDOCORE}/Fdo/Unmanaged/Src/.libs \
	-L${FDOGDAL}/Src/.libs \
	-L${GDALPREFIX}/lib

LIBS = \
	-lFDO \
	-lGRFPOverrides \
	-lGRFPProvider \
	-lgdal \
	-lxerces-c \
	-lxalan-c \
	-lxalanMsg

# Command to make an object file:
COMPILE = $(CXX) $(CXXFLAGS) -c

all : rtest

rtest: rtest.cpp
	${CXX} rtest.cpp -o $@ ${CXXFLAGS} ${INCLUDES} ${LIBDIRS} ${LIBS}

clean :
	${RM} ~*
	${RM} *.o
	${RM} rtest
