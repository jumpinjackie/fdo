#ifndef FDOSMPHRDSQSDBOBJECTBINDS_H
#define FDOSMPHRDSQSDBOBJECTBINDS_H		1
/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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

#include <Sm/Ph/Row.h>
#include <Sm/Ph/SchemaElement.h>

// This class provides support for building SqlServer selects that retrieve data for 
// an owner and a number of database objects (tables,views,etc.), where
// the owner and object names are bound into the select.
// It builds the collection of bind field objects (with bind values) and the where sub-class for 
// specifying the binds in the select. A bind field is created for the owner and 2 fields for 
// each object (1 for user name part and 1 for object name part.
class FdoSmPhRdSqsDbObjectBinds : public FdoSmPhSchemaElement
{
public:
	FdoSmPhRdSqsDbObjectBinds(
        FdoSmPhMgrP mgr,                 // Physical Schema Manager
        FdoStringP ownerFieldName,       // name of column representing the owner name.
                                         // can be qualified by table alias to resolve ambiguity
        FdoStringP ownerAlias,           // owner column alias (as defined in the select clause
                                         // of the destination select statement).
        FdoStringP userFieldName,        // name of column representing the user name part of an object.
                                         // can be qualified by table alias to resolve ambiguity
        FdoStringP userAlias,            // user name column alias (as defined in the select clause
                                         // of the destination select statement).
        FdoStringP objectFieldName,      // name of column representing the object name part of an object.
                                         // can be qualified by table alias to resolve ambiguity
        FdoStringP objectAlias,         // object name column alias (as defined in the select clause
                                         // of the destination select statement).
        FdoStringP ownerName,            // owner. A bind field is generated for this owner.
        FdoStringsP objectNames,         // list of names of objects to select 
                                         // a bind field is generated for each name.
                                         // the list can be empty, in which case no object names are bound.
        FdoSmPhRowP binds = (FdoSmPhRow*) NULL,        
                                         // initial bind field list. The owner and object binds are 
                                         // added to it.
                                         // If NULL, a new bind field collection is created.
                                         // Must not be NULL if rebind = true.
        bool rebind = false              // If true, do not create bind fields. Just bind the new 
                                         // owner and object name values to these fields.
                                         // The caller is responsible for ensuring that these fields
                                         // already exist in the binds collection passed in.
    );

    // returns the where sub-clause for defining the bind variables. Can be pasted into the 
    // destination select.
    FdoStringP GetSQL(); 

    // returns the bind fields.
    FdoSmPhRowP GetBinds();

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdSqsDbObjectBinds() {}

    virtual ~FdoSmPhRdSqsDbObjectBinds();

private:
    FdoStringP mSQL;
    FdoSmPhRowP mBinds;
};

typedef FdoPtr<FdoSmPhRdSqsDbObjectBinds> FdoSmPhRdSqsDbObjectBindsP;

#endif


