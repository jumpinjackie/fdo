#ifndef _STRINGVALUE_H_
#define _STRINGVALUE_H_
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
/// The FdoStringValue class derives from FdoDataValue and represents a literal string.
class FdoStringValue : public FdoDataValue
{
    friend class FdoBooleanValue;
    friend class FdoByteValue;
    friend class FdoDateTimeValue;
    friend class FdoDecimalValue;
    friend class FdoDoubleValue;
    friend class FdoInt16Value;
    friend class FdoInt32Value;
    friend class FdoInt64Value;
    friend class FdoSingleValue;
    friend class FdoDataValue;
/// \cond DOXYGEN-IGNORE
protected:
    /// \brief
    /// Constructs a default instance of an FdoStringValue with a
    /// value of null.
    /// \return
    /// Returns nothing
    /// 
    FdoStringValue();

    /// \brief
    /// Constructs a default instance of an FdoStringValue using the specified arguments.
    /// \return
    /// Returns nothing
    /// 
    FdoStringValue(FdoString* value);

    /// \brief
    /// Default destructor for FdoStringValue.
    virtual ~FdoStringValue();

    virtual void Dispose();
/// \endcond

public:
    /// \brief
    /// Constructs a default instance of an FdoStringValue with a value of null.
    /// 
    /// \return
    /// Returns the created FdoStringValue
    /// 
    FDO_API static FdoStringValue* Create();

    /// \brief
    /// Constructs a default instance of an FdoStringValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a character string
    /// 
    /// \return
    /// Returns the created FdoStringValue
    /// 
    FDO_API static FdoStringValue* Create(FdoString* value);

    /// \brief
    /// Constructs an instance of an FdoStringValue from another FdoDataValue.
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
    ///
    /// In all other cases, the src type is considered incompatible with this type.
    /// \param nullIfIncompatible 
    /// Input will determine what to do if the source value cannot be converted to 
    /// this type:
    ///     true - return NULL.
    ///     false - throw an exception
    /// 
    /// \param shift 
    /// Input determines whether FdoFloat or FdoDouble values are allowed to shift 
    /// when conversion to strings causes loss of precision.
    ///     true - convert values allowing them to shift.
    ///     false - behaviour depends on nullIfIncompatible:
    ///         true - return NULL.
    ///         false - throw an exception
    /// \param truncate 
    /// Input for future use. There are currently no possible out of range
    /// src values.
    /// \return
    /// Returns an FdoStringValue, whose value is converted from the src value. 
    /// If src is an FdoStringValue then its value is simply copied to the 
    /// returned FdoStringValue. Otherwise, the value is converted by calling
    /// src->ToString().
    ///
    FDO_API static FdoStringValue* Create(
        FdoDataValue* src, 
        FdoBoolean nullIfIncompatible = false,
        FdoBoolean shift = true, 
        FdoBoolean truncate = false 
    );

    /// \brief
    /// Gets the data type of the FdoStringValue.
    /// 
    /// \return
    /// Returns FdoDataType
    /// 
    FDO_API FdoDataType GetDataType();

    /// \brief
    /// Gets the FdoStringValue as a string.
    /// 
    /// \return
    /// Returns a character string
    /// 
    FDO_API FdoString* GetString();

    /// \brief
    /// Sets the FdoStringValue as a string.
    /// 
    /// \param value 
    /// Input a character string
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void SetString(FdoString* value);

    /// \brief
    /// Sets the FdoStringValue to a null value.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void SetNull();

    /// \brief
    /// Overrides FdoExpression.Process to pass the FdoStringValue to the appropriate
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
    /// A cast operator to get the string.
    /// 
    /// \return
    /// Returns a character string
    /// 
    FDO_API operator wchar_t*()
    {
        return m_data;
    }

protected:
    template <class C> C* ConvertFrom( bool nullIfIncompatible, bool shift, bool truncate, FdoString* sTO )
    {
        C*   ret = NULL;
        FdoPtr<FdoDataValue> parsed = Parse();
        
        if ( (parsed == NULL) || (parsed->IsNull()) || (parsed->GetDataType() == FdoDataType_String) ) 
        {
            if ( !nullIfIncompatible )
                throw FdoExpressionException::Create(
                    FdoException::NLSGetMessage(
                        FDO_NLSID(EXPRESSION_22_INCOMPATIBLEDATATYPES),
                        this->ToString(),
                        (FdoString *) DataTypeToString(GetDataType()),
                        sTO
                    )
                );
        }
        else
        {
            ret = C::Create(parsed, nullIfIncompatible, shift, truncate);
        };

        return ret;
    };

    // See FdoDataValue::DoCompare()
    virtual FdoCompareType DoCompare( FdoDataValue* other );

    FdoDataValue* Parse();

    wchar_t*    m_data;
    size_t      m_allocatedSize;
};
#endif



