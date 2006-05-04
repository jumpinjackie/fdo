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

#include "FDO\Expression\mgLiteralValue.h"
#include "FDO\Schema\mgDataType.h"

class FdoDataValue;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

/// \brief
/// The FdoDataValue class derives from FdoLiteralValue and represents a literal
/// value such as a string or a number.
public __gc class DataValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValue
{
public:
    /// \brief
    /// Constructs an instance of a null FdoDataValue using the specified arguments.
    /// 
    /// \param dataType 
    /// Input data type
    /// 
    /// \return
    /// Returns nothing
    /// 
	DataValue(NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType);

    /// \brief
    /// Constructs an instance of an FdoBooleanValue using the specified argument.
    /// 
    /// \param value 
    /// Input a Boolean value
    /// 
    /// \return
    /// Returns an FdoBooleanValue
    /// 
	DataValue(System::Boolean value);

    /// \brief
    /// Constructs an instance of an FdoByteValue using the specified argument.
    /// 
    /// \param value 
    /// Input a byte
    /// 
    /// \return
    /// Returns an FdoByteValue
    /// 
	DataValue(System::Byte value);

    /// \brief
    /// Constructs an instance of an FdoDateTimeValue using the specified argument.
    /// 
    /// \param value 
    /// Input a FdoDateTime
    /// 
    /// \return
    /// Returns an FdoDateTimeValue
    /// 
	DataValue(System::DateTime value);

    /// \brief
    /// Constructs an instance of an FdoDecimalValue or FdoDoubleValue using 
    /// the specified arguments.
    /// 
    /// \param value 
    /// Input a double
    /// \param dataType 
    /// Input a data type
    /// 
    /// \return
    /// Returns an FdoDecimalValue or FdoDoubleValue
    /// 
	DataValue(System::Double value, NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType);

    /// \brief
    /// Constructs an instance of an FdoInt16Value using the specified argument.
    /// 
    /// \param value 
    /// Input a 16 bit integer
    /// 
    /// \return
    /// Returns an FdoInt16Value
    /// 
	DataValue(System::Int16 value);

    /// \brief
    /// Constructs an instance of an FdoInt32Value using the specified argument.
    /// 
    /// \param value 
    /// Input a 32 bit integer
    /// 
    /// \return
    /// Returns an FdoInt32Value
    /// 
	DataValue(System::Int32 value);

    /// \brief
    /// Constructs an instance of an FdoInt64Value using the specified argument.
    /// 
    /// \param value 
    /// Input a 64 bit integer
    /// 
    /// \return
    /// Returns an FdoInt64Value
    /// 
	DataValue(System::Int64 value);

    /// \brief
    /// Constructs an instance of an FdoSingleValue using the specified argument.
    /// 
    /// \param value 
    /// Input a single precision floating point value
    /// 
    /// \return
    /// Returns an FdoSingleValue
    /// 
	DataValue(System::Single value);

    /// \brief
    /// Constructs an instance of an FdoStringValue using the specified argument.
    /// 
    /// \param value 
    /// Input a character string
    /// 
    /// \return
    /// Returns an FdoStringValue
    /// 
	DataValue(System::String* value);

    /// \brief
    /// Constructs an instance of an FdoBLOBValue or FdoCLOBValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a byte array
    /// \param length 
    /// Input the length of the byte array
    /// \param dataType 
    /// Input the type of value to create
    /// 
    /// \return
    /// Returns an FdoBLOBValue or an FdoCLOBValue
    /// 
	DataValue(System::Byte value [], NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType);
	
    /// \brief
    /// Gets the data type of the FdoDataValue.
    /// 
    /// \return
    /// Returns an FdoDataType
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_DataType();

    /// \brief
    /// Returns true if the FdoDataValue represents a null value.
    /// 
    /// \return
    /// Returns true if the FdoDataValue represents a null value
    /// 
	System::Boolean IsNull();

    /// \brief
    /// Sets the FdoDataValue to a null value of the specified type.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void SetNull();

	DataValue(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoDataValue* GetImpObj();
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


