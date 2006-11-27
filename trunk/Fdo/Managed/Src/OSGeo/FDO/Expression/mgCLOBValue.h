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
public __value enum DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

public __gc __interface IExpressionProcessor;

/// \brief
/// The FdoCLOBValue class derives from FdoLOBValue and represents a literal
/// CLOB value.
public __gc class CLOBValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue 
{
public:
    /// \brief
    /// Constructs a default instance of an FdoCLOBValue with a value of null.
    /// 
    /// \return
    /// Returns the created FdoCLOBValue
    /// 
	CLOBValue();

    /// \brief
    /// Constructs a default instance of an FdoCLOBValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a byte array
    /// 
    /// \return
    /// Returns the created FdoCLOBValue
    /// 
	CLOBValue(System::Byte value []);

    /// \brief
    /// Gets the data type of the FdoCLOBValue.
    /// 
    /// \return
    /// Returns an FdoDataType
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_DataType();

    /// \brief
    /// Gets the CLOB value.
    /// 
    /// \return
    /// Returns a byte array
    /// 
	__property System::Byte get_Data() [];

    /// \brief
    /// Sets the CLOB value.
    /// 
    /// \param value 
    /// Input a byte array
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Data(System::Byte value []);

    /// \brief
    /// Sets the CLOB value to a null value.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void SetNull();

    /// \brief
    /// Overrides FdoExpression.Process to pass the FdoCLOBValue to the appropriate
    /// expression processor operation.
    /// 
    /// \param processor 
    /// Input an FdoIExpressionProcessor
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

	CLOBValue(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoCLOBValue* GetImpObj();
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


