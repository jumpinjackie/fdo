// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
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
#ifndef SDFFILTEREXECUTOR_H
#define SDFFILTEREXECUTOR_H

#include "PropertyIndex.h"
#include "DataValue.h"
#include "DataValueStack.h"

typedef DataValueStack<DataValue> retval_stack;

//forward declare
class DataValuePool;

///<summary>
///An implementation of FdoIFilterProcessor and FdoIExpressionProcessor which
///evaluates a filter in the context of a given feature, via an FdoIFeatureReader.
///</summary>
//TODO: get rid of the multiple inheritance!!!
class FilterExecutor : public FdoIFilterProcessor, public FdoIExpressionProcessor
{

public:

    FilterExecutor(FdoIFeatureReader* featureReader, PropertyIndex* propIndex, FdoIdentifierCollection* compIdents, FdoClassDefinition* classDef);
    ~FilterExecutor();

protected:

    void Dispose();

public:

    static FilterExecutor* Create(FdoIFeatureReader* featureReader, PropertyIndex* propIndex, FdoIdentifierCollection* compIdents, FdoClassDefinition* classDef);

public:

    //--------------------------------------------------------------------
    // FdoIFilterProcessor
    //--------------------------------------------------------------------

    virtual void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter);

    virtual void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter);

    virtual void ProcessComparisonCondition(FdoComparisonCondition& filter);

    virtual void ProcessInCondition(FdoInCondition& filter);

    virtual void ProcessNullCondition(FdoNullCondition& filter);

    virtual void ProcessSpatialCondition(FdoSpatialCondition& filter);

    virtual void ProcessDistanceCondition(FdoDistanceCondition& filter);


    //--------------------------------------------------------------------
    // FdoIExpressionProcessor
    //--------------------------------------------------------------------

    virtual void ProcessBinaryExpression(FdoBinaryExpression& expr);

    virtual void ProcessUnaryExpression(FdoUnaryExpression& expr);

    virtual void ProcessFunction(FdoFunction& expr);

    virtual void ProcessIdentifier(FdoIdentifier& expr);

	virtual void ProcessComputedIdentifier(FdoComputedIdentifier& expr);

    virtual void ProcessParameter(FdoParameter& expr);

    virtual void ProcessBooleanValue(FdoBooleanValue& expr);

    virtual void ProcessByteValue(FdoByteValue& expr);

    virtual void ProcessDateTimeValue(FdoDateTimeValue& expr);

    virtual void ProcessDecimalValue(FdoDecimalValue& expr);

    virtual void ProcessDoubleValue(FdoDoubleValue& expr);

    virtual void ProcessInt16Value(FdoInt16Value& expr);

    virtual void ProcessInt32Value(FdoInt32Value& expr);

    virtual void ProcessInt64Value(FdoInt64Value& expr);

    virtual void ProcessSingleValue(FdoSingleValue& expr);

    virtual void ProcessStringValue(FdoStringValue& expr);

    virtual void ProcessBLOBValue(FdoBLOBValue& expr);

    virtual void ProcessCLOBValue(FdoCLOBValue& expr);

    virtual void ProcessGeometryValue(FdoGeometryValue& expr);


public:

    //generic identifier processing, including computed and non-computed
    //identifiers
    virtual void ProcessIdentifier(FdoString* name);

    //-----------------------------------------------------------------
    // SdfFilterExecutor
    //-----------------------------------------------------------------

    void Reset();
    DataValueType GetResultType();
    bool GetBooleanResult();
    double GetDoubleResult();
    FdoInt64 GetInt64Result();
    wchar_t* GetStringResult(); //caller frees return value!!!
    FdoDateTime GetDateTimeResult();

protected:
    //execution stack
    retval_stack m_retvals;

    //feature reader
    FdoIFeatureReader* m_reader;

	// The class definition
	FdoClassDefinition* m_classDefinition;

    //feature class property cache
    PropertyIndex* m_propIndex;

    //data storage object pool
    DataValuePool* m_pPool;

    //list of computed identifiers
    FdoIdentifierCollection* m_compIdents;

private:

	void PushIdentifierValue(FdoIFeatureReader* reader, FdoString* name, FdoDataType type );

    void ExecuteARGB(FdoFunction& function);
    
    //pattern matching
    bool MatchesHere(wchar_t* pattern, wchar_t* src);
    bool MatchPercent(wchar_t* pattern, wchar_t* src);
    bool MatchBracket(wchar_t* pattern, wchar_t* src);

};


#endif

