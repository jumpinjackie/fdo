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
*
*/

#pragma once

#include "FDO\Expression\mgDataValue.h"

class FdoDecimalValue;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
enum class DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

interface class IExpressionProcessor;

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The DecimalValue class derives from DataValue and represents a decimal value.
public ref class DecimalValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue
{
public:
    /// \brief
    /// A cast operator to get the decimal value.
    /// 
    /// \return
    /// Returns a double
    /// 
	static operator System::Double ( NAMESPACE_OSGEO_FDO_EXPRESSION::DecimalValue^ value );

    /// \brief
    /// Constructs a default instance of an DecimalValue with a value of null.
    /// 
    /// \return
    /// Returns nothing
    /// 
	DecimalValue();

    /// \brief
    /// Constructs an instance of an DecimalValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a double
    /// 
    /// \return
    /// Returns nothing
    /// 
	DecimalValue(System::Double value);

    /// \brief
    /// Constructs an instance of a DecimalValue from another DataValue.
    /// 
    /// \param src 
    /// Input the other DataValue. Must be of one of the following types:
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
    ///
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
    /// Returns a DecimalValue, whose value is converted from the src value. 
    /// If src is an FdoBooleanValue:
    ///     false is converted to 0
    ///     true is converted to 1
    DecimalValue(
        DataValue^ src, 
        System::Boolean nullIfIncompatible,
        System::Boolean shift,
        System::Boolean truncate
    );

    /// \brief
    /// Constructs an instance of a DecimalValue from another DataValue.
    /// Equivalent to DecimalValue(src, false, true, false)
    /// 
    /// \param src 
    /// Input the other DataValue. Must be of one of the following types:
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
    /// \return
    /// Returns a DecimalValue, whose value is converted from the src value. 
    /// If src is an FdoBooleanValue:
    ///     false is converted to 0
    ///     true is converted to 1
    DecimalValue(
        DataValue^ src
    );

    /// \brief
    /// Gets the data type of the DecimalValue.
    /// 
    /// \return
    /// Returns an DataType
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::DataType DataType
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::DataType get();
    }

    /// \brief
    /// Gets the decimal value.
    /// 
    /// \return
    /// Returns a double
    /// 
    /// \brief
    /// Sets the decimal value.
    /// 
    /// \param value 
    /// Input a double
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Double Decimal
    {
        System::Double get();
        System::Void set(System::Double value);
    }

    /// \brief
    /// Overrides Expression.Process to pass the DecimalValue to the appropriate
    /// expression processor operation.
    /// 
    /// \param processor 
    /// Input an IExpressionProcessor
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor) override;

    /// \brief
    /// Returns the well defined text representation of this expression.
    /// 
    /// \return
    /// Returns a character string
    /// 
	virtual System::String^ ToString() override;

    /// \brief
    /// Constructs a DecimalValue object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	DecimalValue(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoDecimalValue* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


