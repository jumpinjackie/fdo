#ifndef _FUNCTIONDEFINITION_H_
#define _FUNCTIONDEFINITION_H_
//

//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifdef _WIN32
#pragma once
#endif

#include <Std.h>
#include <Fdo/Schema/PropertyType.h>
#include <Fdo/Connections/Capabilities/ArgumentDefinition.h>
#include <Fdo/Connections/Capabilities/ReadOnlyArgumentDefinitionCollection.h>

/// \brief
///  The FdoFunctionDefinition class contains metadata that describes a function and its arguments.
class FdoFunctionDefinition : public FdoIDisposable
{
protected:
/// \cond DOXYGEN-IGNORE
    /// Constructs an instance of an FdoFunctionDefinition using the specified arguments.
    FdoFunctionDefinition(FdoString* name, FdoString* description, FdoDataType returnType, FdoArgumentDefinitionCollection* arguments);

    /// Constructs an instance of an FdoFunctionDefinition using the specified arguments.
    FdoFunctionDefinition(FdoString* name, FdoString* description, FdoPropertyType returnPropertyType, FdoDataType returnType, FdoArgumentDefinitionCollection* arguments);

    /// Constructs an instance of an FdoFunctionDefinition using default values.
    FdoFunctionDefinition();

    virtual ~FdoFunctionDefinition();

    virtual void Dispose();
/// \endcond

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
    FDO_API static FdoFunctionDefinition* Create(FdoString* name, FdoString* description, FdoDataType returnType, FdoArgumentDefinitionCollection* arguments);

    /// \brief
    ///  Constructs an instance of an FdoFunctionDefinition using the specified arguments.
    /// 
    /// \param name 
    /// Input the name of the function.
    /// \param description 
    /// Input a brief description.
    /// \param returnProperty Type 
    /// Input the function return property type
    /// \param returnType 
    /// Input the function return data type (ignore it property type is not data)
    /// \param arguments 
    /// Input the argument definition list
    /// \param length 
    /// Input the number of arguments
    /// 
    /// \return
    /// Returns FdoFunctionDefinition
    /// 
    FDO_API static FdoFunctionDefinition* Create(FdoString* name, FdoString* description, FdoPropertyType returnPropertyType, FdoDataType returnType, FdoArgumentDefinitionCollection* arguments);

    /// \brief
    ///  Gets the name of the function.
    /// 
    /// \return
    /// Returns the name of the function
    /// 
    FDO_API FdoString* GetName();
    /// \brief
    ///  Gets a brief description of the function.
    /// 
    /// \return
    /// Returns the description of the function
    /// 
    FDO_API FdoString* GetDescription();
    /// \brief
    ///  Gets an array of FdoArgumentDefinition objects required for the function.
    /// 
    /// \param length 
    /// Output the number of argument definitions.
    /// 
    /// \return
    /// Returns the list of argument definitions
    /// 
    FDO_API FdoReadOnlyArgumentDefinitionCollection* GetArguments();
    /// \brief
    ///  Gets the FdoPropertyType of the function return value.
    /// 
    /// \return
    /// Returns the function return property type
    /// 
    FDO_API FdoPropertyType GetReturnPropertyType();
    /// \brief
    ///  Gets the FdoDataType of the function return value.
    ///  This is only valid if the function return property type is FdoPropertyType_DataProperty.
    /// 
    /// \return
    /// Returns the function return data type
    /// 
    FDO_API FdoDataType GetReturnType();
    /// \brief
    ///  Indicates that this object allows its name
    /// to change. This function is defined so that this class can 
    /// be a FdoNamedCollection element.
    /// 
    /// \return
    /// Returns false
    /// 
    FDO_API virtual FdoBoolean CanSetName()
    {
        return false;
    }

protected:
    wchar_t*                m_name;
    wchar_t*                m_description;
    FdoReadOnlyArgumentDefinitionCollection*    m_arguments;
    FdoPropertyType         m_returnPropertyType;
    FdoDataType             m_returnDataType;
};
#endif


