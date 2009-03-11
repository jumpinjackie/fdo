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
ref class FeatureSchemaCollection;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

/// \ingroup (OSGeoFDOCommandsSchema)
/// \interface OSGeo::FDO::Commands::Schema::IDescribeSchema
/// \brief
/// The IDescribeSchema interface defines the DescribeSchema command, which
/// describes the feature schemas available from the connection. The DescribeSchema
/// command can describe a single schema or all schemas available from
/// the connection. The Execute operation returns an FeatureSchemaCollection
/// object.
public interface class IDescribeSchema : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Gets the name of the schema to describe. This function is optional;
    /// if not specified, execution of the command will describe all schemas.
    /// 
    /// \return
    /// Returns the schema name
    /// 
    /// \brief
    /// Sets the name of the schema to describe. This function is optional; if not
    /// specified execution of the command will describe all schemas.
    /// 
    /// \param value 
    /// Input the schema name
    /// 
    /// \return
    /// Returns nothing
    /// 
    property System::String^ SchemaName
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Gets the names of the classes to retrieve. This is optional,
    /// if not specified execution of the command will describe all classes.
    /// If the class name is not qualified, and the schema name is not specified,
    /// the requested class from all schemas will be described.
    /// The class names specified serve only as a hint.  Use of the hint
    /// during command execution is provider dependent.  Providers that 
    /// will not use the hint will describe the schema for all classes.
    /// 
    /// \return
    /// Returns the schema name
    /// 
    /// \brief
    /// Sets the name of the classes to retrieve. This is optional, if not
    /// specified execution of the command will describe all classes.
    /// If the class name is not qualified, and the schema name is not specified,
    /// the requested class from all schemas will be described.
    /// The class names specified serve only as a hint.  Use of the hint
    /// during command execution is provider dependent.  Providers that 
    /// will not use the hint will describe the schema for all classes.
    /// 
    /// \param value 
    /// Input the schema name
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_COMMON::StringCollection^ ClassNames
    {
        NAMESPACE_OSGEO_COMMON::StringCollection^ get();
        System::Void set(NAMESPACE_OSGEO_COMMON::StringCollection^ value);
    }

    /// \brief
    /// Executes the DescribeSchema command and returns a 
    /// FeatureSchemaCollection. If a schema name is given that has 
    /// references to another schema, the dependent schemas will 
    /// be returned as well. If the specified schema name does not exist,
    /// the Execute method throws an exception.
    /// 
    /// \return
    /// Returns the schema collection representing the schema created.
    /// The element states for all elements will be set to SchemaElementState_Unchanged.
    /// Each provider-specific implementation of Execute() can ensure 
    /// that this is the case by 
    /// calling FeatureSchema::AcceptChanges() for each feature schema
    /// in the returned collection.
    /// 
	NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection^ Execute();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA



