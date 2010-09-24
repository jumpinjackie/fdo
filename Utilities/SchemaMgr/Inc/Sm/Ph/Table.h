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
#include <Sm/Ph/IndexCollection.h>
#include <Sm/Ph/ColumnBatchCollection.h>
#include <Sm/Ph/CheckConstraintCollection.h>

class FdoSmPhMgr;
class FdoSmPhOwner;
class FdoSmPhReader;
class FdoSmPhRdConstraintReader;
class FdoSmPhTableComponentReader;

// Table represents a table in the database.
class FdoSmPhTable : virtual public FdoSmPhDbObject
{
public:
    /// Returns true if adding not-null column to existing table supported
    /// (Oracle and MySql support this as long as the table is empty).
    /// Provider specific derivations must override this function if they don't
    /// support adding not-null columns.
    virtual bool SupportsAddNotNullColumn() const;

    /// Returns all the unique columns in this table.
    const FdoSmPhBatchColumnCollection* RefUkeyColumns() const;
    FdoSmPhBatchColumnsP GetUkeyColumns();

    /// Returns all the unique columns in this table.
    const FdoSmPhCheckConstraintCollection* RefCkeyColl() const;
    FdoSmPhCheckConstraintsP GetCkeyColl();

    /// Returns a CHECK() clause, each provider must implement. It should be pure virtual.
	virtual FdoStringP GetCkeyClause( FdoStringP columnName, FdoDataPropertyP fdoProp ) { return L""; }

    /// Returns a CHECK() clause, each provider must implement. It should be pure virtual.
	virtual FdoStringP FixCkeyClause( FdoStringP checkClause ) { return checkClause; }

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

	/// Get SQL clause for dropping a constraint
	virtual FdoStringP GetDropConstraintSql(FdoStringP constraintName);

	/// Get SQL clause for adding a constraint
	virtual FdoStringP GetAddConstraintSql(FdoStringP constraint);

    /// Get SQL for creating UNIQUE constraints on new table
    FdoStringP GetAddUkeysSql();

	/// Get SQL for creating a UNIQUE constraint on an existing table
	FdoStringP GetAddUkeySql(int uCollNum);

    /// Get SQL for creating CHECK constraints on new table
    FdoStringP GetAddCkeysSql();

	/// Get SQL for creating a CHECK constraint on an existing table
	FdoStringP GetAddCkeySql(int uCollNum);

    // Given a check constraint, return the SQL clause for adding it to its table.
    virtual FdoStringP GetAddCkeySql(FdoSmPhCheckConstraint* ckey);
    
    // Given a constraint name, return the name to use in DDL operations, such 
    // as dropping the constraint.
    virtual FdoStringP GetConstraintDDLName( FdoStringP constraintName ) const;

    /// Set the long transaction mode.
    /// Can only be changed on new tables.
    virtual void SetLtMode( FdoLtLockModeType mode );

    /// Set the locking mode.
    /// Can only be changed on new tables.
    virtual void SetLockingMode( FdoLtLockModeType mode );

    /// Add a column to a collection of unique keys 
	void AddUkeyCol(int uCollNum, FdoStringP columnName );

	/// Add all unique keys
	void AddUkeys();

    /// Add a clause to the constraints list.
	void AddCkeyCol(FdoSmPhCheckConstraintP clause );

	/// Add all check constraints
	void AddCkeys();

    /// Add an index to this table
    FdoSmPhIndexP CreateIndex(
        FdoStringP name, 
        bool isUnique
    );

    // Add a unique constraint to this table.
    FdoSmPhColumnsP CreateUkey();

	/// Return the list of constraints marked for drop
	FdoStringsP	GetDeletedConstraints();

    /// Deletes all rows from this table.
    virtual void ClearRows() = 0;

    /// Set the modification state of this table.
	virtual void SetElementState(FdoSchemaElementState elementState);

    // Load this table's unique constraints from the given reader
    virtual void CacheUkeys( FdoPtr<FdoSmPhRdConstraintReader> rdr );

    // Load this table's check constraints from the given reader
    virtual void CacheCkeys( FdoPtr<FdoSmPhRdConstraintReader> rdr );

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

    /// Readers for Constraints 
    /// All have provider-specific implementations
	virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader(FdoString* type) const = 0;
    
    /// Commit modifications to child objects
    virtual void CommitChildren( bool isBeforeParent );

    /// Commit modifications to columns
    virtual void CommitColumns( bool isBeforeParent );

	/// Commit new added Unique constraints
	virtual void CommitUConstraints( bool isBeforeParent );

	/// Commit new added Check constraints
	virtual void CommitCConstraints( bool isBeforeParent );

    /// Column update functions, each provider must implement.
    virtual bool AddColumn( FdoSmPhColumnP column ) = 0;
    virtual bool ModifyColumn( FdoSmPhColumnP column ) = 0;
    virtual bool DeleteColumn( FdoSmPhColumnP column ) = 0;

	/// Drop a constaint given its name in RDBMS.
	virtual void DropConstraint( FdoStringP constraintName ) = 0;

	/// Add a constrait given the fully qualified string.
	virtual bool AddConstraint( FdoStringP constraint ) = 0;


    /// Load Unique Keys if not yet loaded
	void LoadUkeys();
	void LoadUkeys( FdoPtr<FdoSmPhReader> ukeyRdr, bool isSkipAdd );
    
    // Load a unique key column from a reader.
    virtual bool LoadUkeyColumn( FdoPtr<FdoSmPhReader> ukeyRdr, FdoSmPhColumnsP ukey  );
    
    /// Load Check Keys if not yet loaded
	void LoadCkeys();
	virtual void LoadCkeys( FdoPtr<FdoSmPhReader> ckeyRdr, bool isSkipAdd );

    // Create new unique constraint group reader
    virtual FdoPtr<FdoSmPhTableComponentReader> NewTableUkeyReader( FdoPtr<FdoSmPhRdConstraintReader> rdr );

    // Create new check constraint group reader
    virtual FdoPtr<FdoSmPhTableComponentReader> NewTableCkeyReader( FdoPtr<FdoSmPhRdConstraintReader> rdr );

	void AddUkeyColumnError(FdoStringP columnName);
	void AddUkeyError(FdoStringP columnNames);
    void AddCkeyColumnError(FdoStringP columnName);
	void AddCkeyError(FdoStringP columnNames);
    void AddDeleteNotEmptyError(void);

private:
	FdoSmPhBatchColumnsP		mUkeysCollection;
	FdoSmPhCheckConstraintsP	mCkeysCollection;
	FdoStringsP					mDeletedConstraints;

};

typedef FdoPtr<FdoSmPhTable> FdoSmPhTableP;

#endif


