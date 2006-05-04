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

class FdoInt16Value;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __value enum DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

public __gc __interface IExpressionProcessor;

/// \brief
/// The FdoInt16Value class derives from FdoDataValue and represents a 16-bit signed integer value.
public __gc class Int16Value : public NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue
{
public:
    /// \brief
    /// A cast operator to get the integer value.
    /// 
    /// \return
    /// Returns a 16 bit integer
    /// 
	static System::Int16 op_Explicit( NAMESPACE_OSGEO_FDO_EXPRESSION::Int16Value* value );

    /// \brief
    /// Constructs a default instance of an FdoInt16Value with a value of null.
    /// 
    /// \return
    /// Returns the created FdoInt16Value
    /// 
	Int16Value();

    /// \brief
    /// Constructs a default instance of an FdoInt16Value using the specified arguments.
    /// 
    /// \param value 
    /// Input a 16 bit integer
    /// 
    /// \return
    /// Returns the created FdoInt16Value
    /// 
	Int16Value(System::Int16 value);

    /// \brief
    /// Gets the data type of the FdoInt16Value.
    /// 
    /// \return
    /// Returns an FdoDataType
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_DataType();

    /// \brief
    /// Gets the FdoInt16Value.
    /// 
    /// \return
    /// Returns a 16 bit integer
    /// 
	__property System::Int16 get_Int16();

    /// \brief
    /// Sets the FdoInt16Value.
    /// 
    /// \param value 
    /// Input a 16 bit integer
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Int16(System::Int16 value);

    /// \brief
    /// Overrides FdoExpression.Process to pass the FdoInt16Value to the appropriate
    /// expression processor operation.
    /// 
    /// \param processor 
    /// Input an FdoIExpressionProcessor
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

	Int16Value(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoInt16Value* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_EXPRESSION


