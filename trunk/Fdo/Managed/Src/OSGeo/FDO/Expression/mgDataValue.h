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
public ref class DataValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValue
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
	DataValue(System::String^ value);

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
	DataValue(array<System::Byte>^ value, NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType);
	
    /// \brief
    /// Constructs an instance of an DataValue from another DataValue.
    /// 
    /// \param dataType
    /// Input the destination type. A DataValue of this type is created. 
    /// For more information on how the DataValue is created, see the specific 
    /// constructor for each dataType. For example, if dataType=FdoDataType_Int16, see
    /// Int16Value(DataValue^, Boolean, Boolean, Boolean)
    /// \param src 
    /// Input the source (other) DataValue
    /// \param nullIfIncompatible 
    /// Input will determine what to do if source value type is not compatible with the 
    /// this type:
    ///     true - return NULL.
    ///     false - throw an exception
    /// 
    /// \param shift 
    /// Input determines what to do if the src value is within the range of 
    /// valid values for the destination type but still must be shifted to be a 
    /// valid destination type value (e.g. 3.5 must be shifted to convert it from
    /// DoubleValue to Int32Value):
    ///     true - perform the shift.
    ///     false - behaviour depends on nullIfIncompatible:
    ///         true - return NULL.
    ///         false - throw an exception
    /// 
    /// \param truncate 
    /// Input determines what to do if source value is outside the
    ///  range of valid values for the destination type:
    ///     true - convert values less than the minimum to the minimum, 
    ///            convert values greater than maximum to the maximum
    ///     false - behaviour depends on nullIfIncompatible:
    ///         true - return NULL.
    ///         false - throw an exception
    /// \return
    /// Returns a DataValue, whose value is converted from the src value. 
    DataValue(
        NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType,
        DataValue^ src, 
        System::Boolean nullIfIncompatible,
        System::Boolean shift,
        System::Boolean truncate
    );

    /// \brief
    /// Constructs an instance of an DataValue from another DataValue.
    /// Equivalent to DataValue(dataType, src, false, true, false).
    /// 
    /// \param dataType
    /// Input the destination type. A DataValue of this type is created. 
    /// For more information on how the  DataValue is created, see the specific 
    /// constructor for each dataType. For example, if dataType=FdoDataType_Int16, see
    /// Int16Value(DataValue^, Boolean, Boolean, Boolean)
    /// \param src 
    /// Input the source (other) DataValue
    /// \return
    /// Returns a DataValue, whose value is converted from the src value. 
    DataValue(
        NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType,
        DataValue^ src
    );

    /// \brief
    /// Gets the data type of the DataValue.
    /// 
    /// \return
    /// Returns an DataType
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::DataType DataType
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::DataType get();
    }

    /// \brief
    /// Returns true if the DataValue represents a null value.
    /// 
    /// \return
    /// Returns true if the DataValue represents a null value
    /// 
	virtual System::Boolean IsNull();

    /// \brief
    /// Sets the DataValue to a null value of the specified type.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void SetNull();

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
    property NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValueType LiteralValueType
    {
        NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValueType get();
    }

internal:
	inline FdoDataValue* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


