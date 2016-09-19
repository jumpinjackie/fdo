include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	)

install( FILES
	ExpressionEngine.h
	FdoExpressionEngineCopyFilter.h
	FdoExpressionEngineFilterProcessor.h
	FdoExpressionEngineFunctionCollection.h
	FdoExpressionEngine.h
	FdoExpressionEngineIAggregateFunction.h
	FdoExpressionEngineIFunction.h
	FdoExpressionEngineINonAggregateFunction.h 
	DESTINATION include/ExpressionEngine
	)

install ( FILES
	Util/FdoExpressionEngineUtilDataReader.h
	Util/FdoExpressionEngineUtilFeatureReader.h
	DESTINATION include/ExpressionEngine/Util
	)
