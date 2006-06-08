#ifndef FDOSMPHOWNER_H
#define FDOSMPHOWNER_H		1
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

#include <Sm/Ph/DbObjectCollection.h>
#include <Sm/Ph/LockTypesCollection.h>
#include <Sm/Ph/Table.h>
#include <Sm/Ph/View.h>

static const FdoStringP FDOSYS_OWNER = L"FDOSYS";

class FdoSmPhDatabase;
class FdoSmPhRdDbObjectReader;
class FdoSmPhRdCoordSysReader;
class FdoSmPhRdConstraintReader;

// This class represents an Owner (Physical Schema). The exact meaning
// of Owner depends on the Provider. For example, in the Oracle Provider
// it represents an Oracle schema (user).
//
// An owner can contain a number of database objects (tables, views, etc.).
class FdoSmPhOwner : public FdoSmPhDbElement
{
public:

    /// Constructs an instance of an Owner object.
    /// 
    /// Parameters:
    /// 	name: owner name
    /// 	pDatabase: database containing this owner.
    ///      elementState: determines whether this database object is new
    FdoSmPhOwner(
        FdoStringP name, 
        bool hasMetaSchema,
        const FdoSmPhDatabase* pDatabase,
   		FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

	~FdoSmPhOwner(void);

    /// Get/Set the owner's password
    void SetPassword( FdoStringP password );
    FdoStringP GetPassword();

    /// Get/Set whether the owner has MetaSchema tables.
    /// The Set function can only be called when this owner's
    /// element state is FdoSchemaElementState_Added.
    void SetHasMetaSchema( bool hasMetaSchema );
    bool GetHasMetaSchema();

    /// System database flag
	void SetIsSystem( bool IsSystem );
	bool GetIsSystem();

    /// Returns the MetaSchema version for this Owner.
    /// Returns 0.0 if this owner has no MetaSchema.
    double GetSchemaVersion();

    /// Returns the description of the owner from  the MetaSchema.
    /// Returns L"" if this owner has no MetaSchema or description
    /// is actually blank.
    virtual FdoString* GetDescription() const;

    /// Long Transaction and Locking methods set/get
	void SetLtMode( FdoLtLockModeType LtMode );
	FdoLtLockModeType  GetLtMode();

	void SetLckMode( FdoLtLockModeType LckMode );
	FdoLtLockModeType GetLckMode();

    /// Given a lock mode, return an array of all lock types supported for this mode.
    /// The list depends on the current RDBMS.
    /// size is set to the size of the returned array.
    /// NULL is returned when size is 0.
	virtual const FdoLockType* GetLockTypes( FdoLtLockModeType lckMode, FdoInt32& size);

    /// Get a database object ...

    /// as a read-only pointer (NULL if not found)
    const FdoSmPhDbObject* RefDbObject(FdoStringP dbObject) const;

    /// as a read-write smart pointer (NULL if not found)
    FdoSmPhDbObjectP FindDbObject(FdoStringP dbObject);

    /// as a read-write smart pointer (exception thrown if not found)
    FdoSmPhDbObjectP GetDbObject(FdoStringP dbObject);

    // Reverse-engineers an FDO feature schema name from this datastore.
    // Default implementation returns datastore name prepended by "Fdo".
    // "Fdo" is prepended to prevent name conflict with special schema 
    // that exists in all datastores with metaschema. The special schema
    // holds the datastore description.
    virtual FdoStringP GetBestSchemaName() const;

    /// Convert the given database object name to one that is unique and provider-acceptable.
	FdoStringP UniqueDbObjectName( FdoStringP objectName );

    /// Make this owner the current owner
    virtual void SetCurrent() {}

    /// Create a reader to get all database objects for this owner.
    /// TODO: add flag to cache objects.
    virtual FdoPtr<FdoSmPhRdDbObjectReader> CreateDbObjectReader( FdoStringP dbObject = L"") const = 0;

    /// Create a reader to get one or more coordinate system(s) for this owner.
    virtual FdoPtr<FdoSmPhRdCoordSysReader> CreateCoordSysReader( FdoStringP csysName = L"") const;

    /// Create a reader to get all constraints for this owner.
    /// TODO: add flag to cache objects.
    virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader( FdoStringP constraintName ) const = 0;
 
    /// Create a reader to get all constraints for this owner and this table.
    virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader( FdoStringP tableName, FdoStringP constraintType ) const = 0;

    /// Create a new table. Table is not posted to the datastore until its Commit() function
    /// is called.
    FdoSmPhTableP CreateTable(
        FdoStringP tableName,
        FdoStringP pkeyName = L""
    );

    /// Create a new view. View is not posted to the datastore until its Commit() function
    /// is called.
    FdoSmPhViewP CreateView( 
        FdoStringP viewName, 
        FdoStringP rootDatabase,
        FdoStringP rootOwner,
        FdoStringP rootObjectName
    );

    /// Given a DbObject reader, add its current database object to 
    /// this owner's cache.
    FdoSmPhDbObjectP CacheDbObject(
        FdoPtr<FdoSmPhRdDbObjectReader> reader
    );

    /// Remove a database object from the cache.
    void DiscardDbObject( FdoSmPhDbObject* dbObject );

    /// Gather all errors for this element and child elements into a chain of exceptions.
    /// Adds each error as an exception, to the given exception chain and returns
    /// the chain.
	//
    /// parameters:
    /// 	pFirstException: a chain of exceptions.
	virtual FdoSchemaExceptionP Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;

    /// Remove this owner from its database.
    virtual void Discard();

    /// Serialize the table to an XML file.
    /// Mainly for unit testing.
	virtual void XMLSerialize( FILE* xmlFp, int ref ) const;

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhOwner() {}

    /// Load the lists of lock types per locking mode
    virtual void LoadLckTypes( FdoLtLockModeType lockMode );

    /// Returns a pointer to the database object list
    FdoSmPhDbObjectsP GetDbObjects();

    /// Get the lists of lock types per locking mode.
    FdoSmPhLockTypesCollection* GetLockTypesCollection();

    /// table and view creation functions which each provider
    /// must implement
    virtual FdoSmPhDbObjectP NewTable(
		FdoStringP tableName, 
		FdoSchemaElementState elementState,
        FdoSmPhRdDbObjectReader* reader
	) = 0;

    virtual FdoSmPhDbObjectP NewView(
		FdoStringP viewName, 
        FdoStringP rootDatabase,
        FdoStringP rootOwner,
        FdoStringP rootObjectName,
		FdoSchemaElementState elementState,
        FdoSmPhRdDbObjectReader* reader
	) = 0;

    virtual FdoSmPhDbObjectP NewDbObject(
        FdoStringP objName,
        FdoSchemaElementState elementState,
        FdoPtr<FdoSmPhRdDbObjectReader> reader
    );

    /// Commit modifications to database objects.
    virtual void CommitChildren( bool isBeforeParent );

    /// Commit modifications to foreign keys.
    virtual void CommitFkeys( bool isBeforeParent );

    /// Reserve the given database object name. Prevents UniqueDbObjectName from generating 
    /// this name.
	//
    /// Parameters
    /// 	objectName - the name to reserve
    /// 	bCheckReserved
    /// 		true - reserve only if not already reserved.
    /// 		false - skip the "is reserved" check and just add the table name
    /// 			to the in-memory reserved list. Useful when we know it is not
    /// 			currently reserved and we want to avoid the overhead of the 
    /// 			reserved check.
	void ReserveDbObjectName( FdoStringP objectName, bool bCheckReserved = true );

    /// Returns true if the given database object name is reserved (one of the following is true):
    /// 	- the object has been reserved by ReserveDbObjectName
    /// 	- the object exists in this owner (datastore)
    /// 	- the object is referenced by the metaschema tables in the current datastore.
	bool IsDbObjectNameReserved( FdoStringP objectName );

private:
    /// Load Schema Information
    void LoadSchemaInfo();

    /// Load the long transaction and locking settings.
    void LoadLtLck();

	FdoSmPhDbObjectsP mDbObjects;
	FdoStringsP mReservedDbObjectNames;
    FdoStringP mPassword;
    bool mHasMetaSchema;
    /// FDOSYS database flag
	bool mIsSystem;

    double mSchemaVersion;

	FdoLtLockModeType mLtMode;
	FdoLtLockModeType mLckMode;
    FdoSmPhLockTypesCollectionP mLockTypes;
    bool mSchemaInfoLoaded;
    bool mLtLckLoaded;
};

typedef FdoPtr<FdoSmPhOwner> FdoSmPhOwnerP;

#endif


