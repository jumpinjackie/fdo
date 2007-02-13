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

class FdoInt32Value;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __value enum DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

public __gc __interface IExpressionProcessor;

/// \brief
/// The Int32Value class derives from DataValue and represents a 32-bit signed integer value.
public __gc class Int32Value : public NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue
{
public:
    /// \brief
    /// A cast operator to get the integer value.
    /// 
    /// \return
    /// Returns a 32 bit integer
    /// 
	static System::Int32 op_Explicit( NAMESPACE_OSGEO_FDO_EXPRESSION::Int32Value* value );

    /// \brief
    /// Constructs a default instance of an Int32Value with a value of null.
    /// 
    /// \return
    /// Returns nothing
    /// 
	Int32Value();

    /// \brief
    /// Constructs a default instance of an Int32Value using the specified arguments.
    /// 
    /// \param value 
    /// Input a 32 bit integer
    /// 
    /// \return
    /// Returns nothing
    /// 
	Int32Value(System::Int32 value);

    /// \brief
    /// Gets the data type of the Int32Value.
    /// 
    /// \return
    /// Returns an DataType
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_DataType();

    /// \brief
    /// Gets the Int32Value.
    /// 
    /// \return
    /// Returns a 32 bit integer
    /// 
	__property System::Int32 get_Int32();

    /// \brief
    /// Sets the Int32Value.
    /// 
    /// \param value 
    /// Input a 32 bit integer
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Int32(System::Int32 value);

    /// \brief
    /// Overrides Expression.Process to pass the Int32Value to the appropriate
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
    /// Constructs an Int32Value object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	Int32Value(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoInt32Value* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_EXPRESSION


