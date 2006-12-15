#ifndef FDO_EXPRESSION_INTERNAL_H
#define FDO_EXPRESSION_INTERNAL_H
// 

//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifdef _WIN32
#pragma once
#endif

#include <FdoStd.h>

// Temporary wrapper classes that extend the functionality of the expression classes.
// These functions should eventually be migrated into the API.
// NOTE: Each class has a expression class member, but this member is not ref-counted
// Therefore these wrapper classes must not be kept around but must be destroyed before their members.
class FdoInternalDataValue : public FdoIDisposable
{
public:
    static FdoInternalDataValue* Create( FdoDataValue* dataValue );
    FdoInternalDataValue() {}
    FdoInternalDataValue( FdoDataValue* dataValue );

    /// \brief
    /// Compares this data value with another data value
    /// 
    /// \param other 
    /// Input the other data value
    /// 
    /// \return
    /// Returns:
    ///     FdoCompareType_Equal when this and the other value are equal or both null.
    ///     FdoCompareType_Greater when this value is greater than the other value
    ///     FdoCompareType_Less when this value is less than the other value
    ///     FdoCompareType_Undefined when these two values cannot be compared. Cases where this happens
    ///      are:
    ///         - one value is null and the other is not null
    ///         - the values have incompatible types (e.g. Int32 and DateTime).
    ///         
    /// 
    FdoCompareType Compare( FdoDataValue* other );

protected:
    // ReverseCompare is the same as Compare except for two of the return values:
    ///     FdoCompareType_Greater when the other value is greater than this value
    ///     FdoCompareType_Less when the other value is less than this value
    FdoCompareType ReverseCompare( FdoDataValue* other );

    /// \brief
    /// Type-specific comparison function. Each sub-class has its own implementation.
    /// 
    /// \param other 
    /// Input the other data value
    /// 
    /// \return
    /// Returns:
    ///     FdoCompareType_Equal when this and the other value are equal or both null.
    ///     FdoCompareType_Greater when this value is greater than the other value
    ///     FdoCompareType_Less when this value is less than the other value
    ///     FdoCompareType_Undefined when these two values cannot be compared. Cases where this happens
    ///      are:
    ///         - one value is null and the other is not null
    ///         - the values have incompatible types (e.g. Int32 and DateTime).
    ///
    ///     Base implementation always returns FdoCompareType_Undefined.
    ///         
    /// 
    virtual FdoCompareType DoCompare( FdoDataValue* other );

    FdoDataValue* operator->()
    {
        return mDataValue;
    }

    operator FdoDataValue*( )
    {
        return mDataValue;
    }

protected:
    virtual void Dispose()
    {
        delete this;
    }

    FdoDataValue* mDataValue;
};

class FdoInternalByteValue : public FdoInternalDataValue
{
public:
    FdoInternalByteValue( FdoByteValue* dataValue );
    virtual ~FdoInternalByteValue();

    FdoByteValue* operator->()
    {
        return (FdoByteValue*) mDataValue;
    }

protected:
    virtual FdoCompareType DoCompare( FdoDataValue* other );
};

class FdoInternalDateTimeValue : public FdoInternalDataValue
{
public:
    FdoInternalDateTimeValue( FdoDateTimeValue* dataValue );
    virtual ~FdoInternalDateTimeValue();

    FdoDateTimeValue* operator->()
    {
        return (FdoDateTimeValue*) mDataValue;
    }

protected:
    virtual FdoCompareType DoCompare( FdoDataValue* other );
};

class FdoInternalDecimalValue : public FdoInternalDataValue 
{
public:
    /// \brief
    /// Constructs an instance of an FdoDecimalValue from another FdoDataValue.
    /// 
    /// \param src 
    /// Input the other FdoDataValue. Must be of one of the following types:
    ///     FdoDataType_Byte
    ///     FdoDataType_Decimal
    ///     FdoDataType_Int16
    ///     FdoDataType_Int32
    ///     FdoDataType_Single
    /// \param truncate 
    /// Input in the future, will determine what to do if source value does not fit in the decimal 
    /// number range:
    ///     true - truncate the value to fit.
    ///     false - throw an exception
    /// \param nullIfIncompatible 
    /// Input in the future, will determine what to do if source value type is not compatible with the 
    /// FDO decimal type:
    ///     true - return NULL.
    ///     false - throw an exception
    /// 
    /// \return
    /// Returns an FdoDecimalValue
    /// 
    static FdoDecimalValue* Create(
        FdoDataValue* src, 
        FdoBoolean truncate = false, 
        FdoBoolean nullIfIncompatible = false
    );

    FdoInternalDecimalValue( FdoDecimalValue* dataValue );
    virtual ~FdoInternalDecimalValue();

    FdoDecimalValue* operator->()
    {
        return (FdoDecimalValue*) mDataValue;
    }

protected:
    virtual FdoCompareType DoCompare( FdoDataValue* other );
};

class FdoInternalDoubleValue : public FdoInternalDataValue 
{
public:
    /// \brief
    /// Constructs an instance of an FdoDoubleValue from another FdoDataValue.
    /// 
    /// \param src 
    /// Input the other FdoDataValue. Must be of one of the following types:
    ///     FdoDataType_Byte
    ///     FdoDataType_Decimal
    ///     FdoDataType_Double
    ///     FdoDataType_Int16
    ///     FdoDataType_Int32
    ///     FdoDataType_Int64
    ///     FdoDataType_Single
    /// \param truncate 
    /// Input in the future, will determine what to do if source value does not fit in the double 
    /// number range:
    ///     true - truncate the value to fit.
    ///     false - throw an exception
    /// \param nullIfIncompatible 
    /// Input in the future, will determine what to do if source value type is not compatible with the 
    /// FDO double type:
    ///     true - return NULL.
    ///     false - throw an exception
    /// 
    /// \return
    /// Returns an FdoDoubleValue
    /// 
    static FdoDoubleValue* Create(
        FdoDataValue* src, 
        FdoBoolean truncate = false, 
        FdoBoolean nullIfIncompatible = false
    );

    FdoInternalDoubleValue( FdoDoubleValue* dataValue );
    virtual ~FdoInternalDoubleValue();

    FdoDoubleValue* operator->()
    {
        return (FdoDoubleValue*) mDataValue;
    }

protected:
    virtual FdoCompareType DoCompare( FdoDataValue* other );
};

class FdoInternalInt16Value : public FdoInternalDataValue 
{
public:
    /// \brief
    /// Constructs an instance of an FdoInt16Value from another FdoDataValue.
    /// 
    /// \param src 
    /// Input the other FdoDataValue. Must be of one of the following types:
    ///     FdoDataType_Byte
    ///     FdoDataType_Int16
    /// \param truncate 
    /// Input in the future, will determine what to do if source value does not fit in the int16
    /// number range:
    ///     true - truncate the value to fit.
    ///     false - throw an exception
    /// \param nullIfIncompatible 
    /// Input in the future, will determine what to do if source value type is not compatible with the 
    /// FDO int16 type:
    ///     true - return NULL.
    ///     false - throw an exception
    /// 
    /// \return
    /// Returns an FdoInt16Value
    /// 
    static FdoInt16Value* Create(
        FdoDataValue* src, 
        FdoBoolean truncate = false, 
        FdoBoolean nullIfIncompatible = false
    );

    FdoInternalInt16Value( FdoInt16Value* dataValue );
    virtual ~FdoInternalInt16Value();

    FdoInt16Value* operator->()
    {
        return (FdoInt16Value*) mDataValue;
    }

protected:
    virtual FdoCompareType DoCompare( FdoDataValue* other );
};

class FdoInternalInt32Value : public FdoInternalDataValue 
{
public:
    /// \brief
    /// Constructs an instance of an FdoInt32Value from another FdoDataValue.
    /// 
    /// \param src 
    /// Input the other FdoDataValue. Must be of one of the following types:
    ///     FdoDataType_Byte
    ///     FdoDataType_Int16
    ///     FdoDataType_Int32
    /// \param truncate 
    /// Input in the future, will determine what to do if source value does not fit in the int32
    /// number range:
    ///     true - truncate the value to fit.
    ///     false - throw an exception
    /// \param nullIfIncompatible 
    /// Input in the future, will determine what to do if source value type is not compatible with the 
    /// FDO int32 type:
    ///     true - return NULL.
    ///     false - throw an exception
    /// 
    /// \return
    /// Returns an FdoInt32Value
    /// 
    static FdoInt32Value* Create(
        FdoDataValue* src, 
        FdoBoolean truncate = false, 
        FdoBoolean nullIfIncompatible = false
    );

    FdoInternalInt32Value( FdoInt32Value* dataValue );
    virtual ~FdoInternalInt32Value();

    FdoInt32Value* operator->()
    {
        return (FdoInt32Value*) mDataValue;
    }

protected:
    virtual FdoCompareType DoCompare( FdoDataValue* other );
};

class FdoInternalInt64Value : public FdoInternalDataValue 
{
public:
    /// \brief
    /// Constructs an instance of an FdoInt64Value from another FdoDataValue.
    /// 
    /// \param src 
    /// Input the other FdoDataValue. Must be of one of the following types:
    ///     FdoDataType_Byte
    ///     FdoDataType_Int16
    ///     FdoDataType_Int32
    ///     FdoDataType_Int64
    /// \param truncate 
    /// Input in the future, will determine what to do if source value does not fit in the int64
    /// number range:
    ///     true - truncate the value to fit.
    ///     false - throw an exception
    /// \param nullIfIncompatible 
    /// Input in the future, will determine what to do if source value type is not compatible with the 
    /// FDO int64 type:
    ///     true - return NULL.
    ///     false - throw an exception
    /// 
    /// \return
    /// Returns an FdoInt64Value
    /// 
    static FdoInt64Value* Create(
        FdoDataValue* src, 
        FdoBoolean truncate = false, 
        FdoBoolean nullIfIncompatible = false
    );

    FdoInternalInt64Value( FdoInt64Value* dataValue );
    virtual ~FdoInternalInt64Value();

    FdoInt64Value* operator->()
    {
        return (FdoInt64Value*) mDataValue;
    }

protected:
    virtual FdoCompareType DoCompare( FdoDataValue* other );
};

class FdoInternalSingleValue : public FdoInternalDataValue 
{
public:
    /// \brief
    /// Constructs an instance of an FdoSingleValue from another FdoDataValue.
    /// 
    /// \param src 
    /// Input the other FdoDataValue. Must be of one of the following types:
    ///     FdoDataType_Byte
    ///     FdoDataType_Int16
    ///     FdoDataType_Int32
    ///     FdoDataType_Int64
    ///     FdoDataType_Single
    /// \param truncate 
    /// Input in the future, will determine what to do if source value does not fit in the double 
    /// number range:
    ///     true - truncate the value to fit.
    ///     false - throw an exception
    /// \param nullIfIncompatible 
    /// Input in the future, will determine what to do if source value type is not compatible with the 
    /// FDO float type:
    ///     true - return NULL.
    ///     false - throw an exception
    /// 
    /// \return
    /// Returns an FdoSingleValue
    /// 
    static FdoSingleValue* Create(
        FdoDataValue* src, 
        FdoBoolean truncate = false, 
        FdoBoolean nullIfIncompatible = false
    );

    FdoInternalSingleValue( FdoSingleValue* dataValue );
    virtual ~FdoInternalSingleValue();

    FdoSingleValue* operator->()
    {
        return (FdoSingleValue*) mDataValue;
    }

protected:
    virtual FdoCompareType DoCompare( FdoDataValue* other );
};

class FdoInternalStringValue : public FdoInternalDataValue 
{
public:
    FdoInternalStringValue( FdoStringValue* dataValue );
    virtual ~FdoInternalStringValue();

    FdoStringValue* operator->()
    {
        return (FdoStringValue*) mDataValue;
    }

protected:
    virtual FdoCompareType DoCompare( FdoDataValue* other );
};


#endif


