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

class FdoBooleanValue;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
enum class DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

interface class IExpressionProcessor;

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The BooleanValue class derives from DataValue and represents a Boolean
/// value.
public ref class BooleanValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue
{
public:
    /// \brief
    /// A cast operator to get the Boolean value.
    /// 
	static operator System::Boolean ( NAMESPACE_OSGEO_FDO_EXPRESSION::BooleanValue^ value);

    /// \brief
    /// Constructs a default instance of an BooleanValue with a value of null.
    /// 
    /// \return
    /// Returns the created BooleanValue
    /// 
	BooleanValue();

    /// \brief
    /// Constructs a default instance of an BooleanValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a Boolean value
    /// 
    /// \return
    /// Returns the created BooleanValue
    /// 
	BooleanValue(System::Boolean value);

    /// \brief
    /// Constructs an instance of a BooleanValue from another DataValue.
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
    /// Input determines what to do if source value is numeric but
    /// not 0 or 1:
    ///     true - set the FdoBooleanValue to false if source value is 0, true otherwise.
    ///     false - behaviour depends on nullIfIncompatible:
    ///         true - return NULL.
    ///         false - throw an exception
    /// \return
    /// Returns a BooleanValue, whose value is converted from the src value. 
    /// If src value is numeric then:
    ///     0 is converted to false
    ///     1 is converted to true
    BooleanValue(
        DataValue^ src, 
        System::Boolean nullIfIncompatible,
        System::Boolean shift,
        System::Boolean truncate
    );

    /// \brief
    /// Constructs an instance of a BooleanValue from another DataValue.
    /// Equivalent to BooleanValue(src, false, true, false)
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
    ///         - value must be "TRUE", "FALSE", or numeric.
    ///
    /// In all other cases, the src type is considered incompatible with this type.
    /// \return
    /// Returns a BooleanValue, whose value is converted from the src value. 
    /// If src value is numeric then:
    ///     0 is converted to false
    ///     1 is converted to true
    BooleanValue(
        DataValue^ src
    );

    /// \brief
    /// Gets the data type of the BooleanValue.
    /// 
    /// \return
    /// Returns an DataType
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::DataType DataType
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::DataType get();
    }

    /// \brief
    /// Gets the BooleanValue.
    /// 
    /// \return
    /// Returns a Boolean value
    /// 
    /// \brief
    /// Sets the Boolean value.
    /// 
    /// \param value 
    /// Input a Boolean value
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Boolean Boolean
    {
        System::Boolean get();
        System::Void set(System::Boolean value);
    }

    /// \brief
    ///  Overrides Expression.Process to pass the BooleanValue to the appropriate
    /// expression processor operation.
    /// 
    /// \param processor 
    /// Input the IExpressionProcessor
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor) override;

    /// \brief
    /// Returns the well defined text representation of this expression.
    /// 
    /// \return
    /// Returns a character string.
    /// 
	virtual System::String^ ToString() override;

    /// \brief
    /// Constructs a BooleanValue object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	BooleanValue(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoBooleanValue* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


