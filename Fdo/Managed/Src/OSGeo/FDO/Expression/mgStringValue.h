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
public __value enum DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

public __gc __interface IExpressionProcessor;

/// \brief
/// The StringValue class derives from DataValue and represents a literal string.
public __gc class StringValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue
{
public:
    /// \brief
    /// A cast operator to get the string.
    /// 
    /// \return
    /// Returns a character string
    /// 
	static System::String* op_Explicit( NAMESPACE_OSGEO_FDO_EXPRESSION::StringValue* value );

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
	StringValue(System::String* value);

    /// \brief
    /// Gets the data type of the StringValue.
    /// 
    /// \return
    /// Returns DataType
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_DataType();

    /// \brief
    /// Gets the StringValue as a string.
    /// 
    /// \return
    /// Returns a character string
    /// 
	__property System::String* get_String();

    /// \brief
    /// Sets the StringValue as a string.
    /// 
    /// \param value 
    /// Input a character string
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_String(System::String* value);

    /// \brief
    /// Sets the StringValue to a null value.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void SetNull();

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
	System::Void Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor* processor);

    /// \brief
    /// Returns the well defined text representation of this expression.
    /// 
    /// \return
    /// Returns a character string
    /// 
	System::String* ToString();

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

public private:
	inline FdoStringValue* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_EXPRESSION


