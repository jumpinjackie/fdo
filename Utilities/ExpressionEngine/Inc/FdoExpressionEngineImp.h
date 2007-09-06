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
	static FdoExpressionEngineImp* Create(FdoIReader* reader, FdoClassDefinition* classDef, FdoIdentifierCollection* identifiers,
		FdoExpressionEngineFunctionCollection *userDefinedFunctions);

	virtual FdoLiteralValue* Evaluate(FdoExpression *expression);
    virtual FdoLiteralValue* Evaluate(FdoString* name);
    virtual FdoLiteralValue* Evaluate(FdoIdentifier& expr);

	FdoPropertyValueCollection* RunQuery();
    bool ProcessFilter(FdoFilter *filter);

    FdoFunctionDefinitionCollection *GetAllFunctions();

	// This is the default collection of functions that the expression engine supports
	static FdoFunctionDefinitionCollection *GetStandardFunctions();

	// The following are some utility functions
	static void ValidateFilter( FdoClassDefinition *cls, FdoFilter *filter, FdoIdentifierCollection *selIds = NULL, FdoIFilterCapabilities *filterCapabilities = NULL);
	static FdoFilter* OptimizeFilter( FdoFilter *filter );
   	static bool IsAggregateFunction(FdoFunctionDefinitionCollection *funcDefs, FdoString *name);

public:
    FdoPropertyType GetResultPropertyType ();
    FdoDataType GetResultDataType ();
    bool IsResultNull ();
    bool GetBooleanResult (bool &bIsNull);
    double GetDoubleResult (bool &bIsNull);
    float GetSingleResult (bool &bIsNull);
    double GetDecimalResult (bool &bIsNull);
    FdoByte GetByteResult (bool &bIsNull);
    FdoInt16 GetInt16Result (bool &bIsNull);
    FdoInt32 GetInt32Result (bool &bIsNull);
    FdoInt64 GetInt64Result (bool &bIsNull);
    const wchar_t* GetStringResult (bool &bIsNull);
    FdoDateTime GetDateTimeResult (bool &bIsNull);
    FdoGeometryValue* GetGeometricResult (bool &bIsNull);

	void Reset();


	virtual FdoInt32 AddRef ()
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
    virtual FdoInt32 Release ()
    {
    /// NOTE: due to multiple inheritance, there is an ambiguity in which Release() method to call.
    ///  Calling BOTH Release() methods leads to instances of this class being prematurely released.
        return FdoIFilterProcessor::Release ();
    }

public:
    //
    /// FdoIFilterProcessor interface
    //

    void ProcessBinaryLogicalOperator (FdoBinaryLogicalOperator& filter);
    void ProcessUnaryLogicalOperator (FdoUnaryLogicalOperator& filter);
    void ProcessComparisonCondition (FdoComparisonCondition& filter);
    void ProcessInCondition (FdoInCondition& filter);
    void ProcessNullCondition (FdoNullCondition& filter);
    void ProcessSpatialCondition (FdoSpatialCondition& filter);
    void ProcessDistanceCondition (FdoDistanceCondition& filter);


    //
    /// FdoIExpressionProcessor interface
    //

    void ProcessBinaryExpression (FdoBinaryExpression& expr);
    void ProcessUnaryExpression (FdoUnaryExpression& expr);
    void ProcessFunction (FdoFunction& expr);
    void ProcessIdentifier (FdoIdentifier& expr);
	void ProcessComputedIdentifier (FdoComputedIdentifier& expr);
    void ProcessParameter (FdoParameter& expr);
    void ProcessBooleanValue (FdoBooleanValue& expr);
    void ProcessByteValue (FdoByteValue& expr);
    void ProcessDateTimeValue (FdoDateTimeValue& expr);
    void ProcessDecimalValue (FdoDecimalValue& expr);
    void ProcessDoubleValue (FdoDoubleValue& expr);
    void ProcessInt16Value (FdoInt16Value& expr);
    void ProcessInt32Value (FdoInt32Value& expr);
    void ProcessInt64Value (FdoInt64Value& expr);
    void ProcessSingleValue (FdoSingleValue& expr);
    void ProcessStringValue (FdoStringValue& expr);
    void ProcessBLOBValue (FdoBLOBValue& expr);
    void ProcessCLOBValue (FdoCLOBValue& expr);
    void ProcessGeometryValue (FdoGeometryValue& expr);

protected:
	
	FdoExpressionEngineImp(FdoIReader* Reader, FdoClassDefinition* classDef, FdoIdentifierCollection* compIdents,
		FdoExpressionEngineFunctionCollection *userDefinedFunctions);
    FdoExpressionEngineImp();
	~FdoExpressionEngineImp();
	virtual void Dispose ();

    //generic identifier processing, including computed and non-computed
    //identifiers
	virtual void ProcessIdentifier (FdoString* name);

    //
    /// allocating and pooling data values
    //

    virtual FdoBooleanValue* ObtainBooleanValue (bool bIsNull, bool value); 
    virtual FdoByteValue* ObtainByteValue (bool bIsNull, FdoByte value); 
    virtual FdoDateTimeValue* ObtainDateTimeValue (bool bIsNull, FdoDateTime value); 
    virtual FdoDecimalValue* ObtainDecimalValue (bool bIsNull, double value); 
    virtual FdoDoubleValue* ObtainDoubleValue (bool bIsNull, double value); 
    virtual FdoInt16Value* ObtainInt16Value (bool bIsNull, FdoInt16 value); 
    virtual FdoInt32Value* ObtainInt32Value (bool bIsNull, FdoInt32 value); 
    virtual FdoInt64Value* ObtainInt64Value (bool bIsNull, FdoInt64 value); 
    virtual FdoSingleValue* ObtainSingleValue (bool bIsNull, float value); 
    virtual FdoStringValue* ObtainStringValue (bool bIsNull, FdoString* value); 
    virtual FdoBLOBValue* ObtainBLOBValue (bool bIsNull, FdoByteArray* value); 
    virtual FdoCLOBValue* ObtainCLOBValue (bool bIsNull, FdoByteArray* value);
    virtual FdoGeometryValue* ObtainGeometryValue (bool bIsNull, FdoByteArray* value);
    virtual void RelinquishDataValue (FdoLiteralValue* data);

private:
    void EvaluateAggregateExpression();
    void ProcessAggregateFunctions();
	FdoPropertyDefinition* GetProperty(FdoClassDefinition* cls, FdoString* propName );


	FdoCommonPropertyStub* GetPropInfo(FdoString* name);

    //
    /// processing
    //
    virtual bool Like (FdoDataValue* argLeft, FdoDataValue* argRight);
    virtual FdoDataValue* Add (FdoDataValue* argLeft, FdoDataValue* argRight);
    virtual FdoDataValue* Subtract (FdoDataValue* argLeft, FdoDataValue* argRight);
    virtual FdoDataValue* Multiply (FdoDataValue* argLeft, FdoDataValue* argRight);
    virtual FdoDataValue* Divide (FdoDataValue* argLeft, FdoDataValue* argRight);
    virtual FdoDataValue* Negate (FdoDataValue* argLeft);

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
