#ifndef FDOSMPHCFGGRDPROPERTYREADER_H
#define FDOSMPHCFGGRDPROPERTYREADER_H		1
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

#include <Sm/Ph/Cfg/PropertyReader.h>
#include <Rdbms/Override/RdbmsOvPhysicalSchemaMapping.h>

// This class is used by the Physical Schema object to query the 
// database for schema information.
//
// To perform the query, the caller creates this object with a
// GQL statement, which is parsed and executed. The caller then
// can call ReadNext() to get each row returned by the query and 
// the various Get functions to get the row data.
//
// The query is initially positioned before the first row, so 
// ReadNext must be called before data can be retrieved.

class FdoSmPhCfgGrdPropertyReader : public FdoSmPhCfgPropertyReader
{
public:
	// Create a reader to autogenerate properties based on config document directives.
	//
	// Parameters:
	//		rows: rows and fields to retrieve.
	//		schemaName: generate properties for this schema
	//		className: generate properties for this class
    //      dbObject: generate a property for each column in this database object.
    //      mgr: Physical Schema Manager
	FdoSmPhCfgGrdPropertyReader(
        FdoStringP schemaName, 
        FdoStringP className, 
        FdoSmPhDbObjectP dbObject, 
        FdoSmPhMgrP mgr
    );

    // Deactivates the query.
	~FdoSmPhCfgGrdPropertyReader(void);

protected:
    // unused constructor needed only to build on Linux
    FdoSmPhCfgGrdPropertyReader() {}

    // returns the row limit (max # of rows to sample to determine 
    // geometric property attributes).
    FdoInt32 GetRowLimit()
    {
        return mRowLimit;
    }

    // Given an associated table name, return its corresponding class name.
    virtual FdoStringP GetAssocClassName( FdoStringP tableName );

private:
    FdoInt32 mRowLimit;
};

#endif
