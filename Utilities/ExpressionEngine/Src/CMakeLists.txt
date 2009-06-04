include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${UTILITIES_EXPRESSION_INCLUDE_DIR}
    ${UNMANAGED_INCLUDE_DIR}
	${UNMANAGED_COMMON_INCLUDE_DIR}
	${UTILITIES_COMMON_INCLUDE_DIR}
	)

set( ExpressionEngine_LIB_SRCS
   Functions/Aggregate/CacheValue.cpp
   Functions/Aggregate/CacheValueCollection.cpp
   Functions/Aggregate/FdoFunctionAvg.cpp
   Functions/Aggregate/FdoFunctionCount.cpp
   Functions/Aggregate/FdoFunctionMax.cpp
   Functions/Aggregate/FdoFunctionMedian.cpp
   Functions/Aggregate/FdoFunctionMin.cpp
   Functions/Aggregate/FdoFunctionSpatialExtents.cpp
   Functions/Aggregate/FdoFunctionStddev.cpp
   Functions/Aggregate/FdoFunctionSum.cpp
   Functions/Conversion/FdoFunctionNullValue.cpp
   Functions/Conversion/FdoFunctionToDate.cpp
   Functions/Conversion/FdoFunctionToDouble.cpp
   Functions/Conversion/FdoFunctionToFloat.cpp
   Functions/Conversion/FdoFunctionToInt32.cpp
   Functions/Conversion/FdoFunctionToInt64.cpp
   Functions/Conversion/FdoFunctionToString.cpp
   Functions/Date/FdoFunctionAddMonths.cpp
   Functions/Date/FdoFunctionCurrentDate.cpp
   Functions/Date/FdoFunctionExtract.cpp
   Functions/Date/FdoFunctionExtractToDouble.cpp
   Functions/Date/FdoFunctionExtractToInt.cpp
   Functions/Date/FdoFunctionMonthsBetween.cpp
   Functions/Geometry/FdoFunctionArea2D.cpp
   Functions/Geometry/FdoFunctionLength2D.cpp
   Functions/Geometry/FdoFunctionX.cpp
   Functions/Geometry/FdoFunctionY.cpp
   Functions/Geometry/FdoFunctionZ.cpp
   Functions/Geometry/FdoFunctionM.cpp
   Functions/Geometry/Util.cpp
   Functions/Math/FdoFunctionAbs.cpp
   Functions/Math/FdoFunctionAcos.cpp
   Functions/Math/FdoFunctionAsin.cpp
   Functions/Math/FdoFunctionAtan2.cpp
   Functions/Math/FdoFunctionAtan.cpp
   Functions/Math/FdoFunctionCos.cpp
   Functions/Math/FdoFunctionExp.cpp
   Functions/Math/FdoFunctionLn.cpp
   Functions/Math/FdoFunctionLog.cpp
   Functions/Math/FdoFunctionMod.cpp
   Functions/Math/FdoFunctionPower.cpp
   Functions/Math/FdoFunctionRemainder.cpp
   Functions/Math/FdoFunctionSin.cpp
   Functions/Math/FdoFunctionSqrt.cpp
   Functions/Math/FdoFunctionTan.cpp
   Functions/Numeric/FdoFunctionCeil.cpp
   Functions/Numeric/FdoFunctionFloor.cpp
   Functions/Numeric/FdoFunctionRound.cpp
   Functions/Numeric/FdoFunctionSign.cpp
   Functions/Numeric/FdoFunctionTrunc.cpp
   Functions/String/FdoFunctionConcat.cpp
   Functions/String/FdoFunctionInstr.cpp
   Functions/String/FdoFunctionLength.cpp
   Functions/String/FdoFunctionLower.cpp
   Functions/String/FdoFunctionLpad.cpp
   Functions/String/FdoFunctionLtrim.cpp
   Functions/String/FdoFunctionRpad.cpp
   Functions/String/FdoFunctionRtrim.cpp
   Functions/String/FdoFunctionSoundex.cpp
   Functions/String/FdoFunctionSubstr.cpp
   Functions/String/FdoFunctionTranslate.cpp
   Functions/String/FdoFunctionTrim.cpp
   Functions/String/FdoFunctionUpper.cpp
   Util/FdoExpressionEngineUtilDataReader.cpp
   Util/FdoExpressionEngineUtilFeatureReader.cpp
   FdoExpressionEngine.cpp
   FdoExpressionEngineFunctionCollection.cpp
   FdoExpressionEngineImp.cpp
   FdoExpressionEngineCopyFilter.cpp
   stdafx.cpp
)

add_library( ExpressionEngine SHARED ${ExpressionEngine_LIB_SRCS} )

target_link_libraries( ExpressionEngine
	FDO
	ProvidersCommon
 )

set_target_properties( ExpressionEngine PROPERTIES VERSION ${FDO_VERSION} SOVERSION ${FDO_VERSION_MAJOR} )
install( TARGETS ExpressionEngine DESTINATION ${LIB_INSTALL_DIR} )
