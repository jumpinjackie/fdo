#ifndef FDORDBMSPHINDEXWRITER_H
#define FDORDBMSPHINDEXWRITER_H		1
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
#include "IndexPropertyWriter.h"

// This class writes index definitions 
// to the f_indexdefinition table.

class FdoRdbmsPhIndexWriter :
	public FdoRdbmsPhCommandWriter
{
public:
    /// Create an index definition writer.
	//
    /// Parameters:
    /// 	physicalSchema: holds the connection for the database to query.
	FdoRdbmsPhIndexWriter(FdoRdbmsPhysicalSchema* physicalSchema);

	~FdoRdbmsPhIndexWriter(void);

    /// Various getters for Index attributes

    /// Index name
	FdoStringP GetName();

    /// true if Index is unique
    bool GetIsUnique();

    /// Owning class id
	dbi_cn_id_t  GetClassId();


    /// Corresponding setter functions

    /// Index name
	void SetName( FdoStringP sValue );

    /// Owning class id
	void  SetClassId( dbi_cn_id_t iValue );

    void SetIsUnique( bool bValue );

    /// Add the current row as a new index definition
	virtual void Add();

    /// Modify a row with the current field values
    //
    /// Parameters
    ///      sName: name of index to modify
    virtual void Modify( FdoStringP sName );

    /// Delete a row with the current field values
    //
    /// Parameters
    ///      sName: name of index to delete
    virtual void Delete( FdoStringP sName );

    /// Get a field list for reading from or writing to 
    /// f_indexdefinition.
	//
    /// Parameters:
    /// 	physicalSchema: holds the connection for the database to query.
    static FdoRdbmsPhFromInto* GetFromInto( FdoRdbmsPhysicalSchema* physicalSchema );
};

#endif


