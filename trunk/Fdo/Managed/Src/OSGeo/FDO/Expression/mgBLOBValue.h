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

/// \ingroup (OSGeoFDOExpression)
/// \brief
/// The BLOBValue class derives from LOBValue and represents a literal
/// BLOB value.
public __gc class BLOBValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue 
{
public:
    /// \brief
    /// Constructs a default instance of an BLOBValue with a value of null.
    /// 
    /// \return
    /// Returns the created BLOBValue
    /// 
	BLOBValue();

    /// \brief
    /// Constructs an instance of an BLOBValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a byte array
    /// 
    /// \return
    /// Returns the created BLOBValue
    /// 
	BLOBValue(System::Byte value []);

    /// \brief
    /// Gets the FDO DataType of the BLOB
    /// 
    /// \return
    /// Returns the FDO DataType
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_DataType();

    /// \brief
    /// Gets the data associated to the blob as an Byte Array
    /// 
    /// \return
    /// Returns the Byte Array containing the BLOB data
    /// 
	__property System::Byte get_Data() [];

    /// \brief
    /// Sets the data associated to the BLOB using an FDO ByteArray
    /// 
    /// \param value
    /// Input the FDO Byte Array
    /// 
	__property System::Void set_Data(System::Byte value []);

    /// \brief
    /// Sets the value of the BLOB to NULL.
    /// 
	System::Void SetNull();

    /// \brief
    /// Overrides FdoIExpression::Process to pass the DataValue to the appropriate
    /// expression processor operation.
    /// 
    /// \param processor
    /// Input the pointer to the FDO expression processor
    /// 
	System::Void Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor* processor);

    /// \brief
    /// Gets the value of the BLOB as a string.
    /// 
    /// \return
    /// Returns the well defined text representation of the BLOB
    /// 
	System::String* ToString();

    /// \brief
    /// Constructs a BLOBValue object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	BLOBValue(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoBLOBValue* GetImpObj();
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


