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

class FdoDateTimeValue;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
enum class DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

interface class IExpressionProcessor;

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The DateTimeValue class derives from DataValue and represents a date or time.
public ref class DateTimeValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue
{
public:
    /// \brief
    /// A cast operator to get the date time value.
    /// 
    /// \return
    /// Returns a DateTime
    /// 
	static operator System::DateTime (NAMESPACE_OSGEO_FDO_EXPRESSION::DateTimeValue^ value);

    /// \brief
    /// Constructs a default instance of an DateTimeValue with a value of null.
    /// 
    /// \return
    /// Returns the created DateTimeValue
    /// 
	DateTimeValue();

    /// \brief
    /// Constructs a default instance of an DateTimeValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a DateTime
    /// 
    /// \return
    /// Returns the created DateTimeValue
    /// 
	DateTimeValue(System::DateTime value);

    /// \brief
    /// Constructs an instance of a DateTimeValue from another DataValue.
    /// 
    /// \param src 
    /// Input the other DataValue. Must be of one of the following types:
    ///     FdoDataType_DateTime
    ///     FdoDataType_String
    ///         - value must follow an FDO DateTime format
    ///           (e.g. "TIMESTAMP 'YYYY-MM-DD HH24:MI:SS'"
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
    /// Input for future use.
    /// \return
    /// Returns an FdoDateTimeValue, whose value is converted from the src value. 
    DateTimeValue(
        DataValue^ src, 
        System::Boolean nullIfIncompatible,
        System::Boolean shift,
        System::Boolean truncate
    );

    /// \brief
    /// Constructs an instance of a DateTimeValue from another DataValue.
    /// Equivalent to DateTimeValue(src, false, true, false)
    /// 
    /// \param src 
    /// Input the other DataValue. Must be of one of the following types:
    ///     FdoDataType_DateTime
    ///     FdoDataType_String
    ///         - value must follow an FDO DateTime format
    ///           (e.g. "TIMESTAMP 'YYYY-MM-DD HH24:MI:SS'"
    ///
    /// In all other cases, the src type is considered incompatible with this type.
    /// \return
    /// Returns an FdoDateTimeValue, whose value is converted from the src value. 
    DateTimeValue(
        DataValue^ src
    );

    /// \brief
    /// Gets the data type of the DateTimeValue.
    /// 
    /// \return
    /// Returns an DataType
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::DataType DataType
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::DataType get();
    }

    /// \brief
    /// Gets the date time value.
    /// 
    /// \return
    /// Returns a DateTime
    /// 
    /// \brief
    /// Sets the date time value.
    /// 
    /// \param value 
    /// Input a DateTime
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::DateTime DateTime
    {
        System::DateTime get();
        System::Void set(System::DateTime value);
    }

    /// \brief
    /// Overrides Expression.Process to pass the DateTimeValue to the appropriate
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
    /// Constructs a DateTimeValue object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	DateTimeValue(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoDateTimeValue* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


