#ifndef FDORDBMSPHINDEXIndexPropertyWRITER_H
#define FDORDBMSPHINDEXIndexPropertyWRITER_H		1
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

#include "FromInto.h"
#include "CommandWriter.h"

// This class writes the properties for indexes to F_IndexAttribute.

class FdoRdbmsPhIndexPropertyWriter :
	public FdoRdbmsPhCommandWriter
{
public:
    /// Create an attribute definition writer.
	//
    /// Parameters:
    /// 	physicalSchema: holds the connection for the database to query.
	FdoRdbmsPhIndexPropertyWriter(FdoRdbmsPhysicalSchema* physicalSchema);

	~FdoRdbmsPhIndexPropertyWriter(void);

    /// Various getters for IndexProperty attributes

    /// Index name
	FdoStringP GetIndexName();

    /// Property name 
	FdoStringP GetAttributeName();

    /// 1-based position of property in its index
	int GetPosition();

    /// Corresponding setter functions

	void SetIndexName( FdoStringP sValue );

	void SetAttributeName( FdoStringP sValue );

	void SetPosition( int iValue );

    /// Add the current row as a new index property definition
	virtual void Add();

    /// Modify a row with the current field values
    //
    /// Parameters
    ///      sIndexName, position: primary key of the row to modify
    virtual void Modify( FdoStringP sIndexName, int position );

    /// Delete a row with the current field values
    //
    /// Parameters
    ///      sIndexName, position: primary key of the row to delete
    virtual void Delete( FdoStringP sIndexName, int position );

    /// Delete all properties for the given index
    //
    /// Parameters
    ///      sIndexName: the index.
    virtual void Delete( FdoStringP sIndexName );

    /// Get a field list for reading from or writing to 
    /// f_indexattribute.
	//
    /// Parameters:
    /// 	physicalSchema: holds the connection for the database to query.
    static FdoRdbmsPhFromInto* GetFromInto( FdoRdbmsPhysicalSchema* physicalSchema );
};

#endif


