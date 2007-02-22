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
public __value enum DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

public __gc __interface IExpressionProcessor;

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The ByteValue class derives from DataValue and represents a literal
/// byte value.
public __gc class ByteValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue
{
public:
    /// \brief
    /// A cast operator to get the byte value.
    /// 
    /// \return
    /// Returns a byte
    /// 
	static System::Byte op_Explicit( NAMESPACE_OSGEO_FDO_EXPRESSION::ByteValue* value);

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
    /// Gets the data type of the ByteValue.
    /// 
    /// \return
    /// Returns an DataType
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_DataType();

    /// \brief
    /// Gets the ByteValue.
    /// 
    /// \return
    /// Returns a byte
    /// 
	__property System::Byte get_Byte();

    /// \brief
    /// Sets the byte value.
    /// 
    /// \param value 
    /// Input a byte
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Byte(System::Byte value);

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
	System::Void Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor* processor);

    /// \brief
    /// Returns the well defined text representation of this expression.
    /// 
    /// \return
    /// Returns a text string
    /// 
	System::String* ToString();

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

public private:
	inline FdoByteValue* GetImpObj();
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


