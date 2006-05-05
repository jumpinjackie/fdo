#ifndef FDOSMPHDATABASE_H
#define FDOSMPHDATABASE_H		1
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

#include <Sm/Ph/OwnerCollection.h>

class FdoSmPhMgr;
class FdoSmPhRdOwnerReader;

// This class represents a database. The exact meaning of a database is 
// provider-specific. For example, in the Oracle provider a database is 
// an Oracle instance, either the current instance for the connection or an 
// instance accessible through a database link.
//
// Each database can have a number of owners (physical schemas), which in turn have
// a number of database objects (tables, views, etc.)
class FdoSmPhDatabase : public FdoSmPhDbElement
{
public:

    /// Constructs an instance of a Database object
    /// 
    /// Parameters:
    /// 	name: database name. L"" for the default (current) database.
    /// 	mgr: the Physical Schema Manager.
    ///      elementState: determines whether this database object is new
    FdoSmPhDatabase(
        FdoStringP name, 
        FdoPtr<FdoSmPhMgr> mgr,
   		FdoSchemaElementState elementState = FdoSchemaElementState_Added

    );

	~FdoSmPhDatabase(void);

    /// Get an owner by name. 
    /// 
    /// Parameters:
    /// 	owner: owner name. L"" for the default (current) owner for this database.
    
    /// Get a read-only pointer, NULL if owner not in database
    const FdoSmPhOwner* RefOwner(FdoStringP owner) const;

    /// Get a read-write smart pointer, NULL if owner not in database
    FdoSmPhOwnerP FindOwner(FdoStringP owner);

    /// Get a read-write smart pointer, throws FdoSchemaException if owner not in database
    FdoSmPhOwnerP GetOwner(FdoStringP owner);

    /// Create a new owner.
    virtual FdoSmPhOwnerP CreateOwner(FdoStringP owner, bool hasMetaSchema = true );

    /// Deactivate the current owner leaving no owner current.
    /// Does nothing by default. Providers that can support this
    /// functionality can override.
    virtual void UnsetCurrentOwner();

    /// Remove an owner from the cache
    void DiscardOwner( FdoSmPhOwner* owner );

    /// Create a reader to get all owner objects for this database.
    /// TODO: add flag to cache owners
    virtual FdoPtr<FdoSmPhRdOwnerReader> CreateOwnerReader( FdoStringP owner = L"") const = 0;


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
    FdoSmPhDatabase() {}

    /// Returns the owner (datastore) cache.
    FdoSmPhOwnersP GetOwners();

    /// Instantiate an owner (can be new or existing).
    virtual FdoSmPhOwnerP NewOwner(
        FdoStringP owner,
        bool hasMetaSchema,
   		FdoSchemaElementState elementState = FdoSchemaElementState_Added
    ) = 0;

    /// Commit modifications to owners.
    virtual void CommitChildren( bool isBeforeParent );

private:
    /// Owner cache for this database.
	FdoSmPhOwnersP mOwners;
};

typedef FdoPtr<FdoSmPhDatabase> FdoSmPhDatabaseP;

#endif


