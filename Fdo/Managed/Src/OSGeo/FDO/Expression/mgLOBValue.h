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
public __value enum DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

public __gc __interface IExpressionProcessor;

/// \brief
/// The LOBValue class derives from DataValue and represents a literal
/// BLOB value.
public __gc class LOBValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue
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
	static System::Byte op_Explicit( NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue* value ) [];

    /// \brief
    /// Gets the data type of the LOBValue.
    /// 
    /// \return
    /// Returns an DataType
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_DataType();

    /// \brief
    /// Gets the LOB data.
    /// 
    /// \return
    /// Returns a byte array
    /// 
	__property System::Byte get_Data() [];

    /// \brief
    /// Sets the LOB data.
    /// 
    /// \param value 
    /// Input an array of bytes
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Data(System::Byte value []);

    /// \brief
    /// Sets the LOBValue to a null value.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void SetNull();

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
	System::Void Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor* processor);

    /// \brief
    /// Returns the well defined text representation of this expression.
    /// 
    /// \return
    /// Returns a text string
    /// 
	System::String* ToString(); 

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

public private:
	inline FdoLOBValue* GetImpObj();
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


