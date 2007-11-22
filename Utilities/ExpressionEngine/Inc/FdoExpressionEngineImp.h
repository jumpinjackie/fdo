// 
//  Copyright (C) 2004-2007  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#ifndef FDOEXPRESSIONENGINEIMP_H
#define FDOEXPRESSIONENGINEIMP_H

#include <ExpressionEngine.h>
#include <vector>
#include <FdoExpressionEngineFunctionCollection.h>
#include <Fdo/Connections/Capabilities/FunctionDefinition.h>
#include <FdoCommonPropertyIndex.h>
#include <FdoExpressionEngineIAggregateFunction.h>
#include <FdoExpressionEngineINonAggregateFunction.h>

typedef std::vector<FdoLiteralValue*> retval_stack;

struct FunctionCache
{
	FdoFunction *address;                               // used to store the address
    bool        isAggregate;                            // indicate whether the function is an aggregate function
	FdoExpressionEngineINonAggregateFunction *function;  // only set if the function is a non aggregate function
};

class FdoExpressionEngineImp : public FdoIFilterProcessor, public FdoIExpressionProcessor
{

public:
	EXPRESSIONENGINE_API static FdoExpressionEngineImp* Create(FdoIReader* reader, FdoClassDefinition* classDef, FdoIdentifierCollection* identifiers,
		FdoExpressionEngineFunctionCollection *userDefinedFunctions);

	EXPRESSIONENGINE_API virtual FdoLiteralValue* Evaluate(FdoExpression *expression);
    EXPRESSIONENGINE_API virtual FdoLiteralValue* Evaluate(FdoString* name);
    EXPRESSIONENGINE_API virtual FdoLiteralValue* Evaluate(FdoIdentifier& expr);

	EXPRESSIONENGINE_API FdoPropertyValueCollection* RunQuery();
    EXPRESSIONENGINE_API bool ProcessFilter(FdoFilter *filter);

    EXPRESSIONENGINE_API FdoFunctionDefinitionCollection *GetAllFunctions();

	// This is the default collection of functions that the expression engine supports
	EXPRESSIONENGINE_API static FdoFunctionDefinitionCollection *GetStandardFunctions();

	// The following are some utility functions
	EXPRESSIONENGINE_API static void ValidateFilter( FdoClassDefinition *cls, FdoFilter *filter, FdoIdentifierCollection *selIds = NULL, FdoIFilterCapabilities *filterCapabilities = NULL);
	EXPRESSIONENGINE_API static FdoFilter* OptimizeFilter( FdoFilter *filter );
   	EXPRESSIONENGINE_API static bool IsAggregateFunction(FdoFunctionDefinitionCollection *funcDefs, FdoString *name);
	EXPRESSIONENGINE_API static void GetExpressionType(FdoFunctionDefinitionCollection *functionDefinitions, FdoClassDefinition* originalClassDef, FdoExpression *expr, FdoPropertyType &retPropType, FdoDataType &retDataType);
	EXPRESSIONENGINE_API static void GetExpressionType(FdoClassDefinition* originalClassDef, FdoExpression *expr, FdoPropertyType &retPropType, FdoDataType &retDataType);

public:
    EXPRESSIONENGINE_API FdoPropertyType GetResultPropertyType ();
    EXPRESSIONENGINE_API FdoDataType GetResultDataType ();
    EXPRESSIONENGINE_API bool IsResultNull ();
    EXPRESSIONENGINE_API bool GetBooleanResult (bool &bIsNull);
    EXPRESSIONENGINE_API double GetDoubleResult (bool &bIsNull);
    EXPRESSIONENGINE_API float GetSingleResult (bool &bIsNull);
    EXPRESSIONENGINE_API double GetDecimalResult (bool &bIsNull);
    EXPRESSIONENGINE_API FdoByte GetByteResult (bool &bIsNull);
    EXPRESSIONENGINE_API FdoInt16 GetInt16Result (bool &bIsNull);
    EXPRESSIONENGINE_API FdoInt32 GetInt32Result (bool &bIsNull);
    EXPRESSIONENGINE_API FdoInt64 GetInt64Result (bool &bIsNull);
    EXPRESSIONENGINE_API const wchar_t* GetStringResult (bool &bIsNull);
    EXPRESSIONENGINE_API FdoDateTime GetDateTimeResult (bool &bIsNull);
    EXPRESSIONENGINE_API FdoGeometryValue* GetGeometricResult (bool &bIsNull);

	EXPRESSIONENGINE_API void Reset();


	EXPRESSIONENGINE_API virtual FdoInt32 AddRef ()
    {
    /// NOTE: due to multiple inheritance, there is an ambiguity in which AddRef() method to call.
    ///  Calling BOTH AddRef() methods leads to instances of this class being prematurely released.
        return FdoIFilterProcessor::AddRef ();
    }

    /// \brief
    /// Decrease the reference count.
    /// 
    /// \return
    /// Returns the new reference count (value for debugging use only).
    /// 
    EXPRESSIONENGINE_API virtual FdoInt32 Release ()
    {
    /// NOTE: due to multiple inheritance, there is an ambiguity in which Release() method to call.
    ///  Calling BOTH Release() methods leads to instances of this class being prematurely released.
        return FdoIFilterProcessor::Release ();
    }

public:
    //
    /// FdoIFilterProcessor interface
    //

    EXPRESSIONENGINE_API void ProcessBinaryLogicalOperator (FdoBinaryLogicalOperator& filter);
    EXPRESSIONENGINE_API void ProcessUnaryLogicalOperator (FdoUnaryLogicalOperator& filter);
    EXPRESSIONENGINE_API void ProcessComparisonCondition (FdoComparisonCondition& filter);
    EXPRESSIONENGINE_API void ProcessInCondition (FdoInCondition& filter);
    EXPRESSIONENGINE_API void ProcessNullCondition (FdoNullCondition& filter);
    EXPRESSIONENGINE_API void ProcessSpatialCondition (FdoSpatialCondition& filter);
    EXPRESSIONENGINE_API void ProcessDistanceCondition (FdoDistanceCondition& filter);


    //
    /// FdoIExpressionProcessor interface
    //

    EXPRESSIONENGINE_API void ProcessBinaryExpression (FdoBinaryExpression& expr);
    EXPRESSIONENGINE_API void ProcessUnaryExpression (FdoUnaryExpression& expr);
    EXPRESSIONENGINE_API void ProcessFunction (FdoFunction& expr);
    EXPRESSIONENGINE_API void ProcessIdentifier (FdoIdentifier& expr);
	EXPRESSIONENGINE_API void ProcessComputedIdentifier (FdoComputedIdentifier& expr);
    EXPRESSIONENGINE_API void ProcessParameter (FdoParameter& expr);
    EXPRESSIONENGINE_API void ProcessBooleanValue (FdoBooleanValue& expr);
    EXPRESSIONENGINE_API void ProcessByteValue (FdoByteValue& expr);
    EXPRESSIONENGINE_API void ProcessDateTimeValue (FdoDateTimeValue& expr);
    EXPRESSIONENGINE_API void ProcessDecimalValue (FdoDecimalValue& expr);
    EXPRESSIONENGINE_API void ProcessDoubleValue (FdoDoubleValue& expr);
    EXPRESSIONENGINE_API void ProcessInt16Value (FdoInt16Value& expr);
    EXPRESSIONENGINE_API void ProcessInt32Value (FdoInt32Value& expr);
    EXPRESSIONENGINE_API void ProcessInt64Value (FdoInt64Value& expr);
    EXPRESSIONENGINE_API void ProcessSingleValue (FdoSingleValue& expr);
    EXPRESSIONENGINE_API void ProcessStringValue (FdoStringValue& expr);
    EXPRESSIONENGINE_API void ProcessBLOBValue (FdoBLOBValue& expr);
    EXPRESSIONENGINE_API void ProcessCLOBValue (FdoCLOBValue& expr);
    EXPRESSIONENGINE_API void ProcessGeometryValue (FdoGeometryValue& expr);

protected:
	
	EXPRESSIONENGINE_API FdoExpressionEngineImp(FdoIReader* Reader, FdoClassDefinition* classDef, FdoIdentifierCollection* compIdents,
		FdoExpressionEngineFunctionCollection *userDefinedFunctions);
    EXPRESSIONENGINE_API FdoExpressionEngineImp();
	EXPRESSIONENGINE_API ~FdoExpressionEngineImp();
	EXPRESSIONENGINE_API virtual void Dispose ();

    //generic identifier processing, including computed and non-computed
    //identifiers
	EXPRESSIONENGINE_API virtual void ProcessIdentifier (FdoString* name);

    //
    /// allocating and pooling data values
    //

    EXPRESSIONENGINE_API virtual FdoBooleanValue* ObtainBooleanValue (bool bIsNull, bool value); 
    EXPRESSIONENGINE_API virtual FdoByteValue* ObtainByteValue (bool bIsNull, FdoByte value); 
    EXPRESSIONENGINE_API virtual FdoDateTimeValue* ObtainDateTimeValue (bool bIsNull, FdoDateTime value); 
    EXPRESSIONENGINE_API virtual FdoDecimalValue* ObtainDecimalValue (bool bIsNull, double value); 
    EXPRESSIONENGINE_API virtual FdoDoubleValue* ObtainDoubleValue (bool bIsNull, double value); 
    EXPRESSIONENGINE_API virtual FdoInt16Value* ObtainInt16Value (bool bIsNull, FdoInt16 value); 
    EXPRESSIONENGINE_API virtual FdoInt32Value* ObtainInt32Value (bool bIsNull, FdoInt32 value); 
    EXPRESSIONENGINE_API virtual FdoInt64Value* ObtainInt64Value (bool bIsNull, FdoInt64 value); 
    EXPRESSIONENGINE_API virtual FdoSingleValue* ObtainSingleValue (bool bIsNull, float value); 
    EXPRESSIONENGINE_API virtual FdoStringValue* ObtainStringValue (bool bIsNull, FdoString* value); 
    EXPRESSIONENGINE_API virtual FdoBLOBValue* ObtainBLOBValue (bool bIsNull, FdoByteArray* value); 
    EXPRESSIONENGINE_API virtual FdoCLOBValue* ObtainCLOBValue (bool bIsNull, FdoByteArray* value);
    EXPRESSIONENGINE_API virtual FdoLiteralValueCollection* ObtainLiteralValueCollection();
    EXPRESSIONENGINE_API virtual FdoGeometryValue* ObtainGeometryValue (bool bIsNull, FdoByteArray* value);
    EXPRESSIONENGINE_API virtual void RelinquishDataValue (FdoLiteralValue* data);
    EXPRESSIONENGINE_API virtual void RelinquishLiteralValueCollection(FdoLiteralValueCollection* literals);


private:
    void EvaluateAggregateExpression();
    void ProcessAggregateFunctions();
	FdoPropertyDefinition* GetProperty(FdoClassDefinition* cls, FdoString* propName );
    static FdoFunctionDefinitionCollection *DeepCopyFunctionDefinitions(FdoExpressionEngineFunctionCollection *functions);
    static FdoFunctionDefinition *DeepCopyFunctionDefinition(FdoFunctionDefinition *function);




	FdoCommonPropertyStub* GetPropInfo(FdoString* name);

    //
    /// processing
    //
    bool Like (FdoDataValue* argLeft, FdoDataValue* argRight);
    FdoDataValue* Add (FdoDataValue* argLeft, FdoDataValue* argRight);
    FdoDataValue* Subtract (FdoDataValue* argLeft, FdoDataValue* argRight);
    FdoDataValue* Multiply (FdoDataValue* argLeft, FdoDataValue* argRight);
    FdoDataValue* Divide (FdoDataValue* argLeft, FdoDataValue* argRight);
    FdoDataValue* Negate (FdoDataValue* argLeft);

    /// pattern matching
    bool MatchesHere (const wchar_t* pattern, const wchar_t* src);
    bool MatchPercent (const wchar_t* pattern, const wchar_t* src);
    bool MatchBracket (const wchar_t* pattern, const wchar_t* src);

    bool AddToCache(FdoString *functionName, FdoExpressionEngineIFunction *functionExtension, FdoFunction& expr, bool* isAggregate);

	void PushIdentifierValue(FdoIReader* reader, FdoString* name, FdoDataType type );
    void PushLiteralValue(FdoLiteralValue *literal );


protected:
    FdoIReader* m_reader;   // NOTE: weak reference
	// execution stack
    retval_stack m_retvals;

private:
    FdoPtr< FdoArray<FdoFunction*> > mAggrIdents;

    /// list of computed identifiers
    FdoPtr<FdoIdentifierCollection> m_compIdents;

    /// pools of data values
    std::vector<FdoBooleanValue*> mBooleanPool;
    std::vector<FdoByteValue*> mBytePool;
    std::vector<FdoDateTimeValue*> mDateTimePool;
    std::vector<FdoDecimalValue*> mDecimalPool;
    std::vector<FdoDoubleValue*> mDoublePool;
    std::vector<FdoInt16Value*> mInt16Pool;
    std::vector<FdoInt32Value*> mInt32Pool;
    std::vector<FdoInt64Value*> mInt64Pool;
    std::vector<FdoSingleValue*> mSinglePool;
    std::vector<FdoStringValue*> mStringPool;
    std::vector<FdoBLOBValue*> mBLOBPool;
    std::vector<FdoCLOBValue*> mCLOBPool;
    std::vector<FdoLiteralValueCollection*> mLiteralValueCollectionPool;

	FdoPtr<FdoClassDefinition> m_classDefinition;

	FdoCommonPropertyIndex* m_propIndex;

	FdoPtr<FdoExpressionEngineFunctionCollection> m_UserDefinedFunctions; // the user-defined functions to be supported
	std::vector<FdoExpressionEngineIAggregateFunction *> m_AggregateFunctions;   // the aggregate functions. If selecting
                                                                                // min(property), max(property2), min(property3) the first item would be the min object,
                                                                                // second item would be a max object and the last item would be another max object 
	int m_CurrentIndex; // specifies which item of the aggregate object
 
	bool m_processingAggregate; // true if processing an aggregate function, false if retrieving the results of an aggregate function
    bool m_dataRead; // true if a row was read, otherwise false

    // Cache of the functions 
    // If selecting the following, func1(property1), func2(property3), func1(property1), eventually the cache
    // will be populate from three items. Searching of the cache is comparing addresses.
	int m_Size;             // number of items allocated in the cache
	int m_Current;          // number of items in the cache
	FunctionCache *m_CacheFunc; //the cache functions

    FdoPtr<FdoFunctionDefinitionCollection> m_AllFunctions; // collection of user-defined functions + standard functions

};

#endif
