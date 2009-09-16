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
#include "FDO\Connections\Capabilities\mgFunctionCategoryType.h"

class FdoFunctionDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES

ref class ArgumentDefinitionCollection;
ref class ReadOnlyArgumentDefinitionCollection;
ref class SignatureDefinitionCollection;
ref class ReadOnlySignatureDefinitionCollection;

/// \ingroup (OSGeoFDOConnectionsCapabilities)
/// \brief
///  The FunctionDefinition class contains metadata that describes a function and its arguments.
public ref class FunctionDefinition : public NAMESPACE_OSGEO_RUNTIME::Disposable
{

public:

    /// \brief
    /// Constructs a FunctionDefinition object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
    FunctionDefinition(System::IntPtr unmanaged, System::Boolean autoDelete);

    /// \brief
    /// Constructs an instance of a FunctionDefinition using the specified arguments.
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
	FunctionDefinition (System::String ^name,
                        System::String ^description,
                        NAMESPACE_OSGEO_FDO_SCHEMA::DataType returnType,
                        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinitionCollection ^arguments);

    /// \brief
    /// Constructs an instance of a FunctionDefinition using the specified arguments.
    /// 
    /// \param name 
    /// Input the name of the function.
    /// \param description 
    /// Input a brief description.
    /// \param returnType 
    /// Input the function return type
    /// \param arguments 
    /// Input the argument definition list
    /// \param functionCategoryType 
    /// Input the category the function belongs to. Valid values for this parameter are defined
    /// in the enumeration FunctionCategoryType.
    /// \param supportsVariableArgumentsList
    /// Input a flag indicating whether or not this function supports a variable list of arguments
    /// 
    /// \return
    /// Returns FunctionDefinition
    /// 
	FunctionDefinition (System::String ^name,
                        System::String ^description,
                        NAMESPACE_OSGEO_FDO_SCHEMA::DataType returnType,
                        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinitionCollection ^arguments,
                        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionCategoryType functionCategoryType,
                        System::Boolean supportsVariableArgumentsList);

    /// \brief
    ///  Constructs an instance of a FunctionDefinition using the specified arguments.
    /// 
    /// \param name 
    /// Input the name of the function.
    /// \param description 
    /// Input a brief description.
    /// \param returnPropertyType 
    /// Input the function return property type
    /// \param returnType 
    /// Input the function return type
    /// \param arguments 
    /// Input the argument definition list
    /// 
    /// \return
    /// Returns FunctionDefinition
    /// 
    FunctionDefinition (System::String ^name,
                        System::String ^description,
                        NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType returnPropertyType,
                        NAMESPACE_OSGEO_FDO_SCHEMA::DataType returnType,
                        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinitionCollection ^arguments);

    /// \brief
    ///  Constructs an instance of a FunctionDefinition using the specified arguments.
    /// 
    /// \param name 
    /// Input the name of the function.
    /// \param description 
    /// Input a brief description.
    /// \param returnPropertyType 
    /// Input the function return property type
    /// \param returnType 
    /// Input the function return type
    /// \param arguments 
    /// Input the argument definition list
    /// \param functionCategoryType 
    /// Input the category the function belongs to. Valid values for this parameter are defined
    /// in the enumeration FunctionCategoryType.
    /// \param supportsVariableArgumentsList
    /// Input a flag indicating whether or not this function supports a variable list of arguments
    /// 
    /// \return
    /// Returns FunctionDefinition
    /// 
    FunctionDefinition (System::String ^name,
                        System::String ^description,
                        NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType returnPropertyType,
                        NAMESPACE_OSGEO_FDO_SCHEMA::DataType returnType,
                        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ArgumentDefinitionCollection ^arguments,
                        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionCategoryType functionCategoryType,
                        System::Boolean supportsVariableArgumentsList);

    /// \brief
    /// Constructs an instance of a FunctionDefinition using the specified arguments.
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
	FunctionDefinition (System::String ^name,
                        System::String ^description,
                        System::Boolean isAggregate,
                        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection ^signatures);

    /// \brief
    /// Constructs an instance of a FunctionDefinition using the specified arguments.
    /// 
    /// \param name 
    /// Input the name of the function.
    /// \param description 
    /// Input a brief description.
    /// \param isAggregate 
    /// Input the flag indicating whether or not this is an aggregate function
    /// \param signatures 
    /// Input the list of possible function signatures
    /// \param functionCategoryType 
    /// Input the category the function belongs to. Valid values for this parameter are defined
    /// in the enumeration FunctionCategoryType.
    /// \param supportsVariableArgumentsList
    /// Input a flag indicating whether or not this function supports a variable list of arguments
    /// 
    /// \return
    /// Returns FunctionDefinition
    /// 
	FunctionDefinition (System::String ^name,
                        System::String ^description,
                        System::Boolean isAggregate,
                        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::SignatureDefinitionCollection ^signatures,
                        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionCategoryType functionCategoryType,
                        System::Boolean supportsVariableArgumentsList);

    /// \brief
    /// Gets the name of the function.
    /// 
    /// \return
    /// Returns the name of the function
    /// 
    property System::String^ Name
    {
        System::String^ get();
    }

    /// \brief
    /// Gets a brief description of the function.
    /// 
    /// \return
    /// Returns the description of the function
    /// 
    property System::String^ Description
    {
        System::String^ get();
    }

    /// \brief
    /// Gets an array of ArgumentDefinition objects required for the function.
    /// 
    /// \return
    /// Returns the list of argument definitions
    /// 
    property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection^ Arguments
    {
        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlyArgumentDefinitionCollection^ get();
    }

    /// \brief
    /// Gets the PropertyType of the function return value.
    /// 
    /// \return
    /// Returns the function return property type
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType ReturnPropertyType
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType get();
    }

    /// \brief
    /// Gets the DataType of the function return value.
    /// 
    /// \return
    /// Returns the function return type
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::DataType ReturnType
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::DataType get();
    }

    /// \brief
    /// Indicates that this object allows its name to change. This function is
    /// defined so that this class can be a NamedCollection element.
    /// 
    /// \return
    /// Returns true if the name of the object can be changed.
    /// 
    property System::Boolean CanSetName
    {
        System::Boolean get();
    }

    /// \brief
	/// Gets an array of SignatureDefinition objects that describe the different signatures
    /// supported by the function.
    ///
    property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection^ Signatures
    {
        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::ReadOnlySignatureDefinitionCollection^ get();
    }

    ///
    /// \brief
	/// Returns true if the function is an aggregate function and false if it is a simple function.
    property System::Boolean IsAggregate
    {
        System::Boolean get();
    }

    /// \brief
    ///  Gets the category the function belongs to.
    /// 
    /// \return
    /// Returns the function category
    /// 
    property NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionCategoryType FunctionCategoryType
    {
        NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES::FunctionCategoryType get();
    }

    /// \brief
    /// Returns true if the function allows a variable list of arguments and false otherwise.
    ///
    property System::Boolean SupportsVariableArgumentsList
    {
        System::Boolean get();
    }

internal:
	inline FdoFunctionDefinition* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};  //  FunctionDefinition

END_NAMESPACE_OSGEO_FDO_CONNECTIONS_CAPABILITIES


