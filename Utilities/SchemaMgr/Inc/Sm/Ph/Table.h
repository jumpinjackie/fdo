#ifndef FDOSMPHTABLE_H
#define FDOSMPHTABLE_H		1
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Ph/DbObject.h>
#include <Sm/Ph/FkeyCollection.h>
#include <Sm/Ph/IndexCollection.h>
#include <Sm/Ph/ColumnBatchCollection.h>
#include <Sm/Ph/CheckConstraintCollection.h>

class FdoSmPhMgr;
class FdoSmPhOwner;
class FdoSmPhRdPkeyReader;
class FdoSmPhRdFkeyReader;
class FdoSmPhRdConstraintReader;
class FdoSmPhRdIndexReader;

// Table represents a table in the database.
class FdoSmPhTable : virtual public FdoSmPhDbObject
{
public:
    /// Returns true if adding not-null column to existing table supported
    /// (Oracle and MySql support this as long as the table is empty).
    /// Provider specific derivations must override this function if they don't
    /// support adding not-null columns.
    virtual bool SupportsAddNotNullColumn() const;

    /// Returns all the primary key columns in this table.
    const FdoSmPhColumnCollection* RefPkeyColumns() const;
    FdoSmPhColumnsP GetPkeyColumns();

    /// Returns all the unique columns in this table.
    const FdoSmPhBatchColumnCollection* RefUkeyColumns() const;
    FdoSmPhBatchColumnsP GetUkeyColumns();

    /// Returns all the unique columns in this table.
    const FdoSmPhCheckConstraintCollection* RefCkeyColl() const;
    FdoSmPhCheckConstraintCollection* GetCkeyColl();

    /// Returns a CHECK() clause, each provider must implement. It should be pure virtual.
	virtual FdoStringP GetCkeyClause( FdoStringP columnName, FdoDataPropertyP fdoProp ) { return L""; }

    /// Returns all the indexes in this table.
    const FdoSmPhIndexCollection* RefIndexes() const;
    FdoSmPhIndexesP GetIndexes();

    /// Returns the table's best identity columns.
    /// These are the primary key columns if the table has a primary key.
    /// Otherwise, the columns for the table's lightest unique index are returned.
    /// NULL is returned if the table has no identity.
    FdoSmPhColumnsP GetBestIdentity();
    
    /// Returns all foreign keys for which this table is the foreign table
    const FdoSmPhFkeyCollection* RefFkeysUp() const;
    FdoSmPhFkeysP GetFkeysUp();

    /// Returns the name of this table's primary key
	FdoStringP GetPkeyName() const;

    /// Returns an array of all types of locking supported on this Database Object.
    /// size is set to the size of the array.
    /// Returns NULL when size is 0.
	virtual const FdoLockType* GetLockTypes(FdoInt32& size) const;

    /// Get SQL "create table" statement.
    virtual FdoStringP GetAddSql();

    /// Get SQL "drop table" statement
    virtual FdoStringP GetDeleteSql();

    /// Get SQL "create table" heading
    virtual FdoStringP GetAddHdgSql();

    /// Get SQL "create table" column list
    virtual FdoStringsP GetAddColsSql();

    /// Get SQL clause for adding a single column
    virtual FdoStringP GetAddColSql();

    /// Get SQL clause for dropping a column
    virtual FdoStringP GetDeleteColSql();

    /// Get SQL for creating primary key on new table
    FdoStringP GetAddPkeySql();

    /// Get SQL for creating UNIQUE constraints on new table
    FdoStringP GetAddUkeySql();

    /// Get SQL for creating CHECK constraints on new table
    FdoStringP GetAddCkeySql();

    /// Set the long transaction mode.
    /// Can only be changed on new tables.
    virtual void SetLtMode( FdoLtLockModeType mode );

    /// Sets the primary key name for this table.
    /// Can only be called for new tables.
    virtual void SetPkeyName( FdoStringP pkeyName );

    /// Set the locking mode.
    /// Can only be changed on new tables.
    virtual void SetLockingMode( FdoLtLockModeType mode );

    /// Add a column to the primary key.
	void AddPkeyCol(FdoStringP columnName );

    /// Add a column to a collection of unque keys 
	void AddUkeyCol(int uCollNum, FdoStringP columnName );

    /// Add a clause to the constraints list.
	void AddCkeyCol(FdoSmPhCheckConstraintP clause );

    /// Add an index to this table
    FdoSmPhIndexP CreateIndex(
        FdoStringP name, 
        bool isUnique
    );

    /// Add a foreign key to this table
    FdoSmPhFkeyP CreateFkey(
        FdoStringP name, 
        FdoStringP pkeyTableName,
        FdoStringP pkeyTableOwner = L""
    );

    /// Removes the given index from the cache without
    /// deleting it from the datastore.
    void DiscardIndex( FdoSmPhIndex* index );

    /// Commit modifications to foreign keys.
    virtual void CommitFkeys( bool isBeforeParent );

    /// Deletes all rows from this table.
    virtual void ClearRows() = 0;

    /// Set the modification state of this table.
	virtual void SetElementState(FdoSchemaElementState elementState);

    /// Drops the table whether or not it has data.
    void ForceDelete();

    /// Gather all errors for this element and child elements into a chain of exceptions.
    /// Adds each error as an exception, to the given exception chain and returns
    /// the chain.
	//
    /// parameters:
    /// 	pFirstException: a chain of exceptions.
	virtual FdoSchemaExceptionP Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;

    /// Serialize the table to an XML file.
    /// Mainly for unit testing.
	virtual void XMLSerialize( FILE* xmlFp, int ref ) const;

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhTable() {}

    /// Constructs an instance of a Table object.
    /// 
    /// Parameters:
    /// 	name: table name
    /// 	pOwner: the table's owning schema.
    ///      elementState: indicates whether this is a new or existing table.
    ///      pkeyName: name for table's primary key. Applies only to new tables.
    ///          Primary key name on existing tables not changed.
    FdoSmPhTable(
        FdoStringP name, 
        const FdoSmPhOwner* pOwner,
		FdoSchemaElementState elementState = FdoSchemaElementState_Added,
        FdoStringP pkeyName = L""
    );

	~FdoSmPhTable(void);

    /// Add an index from an index reader
    FdoSmPhIndexP CreateIndex(
        FdoPtr<FdoSmPhRdIndexReader> rdr
    );

    /// Readers for Primary Key, Constraints, Foreign Keys and Indexes.
    /// All have provider-specific implementations
    virtual FdoPtr<FdoSmPhRdPkeyReader> CreatePkeyReader() const = 0;
    virtual FdoPtr<FdoSmPhRdFkeyReader> CreateFkeyReader() const = 0;
	virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader(FdoString* type) const = 0;
    virtual FdoPtr<FdoSmPhRdIndexReader> CreateIndexReader() const = 0;

    /// Index object creator
    virtual FdoSmPhIndexP NewIndex(
        FdoStringP name, 
        bool isUnique,
		FdoSchemaElementState elementState = FdoSchemaElementState_Added
    ) = 0;

    /// Spatial Index object creator
    virtual FdoSmPhIndexP NewSpatialIndex(
        FdoStringP name, 
        bool isUnique,
		FdoSchemaElementState elementState = FdoSchemaElementState_Added
    ) = 0;

    /// Foreign key object creator
    virtual FdoSmPhFkeyP NewFkey(
        FdoStringP name, 
        FdoStringP pkeyTableName,
        FdoStringP pkeyTableOwner,
		FdoSchemaElementState elementState = FdoSchemaElementState_Added
    ) = 0;

    /// Commit modifications to child objects
    virtual void CommitChildren( bool isBeforeParent );

    /// Commit modifications to columns
    virtual void CommitColumns( bool isBeforeParent );

    /// Autogenerate a unique primary key name for this table.
	virtual FdoStringP GenPkeyName();

    /// Column update functions, each provider must implement.
    virtual bool AddColumn( FdoSmPhColumnP column ) = 0;
    virtual bool ModifyColumn( FdoSmPhColumnP column ) = 0;
    virtual bool DeleteColumn( FdoSmPhColumnP column ) = 0;

private:
    /// Load Primary Key if not yet loaded
    void LoadPkeys();
    /// Load Foreign Keys if not yet loaded
    void LoadFkeys();
    /// Load Unique Keys if not yet loaded
	void LoadUkeys();
    /// Load Check Keys if not yet loaded
	void LoadCkeys();
	
    /// Load Indexes if not yet loaded
    void LoadIndexes();
	
    void AddPkeyColumnError(FdoStringP columnName);
    void AddFkeyColumnError(FdoStringP columnName);
	void AddUkeyColumnError(FdoStringP columnName);
    void AddCkeyColumnError(FdoStringP columnName);
    void AddIndexColumnError(FdoStringP columnName);
    void AddDeleteNotEmptyError(void);

    FdoSmPhColumnsP mPkeyColumns;

	FdoSmPhBatchColumnsP		mUkeysCollection;
	FdoSmPhCheckConstraintsP	mCkeysCollection;

    FdoSmPhFkeysP mFkeysUp;

    FdoSmPhIndexesP mIndexes;

	FdoStringP mPkeyName;
};

typedef FdoPtr<FdoSmPhTable> FdoSmPhTableP;

#endif


