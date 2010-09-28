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

#include "FDO\Expression\mgLOBValue.h"

class FdoCLOBValue;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
enum class DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

interface class IExpressionProcessor;

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The CLOBValue class derives from LOBValue and represents a literal
/// CLOB value.
public ref class CLOBValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue 
{
public:
    /// \brief
    /// Constructs a default instance of an CLOBValue with a value of null.
    /// 
    /// \return
    /// Returns the created CLOBValue
    /// 
	CLOBValue();

    /// \brief
    /// Constructs a default instance of an CLOBValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a byte array
    /// 
    /// \return
    /// Returns the created CLOBValue
    /// 
	CLOBValue(array<System::Byte>^ value);

    /// \brief
    /// Gets the data type of the CLOBValue.
    /// 
    /// \return
    /// Returns an DataType
    /// 
	property NAMESPACE_OSGEO_FDO_SCHEMA::DataType DataType
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::DataType get();
    }

    /// \brief
    /// Gets the CLOB value.
    /// 
    /// \return
    /// Returns a byte array
    /// 
    /// \brief
    /// Sets the CLOB value.
    /// 
    /// \param value 
    /// Input a byte array
    /// 
    /// \return
    /// Returns nothing
    /// 
    property array<System::Byte>^ Data
    {
        array<System::Byte>^ get();
        System::Void set(array<System::Byte>^ value);
    }

    /// \brief
    /// Sets the CLOB value to a null value.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void SetNull() override;

    /// \brief
    /// Overrides Expression.Process to pass the CLOBValue to the appropriate
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
    /// Returns a text string
    /// 
	virtual System::String^ ToString() override;

    /// \brief
    /// Constructs a CLOBValue object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	CLOBValue(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoCLOBValue* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


