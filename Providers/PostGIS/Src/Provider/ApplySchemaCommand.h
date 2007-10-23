//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_APPLYSCHEMACOMMAND_H_INCLUDED
#define FDOPOSTGIS_APPLYSCHEMACOMMAND_H_INCLUDED

#include "Command.h"

//
// Forward declarations
//


namespace fdo { namespace postgis {

/// Implementation of create command, that can be used to create new
/// datastore on a particular PostgreSQL instance.
///
class ApplySchemaCommand : public Command<FdoIApplySchema>
{
public:

    /// Type of FDO smart pointer for Connection class.
    typedef FdoPtr<ApplySchemaCommand> Ptr;

    /// Default constructor.
    ApplySchemaCommand(Connection* conn);

    /// Destructor.
    virtual ~ApplySchemaCommand();

    //
    // FdoIApplySchema interface
    //

    /// Get name of the schema to create.
    /// \return
    /// Pointer to schema being created.
    /// \exception
    /// TO BE WRITTEN
    ///
    virtual FdoFeatureSchema* GetFeatureSchema();

    /// Set name of the schema to create.
    /// \param
    /// schema [in] - name of schema being created.
    /// \exception
    /// TO BE WRITTEN
    ///
    virtual void SetFeatureSchema(FdoFeatureSchema* schema);

    /// Get FdoPhysicalSchemaMapping used to specify how the schema definition
    /// gets mapped into physical storage.
    ///
    /// \return
    /// Mapping object describing how FDO schema is mapped to physical storage.
    /// \exception
    /// TO BE WRITTEN
    ///
    virtual FdoPhysicalSchemaMapping* GetPhysicalMapping();

    /// Set the FdoPhysicalSchemaMapping used to specify how the schema definition
    /// gets mapped into physical storage.
    ///
    /// \param
    /// sm [in] - object describing how FDO schema is mapped to physical storage.
    /// \exception
    /// TO BE WRITTEN
    ///
    virtual void SetPhysicalMapping(FdoPhysicalSchemaMapping* sm);

    /// Get flag indicating whether Execute() will ignore element states when
    /// applying feature schema.
    ///
    /// \return true if actual state of schema element should be ignored,
    /// false otherwise.
    ///
    virtual FdoBoolean GetIgnoreStates();

    /// Set Changes the handling of element states by Execute().
    ///
    /// \param
    /// ignoreStates [in] - flag indicating if schema element state
    /// should be ignored during command execution,
    ///
    virtual void SetIgnoreStates(FdoBoolean ignoreStates);

    /// Execute the ApplySchema command that creates metadata and
    /// physical storage for the schema.
    ///
    /// Implementors must call FdoFeatureSchema::AcceptChanges() when Execute()
    /// succeeds. It must be called after the schema has been successfully applied,
    /// in order to synchronize the states of the schema elements.
    ///
    /// \exception
    /// FdoException - if the schema already exists or if it exists and the feature
    /// provider only supports a single schema.
    /// \exception -  if schema changes including deletion of classes or the schema
    /// changes itself, but those classes (or the schema, if it is marked as deleted)
    /// still contain any instance data.
    /// \exception
    /// FdoCommandException - if database operation or SQL executeion failed.
    ///
    virtual void Execute();

private:

    //
    // Private data members
    //

    typedef Command<FdoIApplySchema> Base;

    FdoPtr<FdoFeatureSchema> mFeatureSchema;
    FdoPtr<FdoPhysicalSchemaMapping> mSchemaMapping;
    FdoBoolean mIgnoreStates;


    //
    // Private operations
    //


    // This function registers geometry column in PostGIS meta-schema.
    // It is a simple proxy around SQL command:
    // AddGeometryColumn(<schema_name>,<table_name>,<column_name>,<srid>,<type>,<dimension>)
    // \param
    // table - name of table mapped from feature class name.
    // \param
    // prop - property defining geometry column being registered in PostGIS schema.
    //
    void AddGeometryColumn(std::string const& table, FdoPtr<FdoGeometricPropertyDefinition> prop) const;

    // Create GiST index for given table and geometry column in PostGIS meta-schema.
    //
    // \param
    // table - name of table mapped from feature class name.
    // \param
    // prop - property defining geometry column for which GiST index is created.
    //
    void CreateSpatialIndex(std::string const& table, FdoPtr<FdoGeometricPropertyDefinition> prop) const;

    void CreateSequence(std::string const& table, FdoPtr<FdoDataPropertyDefinition> prop) const;
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_APPLYSCHEMACOMMAND_H_INCLUDED
