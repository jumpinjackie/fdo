#ifndef FDOSMPHSPATIALINDEX_H
#define FDOSMPHSPATIALINDEX_H		1
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

//#include <Sm/Ph/Index.h>
#include <Sm/Ph/Table.h>

// Represents a spatial index on a geometric column.
class FdoSmPhSpatialIndex : public FdoSmPhIndex
{
public:

    // Returns true if this spatial index's column is the primary geometry 
    // for its table.
    // This is encoded into the spatial index name; returns true if the name
    // contains "_gsi_" or ends with "_gsi".
    bool GetIsPrimary();

    /// Serialize the Spatial Index to an XML file.
    /// Mainly for unit testing.
	virtual void XMLSerialize( FILE* xmlFp, int ref ) const;

protected:
    /// unused constructor only for building in Linux
    FdoSmPhSpatialIndex() {}

    /// Constructs an instance of a Spatial Index object.
    /// 
    /// Parameters:
    /// 	name: Index name
    ///      pParent: the index's table.
    /// 	isUnique: true if the index is unique, false if non-unique
    ///          for most providers, index would always be non-unique
    ///      elementState: Indicates whether the index is new or already exists.
    FdoSmPhSpatialIndex(
        FdoStringP name, 
        const FdoSmPhTable* pParent,
        bool isUnique,
		FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

	~FdoSmPhSpatialIndex(void);

    /// Called when this spatial is deleted, 
    /// Removes it from its table's index cache.
    /// Also unlinks it from its geometric column.
    virtual void Discard();

private:
};

typedef FdoPtr<FdoSmPhSpatialIndex> FdoSmPhSpatialIndexP;

#endif


