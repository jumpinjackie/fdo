#ifndef _DECIMALVALUE_H_
#define _DECIMALVALUE_H_
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
#include <Fdo/Expression/DataValue.h>
#include <Fdo/Schema/DataType.h>

/// \brief
/// The FdoDecimalValue class derives from FdoDataValue and represents a decimal value.
class FdoDecimalValue : public FdoDataValue
{
/// \cond DOXYGEN-IGNORE
    friend class FdoByteValue;
    friend class FdoInt16Value;
    friend class FdoInt32Value;
    friend class FdoInt64Value;
    friend class FdoSingleValue;
    friend class FdoStringValue;
    friend class FdoDataValue;
protected:
    /// \brief
    /// Constructs a default instance of an FdoDecimalValue with a
    /// value of null.
    /// \return
    /// Returns nothing
    /// 
    FdoDecimalValue();

    /// \brief
    /// Constructs a default instance of an FdoDecimalValue using the specified arguments.
    /// \param value 
    /// Input a double
    /// 
    /// \return
    /// Returns nothing
    /// 
    FdoDecimalValue(double value);

    /// \brief
    /// Default destructor for FdoDecimalValue.
    /// \return
    /// Returns nothing
    /// 
    virtual ~FdoDecimalValue();

    virtual void Dispose();
/// \endcond

public:

    /// \brief
    /// Constructs a default instance of an FdoDecimalValue with a value of null.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API static FdoDecimalValue* Create();

    /// \brief
    /// Constructs an instance of an FdoDecimalValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a double
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API static FdoDecimalValue* Create(double value);

    /// \brief
    /// Constructs an instance of an FdoDecimalValue from another FdoDataValue.
    /// 
    /// \param src 
    /// Input the other FdoDataValue. Must be of one of the following types:
    ///     FdoDataType_Boolean
    ///     FdoDataType_Byte
    ///     FdoDataType_Decimal
    ///     FdoDataType_Double
    ///     FdoDataType_Int16
    ///     FdoDataType_Int32
    ///     FdoDataType_Int64
    ///     FdoDataType_Single
    ///     FdoDataType_String
    ///         - value must be numeric.
    ///
    /// In all other cases, the src type is considered incompatible with this type.
    /// \param nullIfIncompatible 
    /// Input will determine what to do if the source value cannot be converted to 
    /// this type:
    ///     true - return NULL.
    ///     false - throw an exception
    /// 
    /// \param shift 
    /// Input determines whether FdoInt64 values are allowed to shift when they have 
    //  more precision that can be handled by a double.
    ///     true - convert values allowing them to shift.
    ///     false - behaviour depends on nullIfIncompatible:
    ///         true - return NULL.
    ///         false - throw an exception
    /// \param truncate 
    /// Input determines what to do if source value is outside the FdoDouble range
    ///  ( -1.7e308, 1.7e308 ):
    ///     true - convert values less than -1.7e308 to -1.7e308, convert values greater than 1.7e308 to 1.7e308
    ///     false - behaviour depends on nullIfIncompatible:
    ///         true - return NULL.
    ///         false - throw an exception
    /// \return
    /// Returns an FdoDecimalValue, whose value is converted from the src value. 
    /// If src is an FdoBooleanValue:
    ///     false is converted to 0
    ///     true is converted to 1
    FDO_API static FdoDecimalValue* Create(
        FdoDataValue* src, 
        FdoBoolean nullIfIncompatible = false,
        FdoBoolean shift = true, 
        FdoBoolean truncate = false 
    );

    /// \brief
    /// Gets the data type of the FdoDecimalValue.
    /// 
    /// \return
    /// Returns an FdoDataType
    /// 
    FDO_API FdoDataType GetDataType();

    /// \brief
    /// Gets the decimal value.
    /// 
    /// \return
    /// Returns a double
    /// 
    FDO_API double GetDecimal();

    /// \brief
    /// Sets the decimal value.
    /// 
    /// \param value 
    /// Input a double
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void SetDecimal(double value);

    /// \brief
    /// Overrides FdoExpression.Process to pass the FdoDecimalValue to the appropriate
    /// expression processor operation.
    /// 
    /// \param p 
    /// Input an FdoIExpressionProcessor
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void Process(FdoIExpressionProcessor* p);

    /// \brief
    /// Returns the well defined text representation of this expression.
    /// 
    /// \return
    /// Returns a character string
    /// 
    FDO_API FdoString* ToString();

    /// \brief
    /// A cast operator to get the decimal value.
    /// 
    /// \return
    /// Returns a double
    /// 
    FDO_API operator double()
    {
        return m_data;
    }

/// \cond DOXYGEN-IGNORE
protected:
    // See FdoDataValue::DoCompare()
    virtual FdoCompareType DoCompare( FdoDataValue* other );

    double  m_data;
/// \endcond
};
#endif



