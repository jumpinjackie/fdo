#ifndef FDOSMPHRBTABLE_H
#define FDOSMPHRBTABLE_H		1
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

#include <Sm/Ph/Rb/ColumnCollection.h>

// Contains the rollback entries for a table and its columns.
class FdoSmPhRbTable : public FdoSmSchemaElement
{
public:
    FdoSmPhRbTable(
		FdoString* name, 
		FdoSmSchemaElement* parent,
		FdoSchemaElementState elementState
	);

	~FdoSmPhRbTable(void);

    /// Returns all the rollback entries for the columns in this table.
    const FdoSmPhRbColumnCollection* RefColumns() const
	{
		return &mColumns;
	}

    FdoSmPhRbColumnCollection* GetColumns()
	{
		return &mColumns;
	}

    /// Serialize the table to an XML file.
    /// Mainly for unit testing.
	virtual void XMLSerialize( FILE* xmlFp, int ref ) const;

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhRbTable() {}

private:

	FdoSmPhRbColumnCollection mColumns;

};

typedef FdoPtr<FdoSmPhRbTable> FdoSmPhRbTableP;

#endif


