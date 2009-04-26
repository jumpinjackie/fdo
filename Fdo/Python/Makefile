##
## Copyright (C) 2004-2006  Autodesk, Inc.
## 
## This library is free software; you can redistribute it and/or
## modify it under the terms of version 2.1 of the GNU Lesser
## General Public License as published by the Free Software Foundation.
## 
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
##

# Filename: Makefile
#
# Usage:    make [clean]
#
# Description: Compiles and links the Linux version of the FDO wrappers.  This script assumes
#              that the following software packages are installed:
#              - SWIG 1.3.x
#              - Python 2.4
#
#              Once the build is completed, the generated files (_FDO.so and FDO.py)
#              are copied to /usr/local/fdo-3.5.0/lib.
#
# Parameters:  clean
#              Optional input.  Deletes all object files and output files
#              install
#              Optional input.  Copies output files to /usr/local/fdo-3.5.0/lib
#              uninstall
#              Optional input.  Deletes output files from /usr/local/fdo-3.5.0/lib
#
# ------------------------------------------------------------------------------
# ATTENTION: Edit the following variables to match your environment.
# ------------------------------------------------------------------------------

# C++ compiler
CC=g++

# Target directory where the final shared library will be placed
TARGET_DIR=/usr/local/fdo-3.5.0/lib

# Location of the FDO header files.
FDO_INCLUDE_PATH=$(FDO)/Unmanaged/Inc

# Location of the FDO library files.
FDO_LIB_PATH=$(FDO)/Unmanaged/Src

# Additional Libraries
LIB = \
-L$(TARGET_DIR) \
-L$(PYTHON_LIB_PATH)/config \
-lFDO \
-lpython2.4 \
-lutil

# Include directories.  
# Intentionally include the FDO header files FIRST.
# This will prevent GCC from seeing the overriding header files in ./Inc, which
# are for Windows ONLY
INC = -I$(FDO_INCLUDE_PATH) -I$(PYTHON_INCLUDE_PATH) -I./ -I./Inc 

# Compiler and linker flags
CFLAGS = -DFULLPROTO -D__USE_GNU -DLINUX
LFLAGS = -fPIC -shared -Wl,-rpath,$(FDO_LIB_PATH)

# C/C++ Source Files
CSRC = \
Src/Common/utils.cpp \
Src/Common/FdoProxyConnectionManager.cpp \
Src/Fdo/FdoWrapper.cpp \
Src/Common/StringBuffer.cpp

CSRC-OBJ = $(notdir $(patsubst %.cpp,%.o,$(CSRC)))	

# SWIG Interface files
SWIGSRC = \
Swig/Fdo/Main.i \
Swig/Fdo/FdoTypemaps.i \
Swig/Fdo/FdoPointercasts.i \
Swig/Fdo/GeometryIncludes.i \
Swig/Fdo/CommonIncludes.i \
Swig/Fdo/FdoIncludes.i \
Swig/Common/CommonExceptions.i \
Swig/Common/CommonInit.i \
Swig/Common/CommonTypemaps.i

all: sharedlib
	@echo "Make completed."

sharedlib: objs
	@echo "Linking..."
	@$(CC) $(LFLAGS) -o _FDO.so $(CSRC-OBJ) $(LIB)

objs: swig_release $(CSRC)
	@echo "Compiling wrapper source..."
	@$(CC) $(INC) $(CFLAGS) -c $(CSRC)
	@echo "Finished compiling"

# Execute swig command, suppress known warning messages
swig_release: $(SWIGSRC)
	@echo "Generating python wrapper code with SWIG..."
	@swig -c++ -python -I./Swig -I./Inc -I./Inc_Merged -I./Inc_Merged/Common/Io -ignoremissing -nodefault -module FDO -w201 -w389 -w319 -w312 -w361 -w401 -o Src/Fdo/FdoWrapper.cpp Swig/Fdo/Main.i

# Execute swig command, and display all warning messages
swig_verbose: $(SWIGSRC)
	@echo "Generating python wrapper code with SWIG..."
	@swig -c++ -python -I./Swig -I./Inc -I./Inc_merged -I./Inc_Merged/Common/Io -ignoremissing -nodefault -module FDO -o Src/Fdo/FdoWrapper.cpp Swig/Fdo/Main.i

clean:
	@rm -f $(CRC_OBJ)
	@rm -f Src/Fdo/FDO.py
	@rm -f Src/Fdo/FdoWrapper.cpp
	@rm -f *.o
	@rm -f _FDO.so

install:
	cp Src/Fdo/FDO.py $(TARGET_DIR)
	cp _FDO.so $(TARGET_DIR)

uninstall:
	@rm -f $(TARGET_DIR)/FDO.py
	@rm -f $(TARGET_DIR)/_FDO.so

