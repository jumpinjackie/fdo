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

class FdoFunctionDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

public __gc class ArgumentDefinitionCollection;
public __gc class ReadOnlyArgumentDefinitionCollection;

/// \brief
///  The FdoFunctionDefinition class contains metadata that describes a function and its arguments.
public __gc class FunctionDefinition : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    ///  Constructs an instance of an FdoFunctionDefinition using the specified arguments.
    /// 
    /// \param name 
    /// Input the name of the function.
    /// \param description 
    /// Input a brief description.
    /// \param returnType 
    /// Input the function return type
    /// \param arguments 
    /// Input the argument definition list
    /// \param length 
    /// Input the number of arguments
    /// 
    /// \return
    /// Returns FdoFunctionDefinition
    /// 
	FunctionDefinition(System::String* name, System::String* description, NAMESPACE_OSGEO_FDO_SCHEMA::DataType returnType, NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinitionCollection* arguments);

    /// \brief
    ///  Gets the name of the function.
    /// 
    /// \return
    /// Returns the name of the function
    /// 
	__property System::String* get_Name();

    /// \brief
    ///  Gets a brief description of the function.
    /// 
    /// \return
    /// Returns the description of the function
    /// 
	__property System::String* get_Description();

    /// \brief
    ///  Gets an array of FdoArgumentDefinition objects required for the function.
    /// 
    /// \param length 
    /// Output the number of argument definitions.
    /// 
    /// \return
    /// Returns the list of argument definitions
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection* get_Arguments();

    /// \brief
    ///  Gets the FdoPropertyType of the function return value.
    /// 
    /// \return
    /// Returns the function return property type
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType get_ReturnPropertyType();

    /// \brief
    ///  Gets the FdoDataType of the function return value.
    /// 
    /// \return
    /// Returns the function return type
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_ReturnType();

    /// \brief
    /// Indicates that this object allows its name
    /// to change. This function is defined so that this class can 
    /// be a NamedCollection element.
    /// 
    /// \return
    /// Returns false
    /// 
	__property System::Boolean get_CanSetName();

protected:
	System::Void Dispose(System::Boolean disposing);

public private:
	FunctionDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoFunctionDefinition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES


