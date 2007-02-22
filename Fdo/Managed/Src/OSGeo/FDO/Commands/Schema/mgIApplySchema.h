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
public __gc class FeatureSchema;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA
public __gc class PhysicalSchemaMapping;

/// \ingroup (OSGeoFDOCommandsSchema)
/// \interface OSGeo::FDO::Commands::Schema::IApplySchema
/// \brief
/// The IApplySchema interface defines the ApplySchema command, which creates or
/// updates a feature schema within the DataStore. Input to the ApplySchema
/// command is the feature schema instance to be created or updated. Optionally, 
/// a provider-specific IPhysicalSchemaMapping interface can be specified that can be
/// used to control how the feature schema elements are mapped into physical
/// storage.
public __gc __interface IApplySchema : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommand
{
public:
    /// \brief
    /// Gets the name of the schema to create.
    /// 
    /// \return
    /// Returns the name of the schema to create
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema* get_FeatureSchema();

    /// \brief
    /// Sets the name of the schema to create.
    /// 
    /// \param value 
    /// Input the name of the schema to create
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_FeatureSchema(NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema* value);

    /// \brief
    ///  Gets the PhysicalSchemaMapping used to specify how the schema definition
    /// gets mapped into physical storage.
    /// 
    /// \return
    /// Returns PhysicalSchemaMapping
    /// 
	__property NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* get_PhysicalMapping();

    /// \brief
    /// Sets the PhysicalSchemaMapping used to specify how the schema definition
    /// gets mapped into physical storage.
    /// 
    /// \param value 
    /// Input the PhysicalSchemaMapping
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_PhysicalMapping(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* value);

    /// \brief
    ///  Indicates whether Execute() will ignore element states 
    /// when applying the feature schema.
    /// 
    /// \return
    /// Returns true if elements states will be ignored, false otherwise. 
    /// 
    ///  
	__property System::Boolean get_IgnoreStates();

    /// \brief
    /// Changes the handling of element states by Execute().
    /// 
    /// \param ignoreStates 
    /// Input True: Execute() will ignore the 
    /// element states on the Feature Schema elements. Instead, it 
    /// will additively merge the Feature Schema into the current DataStore.
    /// False: Execute() will use the element states to determine whether
    /// each element is inserted, modified or deleted.
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_IgnoreStates(System::Boolean ignoreStates );

    /// \brief
    /// Executes the ApplySchema command that creates metadata and physical
    /// storage for the schema. An exception is thrown if the schema already
    /// exists or if a schema already exists and the feature provider only
    /// supports a single schema. If schema changes include deletion of classes
    /// or the schema itself, then those classes (or the schema, if it is marked
    /// as deleted) must not contain any instance data. If they do, an exception
    /// will be thrown.
    /// Implementors must call FeatureSchema::AcceptChanges() when Execute() succeeds.
    /// It must be called after the schema has been successfully applied, in
    /// order to synchronize the states of the schema elements.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Execute();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA


