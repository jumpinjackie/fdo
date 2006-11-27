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

#pragma  once

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA

/// \brief
/// The FdoDataType enumeration lists all of the base data types supported by FDO.
/// \param FdoDataType_Boolean
/// Represents a Boolean value of true or false
/// \param FdoDataType_Byte
/// Represents unsigned 8-bit integers with values between 0 and 255
/// \param FdoDataType_DateTime
/// Represents a date and time value
/// \param FdoDataType_Decimal
/// Represents values ranging from 1.0 x 10^-28 to approximately 7.9 x 10^28, 
/// with 28-29 significant digits
/// \param FdoDataType_Double
/// Represents a floating point value ranging from approximately 5.0 
/// x 10^-324 to 1.7 x 10^308 with a precision of 15-16 digits
/// \param FdoDataType_Int16
/// Represents signed 16-bit integers with values between -32768 and 32767
/// \param FdoDataType_Int32
/// Represents signed 32-bit integers with values between -2147483648 and 2147483647
/// \param FdoDataType_Int64
/// Represents signed 64-bit integers with values between -9223372036854775808 and 9223372036854775807
/// \param FdoDataType_Single
/// Represents floating point values ranging from approximately 1.5 x 10^-45
/// to 3.4 x 10^38, with a precision of 7 digits
/// \param FdoDataType_String
/// Represents a Unicode character string
/// \param FdoDataType_BLOB
/// Represents a binary large object stored as a collection of bytes
/// \param FdoDataType_CLOB
/// Represents a character large object stored as a collection of
/// characters
public __value enum DataType
{
    /// Represents a Boolean value of true or false.
	DataType_Boolean = FdoDataType_Boolean,

    /// Represents unsigned 8-bit integers with values between 0 and 255.
	DataType_Byte = FdoDataType_Byte,

    /// Represents a date and time value.
	DataType_DateTime = FdoDataType_DateTime,

    /// Represents values ranging from 1.0 x 10^-28 to approximately 7.9 x 10^28
    /// with 28-29 significant digits.
	DataType_Decimal = FdoDataType_Decimal,

    /// Represents a floating point value ranging from approximately 5.0 
    /// x 10^-324 to 1.7 x 10^308 with a precision of 15-16 digits.
	DataType_Double = FdoDataType_Double,

    /// Represents signed 16-bit integers with values between -32768 and 32767.
	DataType_Int16 = FdoDataType_Int16,

    /// Represents signed 32-bit integers with values between -2147483648 
    /// and 2147483647.
	DataType_Int32 = FdoDataType_Int32,

    /// Represents signed 64-bit integers with values 
    /// betwenn -9223372036854775808 and 9223372036854775807.
	DataType_Int64 = FdoDataType_Int64,

    /// Represents floating point values ranging from approximately 1.5 x 10^-45
    /// to 3.4 x 10^38, with a precision of 7 digits. 
	DataType_Single = FdoDataType_Single,

    /// Represents a Unicode character strings.
	DataType_String = FdoDataType_String,

    /// Represents a binary large object stored as a collection of bytes.
	DataType_BLOB = FdoDataType_BLOB,

    /// Represents a character large object stored as a collection of
    /// characters.
	DataType_CLOB = FdoDataType_CLOB,
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


