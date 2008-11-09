#ifndef FDOSMPHCFGPROPERTYREADER_H
#define FDOSMPHCFGPROPERTYREADER_H		1
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

#include <Sm/Ph/Rd/PropertyReader.h>

// This read autogenerates and reads properties based on config document directives.
// It creates a property for each column in the given database object.

class FdoSmPhCfgPropertyReader : public FdoSmPhRdPropertyReader
{
public:
    /// Create a reader to autogenerate properties based on config document directives.
	//
    /// Parameters:
    /// 	rows: rows and fields to retrieve.
    /// 	schemaName: generate properties for this schema
    /// 	className: generate properties for this class
    ///      dbObject: generate a property for each column in this database object.
    ///      mgr: Physical Schema Manager
	FdoSmPhCfgPropertyReader(
        FdoStringP schemaName, 
        FdoStringP className, 
        FdoSmPhDbObjectP dbObject,
        FdoSmPhMgrP mgr
    );

    /// Deactivates the query.
	~FdoSmPhCfgPropertyReader(void);

    /// Get the next row and make it current.
	virtual bool ReadNext();


protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhCfgPropertyReader() {}

    /// Various getters for current schema, class and database object
    FdoStringP GetSchemaName()
    {
        return mSchemaName;
    }

    FdoStringP GetClassName()
    {
        return mClassName;
    }

    FdoSmPhDbObjectP GetDbObject()
    {
        return mDbObject;
    }

    /// Set geometric attribute when current property is geometric
    virtual void SetGeomTypes();

    /// Given an associated table name, return its corresponding class name.
    virtual FdoStringP GetAssocClassName( FdoStringP tableName )
    {
        return L"";
    }

private:
    FdoStringP mSchemaName;
    FdoStringP mClassName;
    FdoSmPhDbObjectP mDbObject;
};

#endif


