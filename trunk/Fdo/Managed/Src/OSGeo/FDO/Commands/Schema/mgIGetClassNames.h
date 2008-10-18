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

#include "FDO\Commands\mgICommand.h"

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class FeatureSchemaCollection;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

/// \ingroup (OSGeoFDOCommandsSchema)
/// \interface OSGeo::FDO::Commands::Schema::IGetClassNames
/// \brief
/// The IGetClassNames interface defines the GetClassNames command, which
/// retrieves the list of available class names from the connection.  The class names
/// are qualified by the name of the feature schema that it belongs to.
/// The Execute operation returns a StringCollection object.
public __gc __interface IGetClassNames : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Gets the name of the schema for the enumeration. This function is optional;
    /// if not specified, execution of the command will enumerate the classes in all schemas.
    /// 
    /// \return
    /// Returns the schema name
    /// 
	__property System::String* get_SchemaName();

    /// \brief
    /// Sets the name of the schema for the enumeration. This function is optional; if not
    /// specified execution of the command will enumerate the classes in all schemas.
    /// 
    /// \param value 
    /// Input the schema name
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_SchemaName(System::String* value);

    /// \brief
    /// Executes the GetClassNames command and returns a 
    /// StringCollection. If the specified schema name does not exist,
    /// the Execute method throws an exception.
    /// 
    /// \return
    /// Returns the string collection of the feature classes for the specified schema.
    /// 
    NAMESPACE_OSGEO_COMMON::StringCollection* Execute();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA


