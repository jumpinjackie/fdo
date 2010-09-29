#ifndef FDOSMPHMYSQLOWNER_H
#define FDOSMPHMYSQLOWNER_H       1
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

#include <FdoCommonThreadMutex.h>
#include "../../../SchemaMgr/Ph/Owner.h"
#include <Sm/Ph/Rd/DbObjectReader.h>
#include <Sm/Ph/Rd/ConstraintReader.h>
#include <Sm/Ph/Rd/TableJoin.h>

// MySql Provider implementation of an Owner.
// Represents an MySql schema (user)
class FdoSmPhMySqlOwner : public FdoSmPhGrdOwner
{
public:

    // Constructs an instance of an Owner object.
    //
    // Parameters:
    //      name: owner name
    //      pDatabase: database containing this owner.
    //      elementState: determines whether this database object is new
    FdoSmPhMySqlOwner(
        FdoStringP name,
        bool hasMetaSchema,
        const FdoSmPhDatabase* pDatabase,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added,
        FdoSmPhRdOwnerReader* reader = NULL
    );

    ~FdoSmPhMySqlOwner(void);

    // Gets this owner's default character set.
    FdoSmPhCharacterSetP GetCharacterSet();

    // The following two functions support a performance enhancement. Queries that join two 
    // information_schema tables are slow. The Schema Manager currently runs queries that 
    // join table_constraints and key_column_usage. These functions create temporary indexed
    // tables for table_constraints and key_column_usage and return the temporary table name.
    // The temporary table can then be queried instead of the information_schema table. 
    // The performance is magnitudes better when the MySql instance has a lot of schema.
    //
    // Each temporary table is built on the first call to each of these functions by a Schema Manager.
    // They are built per MySQL database and connection. Synchronization happens each time 
    // an ApplySchemaCommand is performed. This causes the Schema Managers to free their caches,
    // causing these temporary tables to be rebuilt the next time these functions are called.

    // Build and return temporary table for information_schema.key_column_usage.
    FdoStringP GetKeyColumnUsageTable();
    // Build and return temporary table for information_schema.table_constraints.
    FdoStringP GetTableConstraintsTable();
    // Build and return temporary table for information_schema.tables.
    FdoStringP GetTablesTable(
        bool createTemp // if false, don't create temp table. If it doesn't yet exist
                        //    return information_schema.tables instead,
    );
    // Build and return temporary table for information_schema.columns.
    FdoStringP GetColumnsTable(
        bool createTemp // if false, don't create temp table. If it doesn't yet exist
                        //    return information_schema.tables instead,
    );

    // Functions for dropping the temporary tables
    void DropTempTables();
    void DropKeyColumnUsageTable();
    void DropTableConstraintsTable();
    void DropTablesTable();
    void DropColumnsTable();

	// Create Meta class 
	void CreateMetaClass();

	// Get the function name that returns the datastore name from the server
	FdoString* GetDbNameClause(bool isEqual);

    // Make this owner the current schema
    virtual void SetCurrent();

    virtual FdoPtr<FdoSmPhRdDbObjectReader> CreateDbObjectReader( FdoStringP dbObject = L"") const;

    // Create a reader to get all database objects for this join.
    virtual FdoPtr<FdoSmPhRdDbObjectReader> CreateDbObjectReader( FdoSmPhRdTableJoinP join ) const;

    virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader( FdoStringP constraintName ) const;

	virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader( FdoStringP tableName, FdoStringP constraintType) const;

    virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader( FdoSmPhRdTableJoinP join, FdoStringP constraintType) const;

    virtual FdoPtr<FdoSmPhRdFkeyReader> CreateFkeyReader() const;

    virtual FdoPtr<FdoSmPhRdIndexReader> CreateIndexReader() const;

    virtual FdoPtr<FdoSmPhRdPkeyReader> CreatePkeyReader() const;

    virtual FdoPtr<FdoSmPhRdPkeyReader> CreatePkeyReader( FdoPtr<FdoSmPhRdTableJoin> join ) const;

    virtual FdoPtr<FdoSmPhRdColumnReader> CreateColumnReader() const;

    virtual FdoPtr<FdoSmPhRdColumnReader> CreateColumnReader( FdoSmPhRdTableJoinP join ) const;

	void SetOptions();

    // Drops temporary tables after each physical schema modification. 
    virtual void OnAfterCommit();

protected:
    FdoSmPhMySqlOwner() {}

    // Table and View creation implementors
    virtual FdoSmPhDbObjectP NewTable(
        FdoStringP tableName,
        FdoSchemaElementState elementState,
        FdoSmPhRdDbObjectReader* reader
    );

    virtual FdoSmPhDbObjectP NewView(
        FdoStringP viewName,
        FdoStringP rootDatabase,
        FdoStringP rootOwner,
        FdoStringP rootObjectName,
        FdoSchemaElementState elementState,
        FdoSmPhRdDbObjectReader* reader
    );

    // Overrides for modifying this owner
    virtual bool Add();
    //TODO: nothing to update yet. However, should
    //throw an exception when Modify() is called.
    virtual bool Modify() {return true;}
    virtual bool Delete();


private:
    // Drop the given temporary table
    void DropTempTable( FdoStringP tableName );
    // Generate next unique number for temporary table name.
    FdoInt32 NextTempTableNum();
	// Check if the server has a higher version than 5.5.0, including 5.5.0.
	FdoBoolean IsHigherVersionThan550();

    FdoStringP mCharacterSetName;

    FdoStringP mKeyColumnUsageTable;
    FdoStringP mTableConstraintsTable;
    FdoStringP mTablesTable;
    FdoStringP mColumnsTable;

    static FdoInt32 mTempTableNum;
    static FdoCommonThreadMutex mMutex;

};

typedef FdoPtr<FdoSmPhMySqlOwner> FdoSmPhMySqlOwnerP;

#endif
