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

#include "FDO\Schema\mgDataType.h"
#include "FDO\Schema\mgPropertyType.h"

class FdoArgumentDefinition;

using namespace NAMESPACE_OSGEO_FDO_SCHEMA;
BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

/// \brief
/// The FdoArgumentDefinition class contains metadata that describes an argument to a function.
public __gc class ArgumentDefinition : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Constructs an instance of an FdoArgumentDefinition using the specified arguments.
    /// 
    /// \param name 
    /// Input the name of the argument.
    /// \param description 
    /// Input a brief description of the argument.
    /// \param dataType 
    /// Input the type of argument.
    /// 
    /// \return
    /// Returns FdoArgumentDefinition
    /// 
	ArgumentDefinition(System::String* name, System::String* description, NAMESPACE_OSGEO_FDO_SCHEMA::DataType dataType);

    /// \brief
    /// Gets the name of the argument.
    /// 
    /// \return
    /// Returns the argument name
    /// 
	__property System::String* get_Name();

    /// \brief
    /// Gets a brief description of the argument.
    /// 
    /// \return
    /// Returns the argument description
    /// 
	__property System::String* get_Description();

    /// \brief
    /// Gets the FdoDataType of the argument.
    /// 
    /// \return
    /// Returns the property type of the argument
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType get_PropertyType();

    /// \brief
    /// Gets the FdoDataType of the argument.
    /// 
    /// \return
    /// Returns the data type of the argument
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_DataType();

protected:
	System::Void Dispose(System::Boolean disposing);

public private:
	ArgumentDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoArgumentDefinition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES


