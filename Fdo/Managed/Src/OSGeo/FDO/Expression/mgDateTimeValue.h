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
public __value enum DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

public __gc __interface IExpressionProcessor;

/// \brief
/// The DateTimeValue class derives from DataValue and represents a date or time.
public __gc class DateTimeValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue
{
public:
    /// \brief
    /// A cast operator to get the date time value.
    /// 
    /// \return
    /// Returns a DateTime
    /// 
	static System::DateTime op_Explicit(NAMESPACE_OSGEO_FDO_EXPRESSION::DateTimeValue* value);

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
    /// Gets the data type of the DateTimeValue.
    /// 
    /// \return
    /// Returns an DataType
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_DataType();

    /// \brief
    /// Gets the date time value.
    /// 
    /// \return
    /// Returns a DateTime
    /// 
	__property System::DateTime get_DateTime();

    /// \brief
    /// Sets the date time value.
    /// 
    /// \param value 
    /// Input a DateTime
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_DateTime(System::DateTime value);

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
	System::Void Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor* processor);

    /// \brief
    /// Returns the well defined text representation of this expression.
    /// 
    /// \return
    /// Returns a character string
    /// 
	System::String* ToString();

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

public private:
	inline FdoDateTimeValue* GetImpObj();
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


