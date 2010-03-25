//
// Copyright (C) 2006 Refractions Research, Inc.
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
#ifndef FDOSMPHPOSTGISOWNER_H
#define FDOSMPHPOSTGISOWNER_H

#include <FdoCommonThreadMutex.h>
#include "../../../SchemaMgr/Ph/Owner.h"
#include <Sm/Ph/Rd/DbObjectReader.h>
#include <Sm/Ph/Rd/BaseObjectReader.h>
#include <Sm/Ph/Rd/ConstraintReader.h>
#include <Sm/Ph/Rd/TableJoin.h>
#include "SchemaCollection.h"

/// Defines owner class for PostGIS provider.
/// Owner represents a schema (datastore).
///
class FdoSmPhPostGisOwner : public FdoSmPhGrdOwner
{
public:

    /// Constructs an instance of an Owner object.
    /// Initializesz names of information_schema elements.
    ///
    /// \param
    /// name [in] - owner name
    /// \param
    /// database [in] - database containing this owner.
    /// \param
    /// state [in] - determines whether this database object is new
    ///
    FdoSmPhPostGisOwner(FdoStringP name,
        bool hasMetaSchema,
        const FdoSmPhDatabase* database,
        FdoSchemaElementState state = FdoSchemaElementState_Added,
        FdoSmPhRdDbObjectReader* reader = NULL);

    /// Destructor.
    ~FdoSmPhPostGisOwner();

    /// Build and return tem    porary table for information_schema.tables.
    /// \param
    /// createTemp [in] - false - don't create temporary table.
    /// If it doesn't yet exist return information_schema.tables instead.
    ///
    /// \todo To be documented
    ///
    FdoStringP GetTablesTable() const;

    // Extend base function to commit schema changes.
    virtual void CommitChildren( bool isBeforeParent );

    FdoSmPhPostGisSchemaP CreateSchema( FdoStringP schemaName );

     // Find a schema (given by name) in this owner.
    // Returns NULL if the schema is not in this owner.
    FdoSmPhPostGisSchemaP FindSchema( FdoStringP schemaName );

    // Retrieves all schemas for this owner.
    // Caches them if not already cached
    FdoSmPhPostGisSchemasP GetSchemas();

    /// Build and return temporary table for information_schema.columns.
    /// \param
    /// createTemp [in] - false - don't create temporary table.
    /// If it doesn't yet exist return information_schema.tables instead.
    ///
    /// \todo To be documented
    ///
    FdoStringP GetColumnsTable() const;

    /// Build and return temporary table for
    /// information_schema.table_constraints.
    ///
    /// \todo To be documented
    ///
    FdoStringP GetTableConstraintsTable() const;

    /// Build and return temporary table for
    /// information_schema.key_column_usage.
    ///
    /// \todo To be documented
    ///
    FdoStringP GetKeyColumnUsageTable() const;

	virtual bool IsDbObjectNameReserved( FdoStringP objectName );

    // Removes a schema from the cache without dropping it from
    // the RDBMS.
    void DiscardSchema( FdoSmPhPostGisSchema* schema );

	// Create Meta class 
	void CreateMetaClass();

	/// Get the function name that returns the datastore
    /// name from the server.
    /// \param
    /// isEqual [in] - 
    ///
    /// \todo To be documented
    ///
	FdoString* GetDbNameClause(bool isEqual);

    /// Make this owner the current schema.
    virtual void SetCurrent();

    /// \todo To be documented
    virtual FdoPtr<FdoSmPhRdDbObjectReader> CreateDbObjectReader(
        FdoStringP dbObject = L"") const;

    /// Create a reader to get database objects this owner and object name list.
    virtual FdoPtr<FdoSmPhRdDbObjectReader> CreateDbObjectReader( FdoStringsP objectNames ) const;

    /// Create a reader to get all database objects for this join.
    /// \todo To be documented
    virtual FdoPtr<FdoSmPhRdDbObjectReader> CreateDbObjectReader(
        FdoSmPhRdTableJoinP join) const;

    virtual FdoPtr<FdoSmPhRdBaseObjectReader> CreateBaseObjectReader() const;

    virtual FdoPtr<FdoSmPhRdBaseObjectReader> CreateBaseObjectReader( FdoStringsP objectNames ) const;

    /// \todo To be documented
    virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader(
        FdoStringP constraintName) const;

    /// \todo To be documented
	virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader(
        FdoStringP tableName, FdoStringP constraintType) const;

    virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader( 
        FdoStringsP objectNames, FdoStringP constraintType ) const;

    /// \todo To be documented
    virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader(
        FdoSmPhRdTableJoinP join, FdoStringP constraintType) const;

    /// \todo To be documented
    virtual FdoPtr<FdoSmPhRdFkeyReader> CreateFkeyReader() const;

    // Create a reader to get all indexes (ordered by table) for this owner
    virtual FdoPtr<FdoSmPhRdIndexReader> CreateIndexReader() const;

    /// Create a reader to get all indexes for this owner and object name list.
    virtual FdoPtr<FdoSmPhRdIndexReader> CreateIndexReader( FdoStringsP objectNames ) const;

    /// \todo To be documented
    virtual FdoPtr<FdoSmPhRdPkeyReader> CreatePkeyReader() const;

    /// Create a reader to get primary keys for this owner and object name list.
    virtual FdoPtr<FdoSmPhRdPkeyReader> CreatePkeyReader(  FdoStringsP objectNames ) const;

    /// \todo To be documented
    virtual FdoPtr<FdoSmPhRdColumnReader> CreateColumnReader() const;

    virtual FdoPtr<FdoSmPhRdColumnReader> CreateColumnReader(FdoStringsP objectNames) const;

    /// \todo To be documented
    virtual FdoPtr<FdoSmPhRdColumnReader> CreateColumnReader(
        FdoSmPhRdTableJoinP join) const;

	// Extract spatial context information from PostGis metadata
	virtual FdoPtr<FdoSmPhRdSpatialContextReader> CreateRdSpatialContextReader();

    virtual FdoPtr<FdoSmPhRdSpatialContextReader> CreateRdSpatialContextReader( FdoStringP dbObjectName );

    /// \todo To be documented
	void SetOptions();

protected:
    
    /// Default constructor.
    /// Initializesz names of information_schema elements.
    FdoSmPhPostGisOwner();

    // Table and View creation implementors
    virtual FdoSmPhDbObjectP NewTable(FdoStringP name,
        FdoSchemaElementState state,
        FdoSmPhRdDbObjectReader* reader);

    virtual FdoSmPhDbObjectP NewView(FdoStringP name,
        FdoStringP rootDb,
        FdoStringP rootOwner,
        FdoStringP rootObjectName,
        FdoSchemaElementState state,
        FdoSmPhRdDbObjectReader* reader);

    /// Overrides for modifying this owner
    virtual bool Add();

    //TODO: nothing to update yet. However, should
    //throw an exception when Modify() is called.
    virtual bool Modify();

    virtual bool Delete();

    // Adds the MetaSchema tables and indexes to this owner.
    void AddMetaSchema( FdoStringsP keywords, bool IsSystem = false );

private:

    /// Initializes internal state of the owner instance.
    /// This function is called from the class constructors and
    /// sets collects names of information_schema elements.
    void Init();

    FdoStringP mTablesTable;
    FdoStringP mColumnsTable;
    FdoStringP mTableConstraintsTable;
    FdoStringP mKeyColumnUsageTable;
    FdoStringP mDbNameClause;

    FdoSmPhPostGisSchemasP mSchemas;
};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhPostGisOwner> FdoSmPhPostGisOwnerP;

#endif // FDOSMPHPOSTGISOWNER_H
