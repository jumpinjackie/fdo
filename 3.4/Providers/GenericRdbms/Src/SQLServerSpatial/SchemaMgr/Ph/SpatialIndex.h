#ifndef FDOSMPHSQSSPATIALINDEX_H
#define FDOSMPHSQSSPATIALINDEX_H       1
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
#include <Sm/Ph/SpatialIndex.h>
#include "DbObject.h"

// Oracle Provider implementation of a Spatial Index.
class FdoSmPhSqsSpatialIndex : public FdoSmPhSpatialIndex, public FdoSmPhSqsDbObject
{
public:
    // Constructs an instance of a Spatial Index object.
    //
    // Parameters:
    //      name: Index name
    //      pParent: the index's table.
    //      elementState: Indicates whether the index is new or already exists.

    FdoSmPhSqsSpatialIndex(
        FdoStringP name,
        const FdoSmPhTable* pParent,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

    ~FdoSmPhSqsSpatialIndex(void);

    // Get unqualified index name for use in DDL statement
    virtual FdoStringP GetDDLName() const;

    // Returns true if index is rtree, false if qtree.
    bool GetIsRtree() const;

    // Set the index type. An exception is thrown if this is not a 
    // new index. Type modification is not supported
    void SetIsRtree( 
        bool isRtree //true: rtree, false: quadtree
    );

    // Returns true if the index is 3d, false if 2d.
    bool GetIs3d() const;

    // Set the index dimensionality. An exception is thrown if this is not a 
    // new index. Dimensionality modification is not supported
    void SetIs3d( bool is3d );

	// Gather all errors for this element and child elements into a chain of exceptions.
	// Adds each error as an exception, to the given exception chain and returns
	// the chain.
	//
	// parameters:
	//		pFirstException: a chain of exceptions.
	virtual FdoSchemaExceptionP Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;

protected:
    // unused constructor needed only to build on Linux
//    FdoSmPhSqsSpatialIndex() {}

    // Override column reader creator to return empty reader.
    // Parent table handles the adding of the right columns to each index.
    virtual FdoPtr<FdoSmPhRdColumnReader> CreateColumnReader();

    virtual bool Add();
    virtual bool Modify() {return true;}
    virtual bool Delete();

private:
    bool mIsRtree;
    bool mIs3d;
};

typedef FdoPtr<FdoSmPhSqsSpatialIndex> FdoSmPhSqsSpatialIndexP;

#endif
