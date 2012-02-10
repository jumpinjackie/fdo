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

class FdoStringValue;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
enum class DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

interface class IExpressionProcessor;

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The StringValue class derives from DataValue and represents a literal string.
public ref class StringValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue
{
public:
    /// \brief
    /// A cast operator to get the string.
    /// 
    /// \return
    /// Returns a character string
    /// 
	static operator System::String^( NAMESPACE_OSGEO_FDO_EXPRESSION::StringValue^ value );

    /// \brief
    /// Constructs a default instance of an StringValue with a value of null.
    /// 
    /// \return
    /// Returns the created StringValue
    /// 
	StringValue();

    /// \brief
    /// Constructs a default instance of an StringValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a character string
    /// 
    /// \return
    /// Returns the created StringValue
    /// 
	StringValue(System::String^ value);

    /// \brief
    /// Constructs an instance of a StringValue from another DataValue.
    /// 
    /// \param src 
    /// Input the other DataValue. Must be of one of the following types:
    ///     FdoDataType_Boolean
    ///     FdoDataType_Byte
    ///     FdoDataType_DateTime
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
    /// Returns a StringValue, whose value is converted from the src value. 
    /// If src is a StringValue then its value is simply copied to the 
    /// returned StringValue. Otherwise, the value is converted by calling
    /// src->ToString().
    ///
    StringValue(
        DataValue^ src, 
        System::Boolean nullIfIncompatible,
        System::Boolean shift,
        System::Boolean truncate
    );

    /// \brief
    /// Constructs an instance of a StringValue from another DataValue.
    /// Equivalent to StringValue(src, false, true, false)
    /// 
    /// \param src 
    /// Input the other DataValue. Must be of one of the following types:
    ///     FdoDataType_Boolean
    ///     FdoDataType_Byte
    ///     FdoDataType_DateTime
    ///     FdoDataType_Decimal
    ///     FdoDataType_Double
    ///     FdoDataType_Int16
    ///     FdoDataType_Int32
    ///     FdoDataType_Int64
    ///     FdoDataType_Single
    ///     FdoDataType_String
    ///
    /// In all other cases, the src type is considered incompatible with this type.
    /// \return
    /// Returns a StringValue, whose value is converted from the src value. 
    /// If src is a StringValue then its value is simply copied to the 
    /// returned StringValue. Otherwise, the value is converted by calling
    /// src->ToString().
    ///
    StringValue(
        DataValue^ src
    );

    /// \brief
    /// Gets the data type of the StringValue.
    /// 
    /// \return
    /// Returns DataType
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::DataType DataType
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::DataType get();
    }

    /// \brief
    /// Gets the StringValue as a string.
    /// 
    /// \return
    /// Returns a character string
    /// 
    /// \brief
    /// Sets the StringValue as a string.
    /// 
    /// \param value 
    /// Input a character string
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::String^ String
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Sets the StringValue to a null value.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void SetNull() override;

    /// \brief
    /// Overrides Expression.Process to pass the StringValue to the appropriate
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
    /// Constructs a StringValue object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	StringValue(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoStringValue* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_EXPRESSION


