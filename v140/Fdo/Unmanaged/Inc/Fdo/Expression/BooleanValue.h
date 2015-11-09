#ifndef _BOOLEANVALUE_H_
#define _BOOLEANVALUE_H_
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

/// \brief
/// The FdoBooleanValue class derives from FdoDataValue and represents a Boolean
/// value.
class FdoBooleanValue : public FdoDataValue
{
    friend class FdoStringValue;
    friend class FdoDataValue;
/// \cond DOXYGEN-IGNORE
protected:
    /// \brief
    /// Constructs a default instance of an FdoBooleanValue with a
    /// value of null.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FdoBooleanValue();

    /// \brief
    /// Constructs a default instance of an FdoBooleanValue using the specified
    /// arguments.
    /// 
    /// \param value 
    /// Input a Boolean value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FdoBooleanValue(bool value);

    /// \brief
    /// Default destructor for FdoBooleanValue.
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual ~FdoBooleanValue();

    virtual void Dispose();

/// \endcond

public:
    /// \brief
    /// Constructs a default instance of an FdoBooleanValue with a value of null.
    /// 
    /// \return
    /// Returns the created FdoBooleanValue
    /// 
    FDO_API static FdoBooleanValue* Create();

    /// \brief
    /// Constructs a default instance of an FdoBooleanValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a Boolean value
    /// 
    /// \return
    /// Returns the created FdoBooleanValue
    /// 
    FDO_API static FdoBooleanValue* Create(bool value);

    /// \brief
    /// Constructs an instance of an FdoBooleanValue from another FdoDataValue.
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
    ///         - value must be "TRUE", "FALSE", or numeric.
    ///
    /// In all other cases, the src type is considered incompatible with this type.
    /// \param nullIfIncompatible 
    /// Input will determine what to do if the source value cannot be converted to 
    /// this type:
    ///     true - return NULL.
    ///     false - throw an exception
    /// 
    /// \param shift 
    /// Input for future use.
    /// \param truncate 
    /// Input in the future will determine what to do if source value is numeric but
    /// not 0 or 1:
    ///     true - set the FdoBooleanValue to true.
    ///     false - behaviour depends on nullIfIncompatible:
    ///         true - return NULL.
    ///         false - throw an exception
    /// \return
    /// Returns an FdoBooleanValue, whose value is converted from the src value. 
    /// If src value is numeric then:
    ///     0 is converted to false
    ///     1 is converted to true
    FDO_API static FdoBooleanValue* Create(
        FdoDataValue* src, 
        FdoBoolean nullIfIncompatible = false,
        FdoBoolean shift = true, 
        FdoBoolean truncate = false 
    );

    /// \brief
    /// Gets the data type of the FdoBooleanValue.
    /// 
    /// \return
    /// Returns an FdoDataType
    /// 
    FDO_API FdoDataType GetDataType();

    /// \brief
    /// Gets the FdoBooleanValue.
    /// 
    /// \return
    /// Returns a Boolean value
    /// 
    FDO_API bool GetBoolean();

    /// \brief
    /// Sets the Boolean value.
    /// 
    /// \param value 
    /// Input a Boolean value
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void SetBoolean(bool value);

    /// \brief
    ///  Overrides FdoExpression.Process to pass the FdoBooleanValue to the appropriate
    /// expression processor operation.
    /// 
    /// \param p 
    /// Input the FdoIExpressionProcessor
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API void Process(FdoIExpressionProcessor* p);

    /// \brief
    /// Returns the well defined text representation of this expression.
    /// 
    /// \return
    /// Returns a character string.
    /// 
    FDO_API FdoString* ToString();

    /// \brief
    /// A cast operator to get the Boolean value.
    /// 
    /// \return
    /// Returns a Boolean value
    /// 
    FDO_API operator bool()
    {
        return m_data;
    }

protected:
    /// \brief
    /// Helper template for constructing FdoBooleanValue's from scalar values
    /// of various types.
    /// 
    template <class T> static FdoBooleanValue* Convert( FdoDataValue* src, T val, bool nullIfIncompatible, bool truncate )
    {
        FdoBooleanValue*   ret    = NULL;
        bool isNull = false;

        if ( val == 0 ) 
        {
            ret = FdoBooleanValue::Create(false);
        }
        else if ( (val == 1) || truncate )  
        {
            // Values other than 0 or 1 map to true but only if allowing 
            // value truncation.
            ret = FdoBooleanValue::Create(true);
        }
        else if ( nullIfIncompatible ) 
        {
            // Failed to convert, return null FdoBooleanValue
            ret = FdoBooleanValue::Create();
        }
        else {
            // Failed to convert, error condition
            throw FdoExpressionException::Create(
                FdoException::NLSGetMessage(
                    FDO_NLSID(EXPRESSION_24_BOOLEANTRUNCATED),
                    src->ToString()
                )
            );
        }

        return ret;
    };

    bool        m_data;
};
#endif



