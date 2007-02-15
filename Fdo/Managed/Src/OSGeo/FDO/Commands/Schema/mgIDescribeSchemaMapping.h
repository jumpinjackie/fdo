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

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA
public __gc class PhysicalSchemaMappingCollection;

/// \ingroup (interfaces)
/// \interface IDescribeSchemaMapping
/// \brief
/// The IDescribeSchemaMapping interface defines the DescribeSchemaMapping command, which
/// describes the logical to physical schema mappings for feature schemas 
/// available from the connection. The DescribeSchemaMapping
/// command can describe the mappings for a single schema or all schemas available from
/// the connection. The Execute operation returns an PhysicalSchemaMappingCollection
/// object.
public __gc __interface IDescribeSchemaMapping : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Gets the name of the schema for which to describe
    /// schema mappings.
    /// 
    /// \return
    /// Returns the schema name
    /// 
	__property System::String* get_SchemaName();

    /// \brief
    /// Sets the name of the schema to describe. This function is optional; if not
    /// specified, execution of the command will describe the mappings for
    /// all schemas.
    /// 
    /// \param value 
    /// Input the schema name
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_SchemaName(System::String* value);

    /// \brief
    /// Gets the current "include default mappings" setting.
    /// 
    /// \return
    /// Returns True if Execute() will include default mappings.
    /// 
	__property System::Boolean get_IncludeDefaults();

    /// \brief
    /// Sets the "include default mappings" setting.
    /// 
    /// \param includeDefaults 
    /// Input True: Execute() will return
    /// all mappings for the feature schema(s).
    /// False: Execute() will not include default logical to physical
    /// mappings, only those mappings that have been overridden will 
    /// be returned.
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_IncludeDefaults(System::Boolean includeDefaults );

    /// \brief
    /// Executes the DescribeSchemaMapping command and returns a 
    /// PhysicalSchemaMappingCollection. If the specified schema name does not exist,
    /// the Execute method throws an exception.
    /// 
    /// \return
    /// Returns the schema mapping collection for the requested feature schemas.
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection* Execute();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA


