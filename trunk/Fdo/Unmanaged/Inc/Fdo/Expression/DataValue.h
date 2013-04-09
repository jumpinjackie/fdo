#ifndef _DATAVALUE_H_
#define _DATAVALUE_H_
// 

//
// Copyright (C) 2004-2007  Autodesk, Inc.
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
#include <Fdo/Expression/LiteralValue.h>
#include <Fdo/Schema/DataType.h>
#include <Fdo/Expression/ExpressionException.h>

/// \brief
/// The FdoDataValue class derives from FdoLiteralValue and represents a literal
/// value such as a string or a number.
class FdoDataValue : public FdoLiteralValue
{
/// \cond DOXYGEN-IGNORE
    friend class FdoInternalDataValue;
    friend class FdoByteValue;
    friend class FdoDecimalValue;
    friend class FdoDoubleValue;
    friend class FdoInt16Value;
    friend class FdoInt32Value;
    friend class FdoInt64Value;
    friend class FdoSingleValue;
protected:
    /// \brief
    ///  Constructs a default instance of an FdoDataValue with data type string and a
    /// value of null.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FdoDataValue();
/// \endcond

public:
    /// \brief
    /// Constructs an instance of a null FdoDataValue using the specified arguments.
    /// 
    /// \param dataType 
    /// Input data type
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API static FdoDataValue* Create(FdoDataType dataType);

    /// \brief
    /// Constructs an instance of an FdoBooleanValue using the specified argument.
    /// 
    /// \param value 
    /// Input a Boolean value
    /// 
    /// \return
    /// Returns an FdoBooleanValue
    /// 
    FDO_API static FdoDataValue* Create(bool value);

    /// \brief
    /// Constructs an instance of an FdoByteValue using the specified argument.
    /// 
    /// \param value 
    /// Input a byte
    /// 
    /// \return
    /// Returns an FdoByteValue
    /// 
    FDO_API static FdoDataValue* Create(FdoByte value);

    /// \brief
    /// Constructs an instance of an FdoDateTimeValue using the specified argument.
    /// 
    /// \param value 
    /// Input a FdoDateTime
    /// 
    /// \return
    /// Returns an FdoDateTimeValue
    /// 
    FDO_API static FdoDataValue* Create(FdoDateTime value);

    /// \brief
    /// Constructs an instance of an FdoDecimalValue or FdoDoubleValue using 
    /// the specified arguments.
    /// 
    /// \param value 
    /// Input a double
    /// \param dataType 
    /// Input a data type
    /// 
    /// \return
    /// Returns an FdoDecimalValue or FdoDoubleValue
    /// 
    FDO_API static FdoDataValue* Create(double value, FdoDataType dataType);

    /// \brief
    /// Constructs an instance of an FdoInt16Value using the specified argument.
    /// 
    /// \param value 
    /// Input a 16 bit integer
    /// 
    /// \return
    /// Returns an FdoInt16Value
    /// 
    FDO_API static FdoDataValue* Create(FdoInt16 value);

    /// \brief
    /// Constructs an instance of an FdoInt32Value using the specified argument.
    /// 
    /// \param value 
    /// Input a 32 bit integer
    /// 
    /// \return
    /// Returns an FdoInt32Value
    /// 
    FDO_API static FdoDataValue* Create(FdoInt32 value);

    /// \brief
    /// Constructs an instance of an FdoInt64Value using the specified argument.
    /// 
    /// \param value 
    /// Input a 64 bit integer
    /// 
    /// \return
    /// Returns an FdoInt64Value
    /// 
    FDO_API static FdoDataValue* Create(FdoInt64 value);

    /// \brief
    /// Constructs an instance of an FdoSingleValue using the specified argument.
    /// 
    /// \param value 
    /// Input a single precision floating point value
    /// 
    /// \return
    /// Returns an FdoSingleValue
    /// 
    FDO_API static FdoDataValue* Create(float value);

    /// \brief
    /// Constructs an instance of an FdoStringValue using the specified argument.
    /// 
    /// \param value 
    /// Input a character string
    /// 
    /// \return
    /// Returns an FdoStringValue
    /// 
    FDO_API static FdoDataValue* Create(FdoString* value);

    /// \brief
    /// Constructs an instance of an FdoBLOBValue or FdoCLOBValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a byte array
    /// \param length 
    /// Input the length of the byte array
    /// \param dataType 
    /// Input the type of value to create
    /// 
    /// \return
    /// Returns an FdoBLOBValue or an FdoCLOBValue
    /// 
    FDO_API static FdoDataValue* Create(FdoByte* value, FdoInt32 length, FdoDataType dataType);

    /// \brief
    /// Constructs an instance of an FdoBLOBValue or FdoCLOBValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a byte array
    /// \param dataType 
    /// Input the type of value to create
    /// 
    /// \return
    /// Returns an FdoBLOBValue or an FdoCLOBValue
    /// 
    FDO_API static FdoDataValue* Create(FdoByteArray* value, FdoDataType dataType);

    /// \brief
    /// Constructs an instance of an FdoDataValue from another FdoDataValue.
    /// 
    /// \param dataType
    /// Input the destination type. An FdoDataValue of this type is created. 
    /// For more information on how the FdoDataValue is created, see the specific 
    /// Create(FdoDataValue* FdoBoolean, FdoBoolean, FdoBoolean ) function for
    /// each dataType. For example, if dataType=FdoDataType_Int16, see
    /// FdoInt16Value::Create(FdoDataValue* FdoBoolean, FdoBoolean, FdoBoolean )
    /// \param src 
    /// Input the source (other) FdoDataValue
    /// \param nullIfIncompatible 
    /// Input will determine what to do if source value type is not compatible with the 
    /// this type:
    ///     true - return NULL.
    ///     false - throw an exception
    /// 
    /// \param shift 
    /// Input will determine what to do if the src value is within the range of 
    /// valid values for the destination type but still must be shifted to be a 
    /// valid destination type value (e.g. 3.5 must be shifted to convert it from
    /// FdoDoubleValue to FdoInt32Value):
    ///     true - perform the shift.
    ///     false - behaviour depends on nullIfIncompatible:
    ///         true - return NULL.
    ///         false - throw an exception
    /// 
    /// \param truncate 
    /// Input in the future will determine what to do if source value is outside the
    ///  range of valid values for the destination type:
    ///     true - convert values less than the minimum to the minimum, 
    ///            convert values greater than maximum to the maximum
    ///     false - behaviour depends on nullIfIncompatible:
    ///         true - return NULL.
    ///         false - throw an exception
    /// \return
    /// Returns an FdoDataValue, whose value is converted from the src value. 
    FDO_API static FdoDataValue* Create(
        FdoDataType dataType,
        FdoDataValue* src, 
        FdoBoolean nullIfIncompatible = false,
        FdoBoolean shift = true,
        FdoBoolean truncate = false
    );

    /// \brief
    /// Gets the data type of the FdoDataValue.
    /// 
    /// \return
    /// Returns an FdoDataType
    /// 
    FDO_API virtual FdoDataType GetDataType() = 0;

    /// \brief
    /// Returns true if the FdoDataValue represents a null value.
    /// 
    /// \return
    /// Returns true if the FdoDataValue represents a null value
    /// 
    FDO_API virtual bool IsNull();

    /// \brief
    /// Sets the FdoDataValue to a null value of the specified type.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API virtual void SetNull();

    /// \brief
    /// Gets the literal value type of the FdoDataValue.
    /// 
    /// \return
    /// Returns an FdoLiteralValueType
    /// 
   	FDO_API virtual FdoLiteralValueType GetLiteralValueType () const;

    /// \brief
    /// Returns the type of the expression as data value
    /// 
    /// \return
    /// Returns FdoExpressionItemType
    /// 
   	FDO_API FdoExpressionItemType GetExpressionType() { return FdoExpressionItemType_DataValue; };

	static FdoDataValue* Create(FdoString* value, FdoDataType dataType);

    // Get the value as an XML format string.
    FdoString* GetXmlValue();

/// \cond DOXYGEN-IGNORE
protected:
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

    /// \brief
    /// Constructs an instance of an FdoDoubleValue from a double
    /// 
    /// \param value
    /// Input the double value
    /// \return
    /// Returns an FdoDoubleValue
    ///
    static FdoDataValue* Create( FdoDouble value );

/* Helper Templates for FdoDataValue::Create( FdoDataType, FdoDataValue*, FdoBoolean, FdoBoolean, FdoBoolean ) */

    // Converts values that may need shifting to other values.
    template <class CI,             // input FdoDataValue 
              class CO,             // output FdoDataValue
              class TI,             // input FDO type - must correspond to CI
              class TO              // output FDO type - must correspond to CO
    > 
    static CO* Convert( 
        CI* obj,                            // input FdoDataValue
        TI val,                             // input scalar value for obj
        TO min,                             // minimum value for TO type
        TO max,                             // maximum value for TO type
        TI maxPlusOne,                      // trouble maker when converting floating point
                                            // to integral.
        TI round,                           // amount for rounding 
        FdoBoolean nullIfIncompatible,      // see FdoDataValue::Create()
        FdoBoolean shift,                   // see FdoDataValue::Create()
        FdoBoolean truncate,                // see FdoDataValue::Create()
        FdoString* sTO                       // TO in string format; for exception messages.
    )
    {
        CO*         ret = NULL;
        FdoBoolean  isNull = false;
        TO          out;

        // First, truncate the value to be between output type min and max.
        Truncate<TI,TO>( val, out, isNull, min, max, maxPlusOne, round, nullIfIncompatible, truncate, sTO );
                
        if ( isNull ) 
        {
            // Output determined to be null
            ret = CO::Create();
        }
        else if ( (val < min) || (val > max) ) 
        {
            // value was truncated. Wrap it in FdoDataValue.
            ret = CO::Create(out);
        }
        // When (round != 0) we're converting floating point to integral.
        //  If val = max + 1 and the floating 
        //  point type is sometimes less precise that the integral type then sometimes
        //  (val > max) evaluates to false but (val == max) evaluates to true. 
        //  The following handles this case:
        //    (round != 0): converting from floating point to integral
        //    (val == maxPlusOne): value was truncated by Truncate().
        else if ( (round != 0) && (val == maxPlusOne) ) 
        {
            // value was truncated. Wrap it in FdoDataValue.
            ret = CO::Create(out);
        }
        else
        {
            // Round the value and convert it
            ret = CO::Create( (TO) ((val < 0) ? (val - round) : (val + round)) );
            // Prevent value shifting when shifting not allowed.
            VldShift( obj, ret, nullIfIncompatible, shift );
        }

        return ret;
    };

    // Converts values that don't need shifting, but may need truncation,
    // to other values.
    template <
        class C,                    // output FdoDataValue
        class TI,                   // input FDO type 
        class TO                    // output FDO type - must correspond to CO
    > 
    static C* Convert( 
        TI val,                             // input scalar value for obj
        TO min,                             // minimum value for TO type
        TO max,                             // maximum value for TO type
        FdoBoolean nullIfIncompatible,      // see FdoDataValue::Create()
        FdoBoolean truncate,                // see FdoDataValue::Create()
        FdoString* sTO                      // TO in string format; for exception messages.
    )
    {
        C*          ret    = NULL;
        FdoBoolean  isNull = false;
        TO          out;

        // First, truncate the value to be between output type min and max.
        Truncate<TI, TO>( val, out, isNull, min, max, 0, 0, nullIfIncompatible, truncate, sTO );
                
        if ( isNull ) 
            // Output determined to be null
            ret = C::Create();
        else
            // Wrap output in an FdoDataValue.
            ret = C::Create( out );

        return ret;
    };

    // Truncates a value to be between a minimum and maximum value.
    template <
        class TI,                   // input FDO type 
        class TO                    // output FDO type
    > 
    static bool Truncate( 
        TI in,                      // original value
        TO& out,                    // truncated value
        bool& isNull,               // set to true if value truncated and
                                    // truncate=false and nullIfIncompatible=true
        TO min,                     // minimum value for TO type
        TO max,                     // maximum value for TO type
        TI maxPlusOne,              // trouble maker when converting floating point to integral.
        TI round,
        bool nullIfIncompatible,    // see FdoDataValue::Create()
        bool truncate,              // see FdoDataValue::Create()
        FdoString* sTO              // TO in string format; for exception messages.
    )
    {
        bool success = true;

        // no truncation by default. However, it is up to the caller to 
        // determine if casting shifted the value.
        out = (TO) in;

        if ( in < min ) 
            success = Truncate<TI, TO>( in, out, isNull, min, nullIfIncompatible, truncate );   
        else if ( in > max ) {
            success = Truncate<TI, TO>( in, out, isNull, max, nullIfIncompatible, truncate );
        }
        // This test handles a precision problem. See note in Compare().
        else if ( (round != 0) && (in == maxPlusOne) ) {
            success = Truncate<TI, TO>( in, out, isNull, max, nullIfIncompatible, truncate );
        }

        if ( !success )
            throw FdoExpressionException::Create(
                FdoException::NLSGetMessage(
                    FDO_NLSID(EXPRESSION_21_VALUETRUNCATED),
                    FdoPtr<FdoDataValue>(FdoDataValue::Create(in))->ToString(),
                    sTO,
                    FdoPtr<FdoDataValue>(FdoDataValue::Create(min))->ToString(),
                    FdoPtr<FdoDataValue>(FdoDataValue::Create(max))->ToString()
                )
            );

        return true;
    };

    // Truncates a value to a range endpoint
    template <
        class TI,                   // input FDO type 
        class TO                    // output FDO type
    > 
    static bool Truncate( 
        TI in,                      // input value
        TO& out,                    // output value
        bool& isNull,               // set to true truncate=false and 
                                    // nullIfIncompatible=true 
        TO bound,                   // the range endpoint
        bool nullIfIncompatible,    // see FdoDataValue::Create() 
        bool truncate               // see FdoDataValue::Create()
    )
    {
        bool ret = true;
        
        if ( truncate ) 
        {
            // Truncation allowed so set output to the range endpoint
            out = bound;
        }
        else
        {
            if ( nullIfIncompatible ) 
                // Truncation not allowed so set output to null
                isNull = true;
            else
                // Truncation not allowed so raise an error
                ret = false;
        }

        return ret;
    };

    // Helper function for FdoDataValue::Create( FdoDataType, FdoDataValue*, FdoBoolean, FdoBoolean, FdoBoolean )
    // Adjusts shifted values depending on whether shift is allowd 
    static void VldShift( 
        FdoDataValue* origValue,        // original value
        FdoDataValue* newValue,         // converted value
        bool nullIfIncompatible,        // Determines what happens when shift=false
                                        // and origValue != newValue (shift occurred
                                        // when origValue was converted to newValue but 
                                        // shifting not allowed ):
                                        //     true: set newValue to null.
                                        //     false: throw an exception
         bool shift                     // determines whether value allowed to shift. 
     );

    static FdoStringP DataTypeToString( FdoDataType dataType );

    bool        m_isNull;
    FdoStringP  m_XmlValue; // Manages GetXmlValue return string when datatype is datetime.
/// \endcond

};

/// \ingroup (typedefs)
/// \brief
/// FdoDataValueP is a FdoPtr on FdoDataValue, provided for convenience.
typedef FdoPtr<FdoDataValue> FdoDataValueP;

#endif



