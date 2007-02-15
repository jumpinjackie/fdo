#ifndef FDOSMPHBASEOBJECT_H
#define FDOSMPHBASEOBJECT_H		1
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

#include <Sm/Ph/DbElement.h>

class FdoSmPhDbObject;

// This represents a base object reference (typically a reference from a view to 
// a table or view that it is derived from).
class FdoSmPhBaseObject : public FdoSmPhDbElement
{
public:
    /// Constructs an instance of a base object reference, given the referenced object's name.
    /// 
    /// Parameters:
    /// 	name: referenced object name
    /// 	parent: referencing object
    /// 	ownerName: referenced object's owner name. Defaults to referencing objects owner name.
    /// 	databaseName: referenced object's database name. Defaults to current database.
    FdoSmPhBaseObject(
        FdoStringP name, 
        FdoPtr<FdoSmPhDbObject> parent,
        FdoStringP ownerName = L"",
        FdoStringP databaseName = L""
    );

    /// Constructs an instance of a base object reference, given the referenced object.
    /// 
    /// Parameters:
    /// 	dbObject: referenced object
    /// 	parent: referencing object
    FdoSmPhBaseObject(
        FdoPtr<FdoSmPhDbObject> dbObject,
        FdoPtr<FdoSmPhDbObject> parent
    );

    /// Get the referenced object
    const FdoSmPhDbObject* RefDbObject() const;
    FdoPtr<FdoSmPhDbObject> GetDbObject();

    /// Get the referenced object's owner name
    FdoStringP GetOwnerName() const;

    /// Get the referenced object's database name
    FdoStringP GetDatabaseName() const;

protected:
    /// unused constructor needed only to build on Linux
    FdoSmPhBaseObject();

	virtual ~FdoSmPhBaseObject(void);

    // Base object references are not directly persisted in RDBMS.
    virtual bool Add() {return true;}
    virtual bool Modify() {return true;}
    virtual bool Delete() {return true;}

private:
    FdoStringP mOwnerName;
    FdoStringP mDatabaseName;

    FdoPtr<FdoSmPhDbObject> mDbObject;

};

typedef FdoPtr<FdoSmPhBaseObject> FdoSmPhBaseObjectP;

#endif


