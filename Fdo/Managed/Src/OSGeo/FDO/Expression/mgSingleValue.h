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

class FdoSingleValue;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
enum class DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA 

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

interface class IExpressionProcessor;

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The SingleValue class derives from DataValue and represents a single precision floating point number.
public ref class SingleValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue
{
public:
    /// \brief
    /// A cast operator to get the floating point value.
    /// 
    /// \return
    /// Returns a single precision floating point value
    /// 
	static operator System::Single( NAMESPACE_OSGEO_FDO_EXPRESSION::SingleValue^ value );

    /// \brief
    /// Constructs a default instance of an SingleValue with a value of null.
    /// 
    /// \return
    /// Returns the created SingleValue
    /// 
	SingleValue();

    /// \brief
    /// Constructs a default instance of an SingleValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a single precision floating point value
    /// 
    /// \return
    /// Returns the created SingleValue
    /// 
	SingleValue(System::Single value);

    /// \brief
    /// Constructs an instance of a SingleValue from another DataValue.
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
    /// Input determines whether FdoInt32 or FdoInt64 values are allowed to shift 
    /// when they have more precision that can be handled by a float.
    ///     true - convert values allowing them to shift.
    ///     false - behaviour depends on nullIfIncompatible:
    ///         true - return NULL.
    ///         false - throw an exception
    /// \param truncate 
    /// Input determines what to do if source value is outside the FdoFloat range
    ///  ( -3.4e38, 3.4e38 ):
    ///     true - convert values less than -3.4e38 to -3.4e38, convert values greater than 3.4e38 to 3.4e38
    ///     false - behaviour depends on nullIfIncompatible:
    ///         true - return NULL.
    ///         false - throw an exception
    /// \return
    /// Returns a SingleValue, whose value is converted from the src value. 
    /// If src is a BooleanValue:
    ///     false is converted to 0
    ///     true is converted to 1
    ///
    SingleValue(
        DataValue^ src, 
        System::Boolean nullIfIncompatible,
        System::Boolean shift,
        System::Boolean truncate
    );

    /// \brief
    /// Constructs an instance of a SingleValue from another DataValue.
    /// Equivalent to SingleValue(src, false, true, false)
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
    /// Returns a SingleValue, whose value is converted from the src value. 
    /// If src is a BooleanValue:
    ///     false is converted to 0
    ///     true is converted to 1
    ///
    SingleValue(
        DataValue^ src
    );

    /// \brief
    /// Gets the data type of the SingleValue.
    /// 
    /// \return
    /// Returns an DataType
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::DataType DataType
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::DataType get();
    }

    /// \brief
    /// Gets the SingleValue as a single precision floating point number.
    /// 
    /// \return
    /// Returns a single precision floating point value
    /// 
    /// \brief
    /// Sets the SingleValue as a single precision floating point number.
    /// 
    /// \param value 
    /// Input a single precision floating point value
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Single Single
    {
        System::Single get();
        System::Void set(System::Single value);
    }

    /// \brief
    /// Overrides Expression.Process to pass the SingleValue to the appropriate
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
    /// Constructs a SingleValue object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	SingleValue(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoSingleValue* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_EXPRESSION


