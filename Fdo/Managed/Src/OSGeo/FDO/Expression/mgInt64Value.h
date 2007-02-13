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

class FdoInt64Value;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __value enum DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

public __gc __interface IExpressionProcessor;

/// \brief
/// The Int64Value class derives from DataValue and represents a 64-bit signed integer value.
public __gc class Int64Value : public NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue
{
public:
    /// \brief
    /// A cast operator to get the integer value.
    /// 
    /// \return
    /// Returns a 64 bit integer
    /// 
	static System::Int64 op_Explicit( NAMESPACE_OSGEO_FDO_EXPRESSION::Int64Value* value );

    /// \brief
    /// Constructs a default instance of an Int64Value with a value of null.
    /// 
    /// \return
    /// Returns the created Int64Value
    /// 
	Int64Value();

    /// \brief
    /// Constructs a default instance of an Int64Value using the specified arguments.
    /// 
    /// \param value 
    /// Input a 64 bit integer
    /// 
    /// \return
    /// Returns the created Int64Value
    /// 
	Int64Value(System::Int64 value);

    /// \brief
    /// Gets the data type of the Int64Value.
    /// 
    /// \return
    /// Returns an DataType
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_DataType();

    /// \brief
    /// Gets the Int64Value.
    /// 
    /// \return
    /// Returns a 64 bit integer
    /// 
	__property System::Int64 get_Int64();

    /// \brief
    /// Sets the Int64Value.
    /// 
    /// \param value 
    /// Input a 64 bit integer
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Int64(System::Int64 value);

    /// \brief
    /// Overrides Expression.Process to pass the Int64Value to the appropriate
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
    /// Constructs an Int64Value object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	Int64Value(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoInt64Value* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_EXPRESSION


