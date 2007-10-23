/*
* Copyright (C) 2004-2007  Autodesk, Inc.
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

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The DataValue class derives from LiteralValue and represents a literal
/// value such as a string or a number.
public __gc class DataValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValue
{
public:
    /// \brief
    /// Constructs an instance of a null DataValue using the specified arguments.
    /// 
    /// \param dataType 
    /// Input data type
    /// 
    /// \return
    /// Returns nothing
    /// 
	DataValue(NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType);

    /// \brief
    /// Constructs an instance of an BooleanValue using the specified argument.
    /// 
    /// \param value 
    /// Input a Boolean value
    /// 
    /// \return
    /// Returns an BooleanValue
    /// 
	DataValue(System::Boolean value);

    /// \brief
    /// Constructs an instance of an ByteValue using the specified argument.
    /// 
    /// \param value 
    /// Input a byte
    /// 
    /// \return
    /// Returns an ByteValue
    /// 
	DataValue(System::Byte value);

    /// \brief
    /// Constructs an instance of an DateTimeValue using the specified argument.
    /// 
    /// \param value 
    /// Input a DateTime
    /// 
    /// \return
    /// Returns an DateTimeValue
    /// 
	DataValue(System::DateTime value);

    /// \brief
    /// Constructs an instance of an DecimalValue or DoubleValue using 
    /// the specified arguments.
    /// 
    /// \param value 
    /// Input a double
    /// \param dataType 
    /// Input a data type
    /// 
    /// \return
    /// Returns an DecimalValue or DoubleValue
    /// 
	DataValue(System::Double value, NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType);

    /// \brief
    /// Constructs an instance of an Int16Value using the specified argument.
    /// 
    /// \param value 
    /// Input a 16 bit integer
    /// 
    /// \return
    /// Returns an Int16Value
    /// 
	DataValue(System::Int16 value);

    /// \brief
    /// Constructs an instance of an Int32Value using the specified argument.
    /// 
    /// \param value 
    /// Input a 32 bit integer
    /// 
    /// \return
    /// Returns an Int32Value
    /// 
	DataValue(System::Int32 value);

    /// \brief
    /// Constructs an instance of an Int64Value using the specified argument.
    /// 
    /// \param value 
    /// Input a 64 bit integer
    /// 
    /// \return
    /// Returns an Int64Value
    /// 
	DataValue(System::Int64 value);

    /// \brief
    /// Constructs an instance of an SingleValue using the specified argument.
    /// 
    /// \param value 
    /// Input a single precision floating point value
    /// 
    /// \return
    /// Returns an SingleValue
    /// 
	DataValue(System::Single value);

    /// \brief
    /// Constructs an instance of an StringValue using the specified argument.
    /// 
    /// \param value 
    /// Input a character string
    /// 
    /// \return
    /// Returns an StringValue
    /// 
	DataValue(System::String* value);

    /// \brief
    /// Constructs an instance of an BLOBValue or CLOBValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a byte array
    ///
    /// \param dataType 
    /// Input the type of value to create
    /// 
    /// \return
    /// Returns an BLOBValue or an CLOBValue
    /// 
	DataValue(System::Byte value [], NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType);
	
    /// \brief
    /// Gets the data type of the DataValue.
    /// 
    /// \return
    /// Returns an DataType
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_DataType();

    /// \brief
    /// Returns true if the DataValue represents a null value.
    /// 
    /// \return
    /// Returns true if the DataValue represents a null value
    /// 
	System::Boolean IsNull();

    /// \brief
    /// Sets the DataValue to a null value of the specified type.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void SetNull();

    /// \brief
    /// Constructs a DataValue object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	DataValue(System::IntPtr unmanaged, System::Boolean autoDelete);

    /// \brief
    /// Gets the literal value type of the DataValue.
    /// 
    /// \return
    /// Returns an LiteralValueType
    /// 
    __property NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValueType get_LiteralValueType ();

public private:
	inline FdoDataValue* GetImpObj();
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


