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

AUTOMAKE_OPTIONS = foreign

SUBDIRS = 

EXTRA_DIST = 

noinst_LTLIBRARIES = libExpressionEngine.la

libExpressionEngine_la_SOURCES = \
  Functions/Aggregate/CacheValue.cpp \
  Functions/Aggregate/CacheValueCollection.cpp \
  Functions/Aggregate/FdoFunctionAvg.cpp \
  Functions/Aggregate/FdoFunctionCount.cpp \
  Functions/Aggregate/FdoFunctionMax.cpp \
  Functions/Aggregate/FdoFunctionMedian.cpp \
  Functions/Aggregate/FdoFunctionMin.cpp \
  Functions/Aggregate/FdoFunctionSpatialExtents.cpp \
  Functions/Aggregate/FdoFunctionStddev.cpp \
  Functions/Aggregate/FdoFunctionSum.cpp \
  Functions/Conversion/FdoFunctionNullValue.cpp \
  Functions/Conversion/FdoFunctionToDate.cpp \
  Functions/Conversion/FdoFunctionToDouble.cpp \
  Functions/Conversion/FdoFunctionToFloat.cpp \
  Functions/Conversion/FdoFunctionToInt32.cpp \
  Functions/Conversion/FdoFunctionToInt64.cpp \
  Functions/Conversion/FdoFunctionToString.cpp \
  Functions/Date/FdoFunctionAddMonths.cpp \
  Functions/Date/FdoFunctionCurrentDate.cpp \
  Functions/Date/FdoFunctionExtract.cpp \
  Functions/Date/FdoFunctionMonthsBetween.cpp \
  Functions/Geometry/FdoFunctionArea.cpp \
  Functions/Geometry/FdoFunctionGLength.cpp \
  Functions/Math/FdoFunctionAbs.cpp \
  Functions/Math/FdoFunctionAcos.cpp \
  Functions/Math/FdoFunctionAsin.cpp \
  Functions/Math/FdoFunctionAtan2.cpp \
  Functions/Math/FdoFunctionAtan.cpp \
  Functions/Math/FdoFunctionCos.cpp \
  Functions/Math/FdoFunctionExp.cpp \
  Functions/Math/FdoFunctionLn.cpp \
  Functions/Math/FdoFunctionLog.cpp \
  Functions/Math/FdoFunctionMod.cpp \
  Functions/Math/FdoFunctionPower.cpp \
  Functions/Math/FdoFunctionRemainder.cpp \
  Functions/Math/FdoFunctionSin.cpp \
  Functions/Math/FdoFunctionSqrt.cpp \
  Functions/Math/FdoFunctionTan.cpp \
  Functions/Numeric/FdoFunctionCeil.cpp \
  Functions/Numeric/FdoFunctionFloor.cpp \
  Functions/Numeric/FdoFunctionRound.cpp \
  Functions/Numeric/FdoFunctionSign.cpp \
  Functions/Numeric/FdoFunctionTrunc.cpp \
  Functions/String/FdoFunctionConcat.cpp \
  Functions/String/FdoFunctionInstr.cpp \
  Functions/String/FdoFunctionLength.cpp \
  Functions/String/FdoFunctionLower.cpp \
  Functions/String/FdoFunctionLpad.cpp \
  Functions/String/FdoFunctionLtrim.cpp \
  Functions/String/FdoFunctionRpad.cpp \
  Functions/String/FdoFunctionRtrim.cpp \
  Functions/String/FdoFunctionSoundex.cpp \
  Functions/String/FdoFunctionSubstr.cpp \
  Functions/String/FdoFunctionTranslate.cpp \
  Functions/String/FdoFunctionTrim.cpp \
  Functions/String/FdoFunctionUpper.cpp \
  Util/FdoExpressionEngineUtilDataReader.cpp \
  FdoExpressionEngine.cpp \
  FdoExpressionEngineFunctionCollection.cpp \
  FdoExpressionEngineImp.cpp \
  stdafx.cpp


INCLUDES = \
  -I$(FDOUTILITIES)/ExpressionEngine/Inc \
  -I$(FDO)/Unmanaged/Inc \
  -I$(FDO)/Unmanaged/Src/Common \
  -I$(FDOUTILITIES)/Common/Inc

libExpressionEngine_la_LDFLAGS = 

CXXFLAGS = @CXXFLAGS@ -D__USE_GNU -DLINUX -DLINUX_IA32

INSTALL_DIR  = $(libdir)

#install lib files

install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@cp -u -f libExpressionEngine.la $(INSTALL_DIR)
	@cp -u -f .libs/libExpressionEngine.a $(INSTALL_DIR)

uninstall :
	@rm -drf $(INSTALL_DIR)/libExpressionEngine.a
	@rm -drf $(INSTALL_DIR)/libExpressionEngine.la

