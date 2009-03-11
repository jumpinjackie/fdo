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

class FdoDoubleValue;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
enum class DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

interface class IExpressionProcessor;

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The DoubleValue class derives from DataValue and represents a double-precision floating point number.
public ref class DoubleValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue
{
public:
    /// \brief
    /// A cast operator to get the double value.
    /// 
    /// \return
    /// Returns a double
    /// 
	static operator System::Double ( NAMESPACE_OSGEO_FDO_EXPRESSION::DoubleValue^ value );

    /// \brief
    /// Constructs a default instance of an DoubleValue with a value of null.
    /// 
    /// \return
    /// Returns the created DoubleValue
    /// 
	DoubleValue();

    /// \brief
    /// Constructs a default instance of an DoubleValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a double
    /// 
    /// \return
    /// Returns the created DoubleValue
    /// 
	DoubleValue(System::Double value);

    /// \brief
    /// Gets the data type of the DoubleValue.
    /// 
    /// \return
    /// Returns an DataType
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::DataType DataType
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::DataType get();
    }

    /// \brief
    /// Gets the double value.
    /// 
    /// \return
    /// Returns a double
    /// 
    /// \brief
    /// Sets the double value as a double precision floating point number.
    /// 
    /// \param value 
    /// Input a double
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Double Double
    {
        System::Double get();
        System::Void set(System::Double value);
    }

    /// \brief
    /// Overrides Expression.Process to pass the DoubleValue to the appropriate
    /// expression processor operation.
    /// 
    /// \param processor 
    /// Input an IExpressionProcessor
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor^ processor) override;

    /// \brief
    /// Returns the well defined text representation of this expression.
    /// 
    /// \return
    /// Returns a character string
    /// 
	virtual System::String^ ToString() override;

    /// \brief
    /// Constructs a DoubleValue object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	DoubleValue(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoDoubleValue* GetImpObj();
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


