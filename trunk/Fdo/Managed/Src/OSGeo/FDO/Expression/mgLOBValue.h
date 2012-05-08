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

class FdoLOBValue;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
enum class DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

interface class IExpressionProcessor;

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The LOBValue class derives from DataValue and represents a literal
/// BLOB value.
public ref class LOBValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue
{
public:
    /// \brief
    /// Transforms the FDO LOB value into a byte array.
    /// 
    /// \param value 
    /// Input an FDO LOBValue object
    /// 
    /// \return
    /// Returns a byte array
    /// 
	static operator array<System::Byte>^(NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue^ value);

    /// \brief
    /// Gets the data type of the LOBValue.
    /// 
    /// \return
    /// Returns an DataType
    /// 
	property NAMESPACE_OSGEO_FDO_SCHEMA::DataType DataType
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::DataType get();
    }

    /// \brief
    /// Gets the LOB data.
    /// 
    /// \return
    /// Returns a byte array
    /// 
    /// \brief
    /// Sets the LOB data.
    /// 
    /// \param value 
    /// Input an array of bytes
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
    /// Sets the LOBValue to a null value.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void SetNull() override;

    /// \brief
    /// Overrides Expression.Process to pass the LOBValue to the appropriate
    /// expression processor operation.
    /// 
    /// \param processor 
    /// Input the IExpressionProcessor
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
    /// Constructs a LOBValue object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	LOBValue(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoLOBValue* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


