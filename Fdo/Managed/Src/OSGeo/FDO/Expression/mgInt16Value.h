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
enum class DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

interface class IExpressionProcessor;

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The Int16Value class derives from DataValue and represents a 16-bit signed integer value.
public ref class Int16Value : public NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue
{
public:
    /// \brief
    /// A cast operator to get the integer value.
    /// 
    /// \return
    /// Returns a 16 bit integer
    /// 
	static operator System::Int16( NAMESPACE_OSGEO_FDO_EXPRESSION::Int16Value^ value );

    /// \brief
    /// Constructs a default instance of an Int16Value with a value of null.
    /// 
    /// \return
    /// Returns the created Int16Value
    /// 
	Int16Value();

    /// \brief
    /// Constructs a default instance of an Int16Value using the specified arguments.
    /// 
    /// \param value 
    /// Input a 16 bit integer
    /// 
    /// \return
    /// Returns the created Int16Value
    /// 
	Int16Value(System::Int16 value);

    /// \brief
    /// Gets the data type of the Int16Value.
    /// 
    /// \return
    /// Returns an DataType
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::DataType DataType
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::DataType get();
    }

    /// \brief
    /// Gets the Int16Value.
    /// 
    /// \return
    /// Returns a 16 bit integer
    /// 
    /// \brief
    /// Sets the Int16Value.
    /// 
    /// \param value 
    /// Input a 16 bit integer
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::Int16 Int16
    {
        System::Int16 get();
        System::Void set(System::Int16 value);
    }

    /// \brief
    /// Overrides Expression.Process to pass the Int16Value to the appropriate
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
    /// Constructs an Int16Value object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	Int16Value(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoInt16Value* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_EXPRESSION


