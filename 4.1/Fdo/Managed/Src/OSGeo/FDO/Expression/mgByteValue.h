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

class FdoByteValue;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
enum class DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

interface class IExpressionProcessor;

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The ByteValue class derives from DataValue and represents a literal
/// byte value.
public ref class ByteValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue
{
public:
    /// \brief
    /// A cast operator to get the byte value.
    /// 
    /// \return
    /// Returns a byte
    /// 
	static operator System::Byte( NAMESPACE_OSGEO_FDO_EXPRESSION::ByteValue^ value);

    /// \brief
    /// Constructs a default instance of an ByteValue with a value of null.
    /// 
    /// \return
    /// Returns the created ByteValue
    /// 
	ByteValue();

    /// \brief
    /// Constructs a default instance of an ByteValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a byte
    /// 
    /// \return
    /// Returns the created ByteValue
    /// 
	ByteValue(System::Byte value);

    /// \brief
    /// Constructs an instance of a ByteValue from another DataValue.
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
    /// \param nullIfIncompatible 
    /// Input will determine what to do if the source value cannot be converted to 
    /// this type:
    ///     true - return NULL.
    ///     false - throw an exception
    /// 
    /// \param shift 
    /// Input determines whether non integer values can be converted:
    ///     true - convert values by rounding them.
    ///     false - behaviour depends on nullIfIncompatible:
    ///         true - return NULL.
    ///         false - throw an exception
    /// \param truncate 
    /// Input determines what to do if source value is outside the FdoByte range
    //  ( 0 to 255 ):
    ///     true - convert values less than 0 to 0, convert values greater than 255 to 255
    ///     false - behaviour depends on nullIfIncompatible:
    ///         true - return NULL.
    ///         false - throw an exception
    /// \return
    /// Returns a ByteValue, whose value is converted from the src value. 
    /// If src is a BooleanValue:
    ///     false is converted to 0
    ///     true is converted to 1
    ///
    ByteValue(
        DataValue^ src, 
        System::Boolean nullIfIncompatible,
        System::Boolean shift,
        System::Boolean truncate
    );

    /// \brief
    /// Constructs an instance of a ByteValue from another DataValue.
    /// Equivalent to ByteValue(src, false, true, false)
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
    /// Returns a BteValue, whose value is converted from the src value. 
    /// If src is a BooleanValue:
    ///     false is converted to 0
    ///     true is converted to 1
    ///
    ByteValue(
        DataValue^ src
    );

    /// \brief
    /// Gets the data type of the ByteValue.
    /// 
    /// \return
    /// Returns an DataType
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::DataType DataType
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::DataType get();
    }

    /// \brief
    /// Gets the ByteValue.
    /// 
    /// \return
    /// Returns a byte
    /// 
    /// \brief
    /// Sets the byte value.
    /// 
    /// \param value 
    /// Input a byte
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Byte Byte
    {
        System::Byte get();
        System::Void set(System::Byte value);
    }

    /// \brief
    /// Overrides Expression.Process to pass the ByteValue to the appropriate
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
    /// Returns a text string
    /// 
	virtual System::String^ ToString() override;

    /// \brief
    /// Constructs a ByteValue object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	ByteValue(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoByteValue* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


