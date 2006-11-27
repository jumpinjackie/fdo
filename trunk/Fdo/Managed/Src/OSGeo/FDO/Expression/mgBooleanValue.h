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

class FdoBooleanValue;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __value enum DataType;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION

public __gc __interface IExpressionProcessor;

/// \brief
/// The FdoBooleanValue class derives from FdoDataValue and represents a Boolean
/// value.
public __gc class BooleanValue : public NAMESPACE_OSGEO_FDO_EXPRESSION::DataValue
{
public:
    /// \brief
    /// A cast operator to get the Boolean value.
    /// 
	static System::Boolean op_Explicit( NAMESPACE_OSGEO_FDO_EXPRESSION::BooleanValue* value);

    /// \brief
    /// Constructs a default instance of an FdoBooleanValue with a value of null.
    /// 
    /// \return
    /// Returns the created FdoBooleanValue
    /// 
	BooleanValue();

    /// \brief
    /// Constructs a default instance of an FdoBooleanValue using the specified arguments.
    /// 
    /// \param value 
    /// Input a Boolean value
    /// 
    /// \return
    /// Returns the created FdoBooleanValue
    /// 
	BooleanValue(System::Boolean value);

    /// \brief
    /// Gets the data type of the FdoBooleanValue.
    /// 
    /// \return
    /// Returns an FdoDataType
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_DataType();

    /// \brief
    /// Gets the FdoBooleanValue.
    /// 
    /// \return
    /// Returns a Boolean value
    /// 
	__property System::Boolean get_Boolean();

    /// \brief
    /// Sets the Boolean value.
    /// 
    /// \param value 
    /// Input a Boolean value
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Boolean(System::Boolean value);

    /// \brief
    ///  Overrides FdoExpression.Process to pass the FdoBooleanValue to the appropriate
    /// expression processor operation.
    /// 
    /// \param processor 
    /// Input the FdoIExpressionProcessor
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Process(NAMESPACE_OSGEO_FDO_EXPRESSION::IExpressionProcessor* processor);

    /// \brief
    /// Returns the well defined text representation of this expression.
    /// 
    /// \return
    /// Returns a character string.
    /// 
	System::String* ToString();

	BooleanValue(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoBooleanValue* GetImpObj();
};
END_NAMESPACE_OSGEO_FDO_EXPRESSION


