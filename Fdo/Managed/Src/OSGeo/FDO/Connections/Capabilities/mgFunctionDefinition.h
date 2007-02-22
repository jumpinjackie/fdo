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
public __gc class SignatureDefinitionCollection;
public __gc class ReadOnlySignatureDefinitionCollection;

/// \ingroup (OSGeoFDOConnectionsCapabilities)
/// \brief
///  The FunctionDefinition class contains metadata that describes a function and its arguments.
public __gc class FunctionDefinition : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    ///  Constructs an instance of a FunctionDefinition using the specified arguments.
    /// 
    /// \param name 
    /// Input the name of the function.
    /// \param description 
    /// Input a brief description.
    /// \param returnType 
    /// Input the function return type
    /// \param arguments 
    /// Input the argument definition list
    /// 
    /// \return
    /// Returns FunctionDefinition
    /// 
	FunctionDefinition (System::String *name,
                        System::String *description,
                        NAMESPACE_OSGEO_FDO_SCHEMA::DataType returnType,
                        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinitionCollection *arguments);

    /// \brief
    ///  Constructs an instance of a FunctionDefinition using the specified arguments.
    /// 
    /// \param name 
    /// Input the name of the function.
    /// \param description 
    /// Input a brief description.
    /// \param isAggregate 
    /// Input the flag indicating whether or not this is an aggregate function
    /// \param signatures 
    /// Input the list of possible function signatures
    /// 
    /// \return
    /// Returns FunctionDefinition
    /// 
	FunctionDefinition (System::String *name,
                        System::String *description,
                        System::Boolean isAggregate,
                        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection *signatures);

    /// \brief
    ///  Gets the name of the function.
    /// 
    /// \return
    /// Returns the name of the function
    /// 
	__property System::String *get_Name();

    /// \brief
    ///  Gets a brief description of the function.
    /// 
    /// \return
    /// Returns the description of the function
    /// 
	__property System::String *get_Description();

    /// \brief
    ///  Gets an array of ArgumentDefinition objects required for the function.
    /// 
    /// \return
    /// Returns the list of argument definitions
    /// 
	__property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection *get_Arguments();

    /// \brief
    ///  Gets the PropertyType of the function return value.
    /// 
    /// \return
    /// Returns the function return property type
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType get_ReturnPropertyType();

    /// \brief
    ///  Gets the DataType of the function return value.
    /// 
    /// \return
    /// Returns the function return type
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_ReturnType();

    /// \brief
    /// Indicates that this object allows its name to change. This function is
    /// defined so that this class can be a NamedCollection element.
    /// 
    /// \return
    /// Returns true if the name of the object can be changed.
    /// 
	__property System::Boolean get_CanSetName();

    /// \brief
	/// Gets an array of SignatureDefinition objects that describe the different signatures
    /// supported by the function.
    ///
    __property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection *get_Signatures();

    ///
    /// \brief
	/// Returns true if the function is an aggregate function and false if it is a simple function.
    __property System::Boolean get_IsAggregate();

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond

public private:
	FunctionDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoFunctionDefinition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES


