/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _FDOCOMMONFILTEREXECUTOR_H_
#define _FDOCOMMONFILTEREXECUTOR_H_

#ifdef _WIN32
#pragma once
#endif

#include <vector>

typedef std::vector<FdoLiteralValue*> retval_stack;

/// \brief
/// An implementation of FdoIFilterProcessor and FdoIExpressionProcessor which
/// evaluates a filter in the context of a given feature, via an FdoIFeatureReader.
class FdoCommonFilterExecutor : public FdoIFilterProcessor, public FdoIExpressionProcessor
{

protected:
	FdoCommonFilterExecutor() {};

    /// execution stack
    retval_stack m_retvals;

    /// feature reader
    FdoIReader* m_reader;   // NOTE: weak reference

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

public:

    FdoCommonFilterExecutor (FdoIReader* reader, FdoIdentifierCollection* compIdents);
    ~FdoCommonFilterExecutor ();

protected:

    void Dispose ();

public:

    static FdoCommonFilterExecutor* Create (FdoIReader* featureReader, FdoIdentifierCollection* compIdents);

    /// \brief
    /// Increase the reference count.
    /// 
    /// \return
    /// Returns the new reference count (value for debugging use only).
    /// 
    FdoInt32 AddRef ()
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
    FdoInt32 Release ()
    {
    /// NOTE: due to multiple inheritance, there is an ambiguity in which Release() method to call.
    ///  Calling BOTH Release() methods leads to instances of this class being prematurely released.
        return FdoIFilterProcessor::Release ();
    }

public:

    //
    /// FdoIFilterProcessor interface
    //

    virtual void ProcessBinaryLogicalOperator (FdoBinaryLogicalOperator& filter);

    virtual void ProcessUnaryLogicalOperator (FdoUnaryLogicalOperator& filter);

    virtual void ProcessComparisonCondition (FdoComparisonCondition& filter);

    virtual void ProcessInCondition (FdoInCondition& filter);

    virtual void ProcessNullCondition (FdoNullCondition& filter);

    virtual void ProcessSpatialCondition (FdoSpatialCondition& filter);

    virtual void ProcessDistanceCondition (FdoDistanceCondition& filter);


    //
    /// FdoIExpressionProcessor interface
    //

    virtual void ProcessBinaryExpression (FdoBinaryExpression& expr);

    virtual void ProcessUnaryExpression (FdoUnaryExpression& expr);

    virtual void ProcessFunction (FdoFunction& expr);

    virtual void ProcessIdentifier (FdoIdentifier& expr);

	virtual void ProcessComputedIdentifier (FdoComputedIdentifier& expr);

    virtual void ProcessParameter (FdoParameter& expr);

    virtual void ProcessBooleanValue (FdoBooleanValue& expr);

    virtual void ProcessByteValue (FdoByteValue& expr);

    virtual void ProcessDateTimeValue (FdoDateTimeValue& expr);

    virtual void ProcessDecimalValue (FdoDecimalValue& expr);

    virtual void ProcessDoubleValue (FdoDoubleValue& expr);

    virtual void ProcessInt16Value (FdoInt16Value& expr);

    virtual void ProcessInt32Value (FdoInt32Value& expr);

    virtual void ProcessInt64Value (FdoInt64Value& expr);

    virtual void ProcessSingleValue (FdoSingleValue& expr);

    virtual void ProcessStringValue (FdoStringValue& expr);

    virtual void ProcessBLOBValue (FdoBLOBValue& expr);

    virtual void ProcessCLOBValue (FdoCLOBValue& expr);

    virtual void ProcessGeometryValue (FdoGeometryValue& expr);

    //
    /// implementors need to override these
    //

    /// \brief
    /// Get the property info.
    /// 
    /// \param name 
    /// Input the name of the property to get.
    /// 
    /// \return
    /// Returns the property info (with all properties, not just
    /// the ones selected) from the class definition,
    /// or NULL if it is a computed property.
    /// 
    virtual FdoPropertyDefinition* GetPropInfo (FdoString* name);
    virtual void Reset ();

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

    //
    /// processing
    //

    virtual bool Like (FdoDataValue* argLeft, FdoDataValue* argRight);
    static bool IsEqualTo (FdoDataValue* argLeft, FdoDataValue* argRight);
    static bool IsLessThan (FdoDataValue* argLeft, FdoDataValue* argRight);
    static bool IsGreaterThan (FdoDataValue* argLeft, FdoDataValue* argRight);
    virtual FdoDataValue* Add (FdoDataValue* argLeft, FdoDataValue* argRight);
    virtual FdoDataValue* Subtract (FdoDataValue* argLeft, FdoDataValue* argRight);
    virtual FdoDataValue* Multiply (FdoDataValue* argLeft, FdoDataValue* argRight);
    virtual FdoDataValue* Divide (FdoDataValue* argLeft, FdoDataValue* argRight);
    virtual FdoDataValue* Negate (FdoDataValue* argLeft);


    static void GetExpressionType(FdoIConnection* conn, FdoClassDefinition* originalClassDef, FdoExpression *expr, FdoPropertyType &propType, FdoDataType &dataType);

public:

    //generic identifier processing, including computed and non-computed
    //identifiers
    virtual void ProcessIdentifier (FdoString* name);

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

private:

    /// pattern matching
    bool MatchesHere (const wchar_t* pattern, const wchar_t* src);
    bool MatchPercent (const wchar_t* pattern, const wchar_t* src);
    bool MatchBracket (const wchar_t* pattern, const wchar_t* src);
};

#endif // _FDOCOMMONFILTEREXECUTOR_H_


