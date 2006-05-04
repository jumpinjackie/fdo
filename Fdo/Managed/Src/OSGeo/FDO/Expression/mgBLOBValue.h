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

class FdoBLOBValue;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __value enum DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA 

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

public __gc __interface IExpressionProcessor;

/// \brief
/// The FdoBLOBValue class derives from FdoLOBValue and represents a literal
/// BLOB value.
public __gc class BLOBValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue 
{
public:
    /// \brief
    /// Constructs a default instance of an FdoBLOBValue with a value of null.
    /// 
    /// \return
    /// Returns the created FdoBLOBValue
    /// 
	BLOBValue();

    /// \brief
    /// Constructs an instance of an FdoBLOBValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a byte array
    /// 
    /// \return
    /// Returns the created FdoBLOBValue
    /// 
	BLOBValue(System::Byte value []);

	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_DataType();

	__property System::Byte get_Data() [];

	__property System::Void set_Data(System::Byte value []);

	System::Void SetNull();

	System::Void Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor* processor);

	System::String* ToString();

	BLOBValue(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoBLOBValue* GetImpObj();
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


