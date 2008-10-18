#ifndef FDOSMPHSPATIALCONTEXTCOLLECTION_H
#define FDOSMPHSPATIALCONTEXTCOLLECTION_H		1
//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Ph/SpatialContext.h>
#include <Sm/NamedCollection.h>

// Represents a collection of Spatial Contexts in Physical form.
class FdoSmPhSpatialContextCollection : public FdoSmNamedCollection<FdoSmPhSpatialContext>
{
public:
    /// Create an empty collection.
	FdoSmPhSpatialContextCollection(void);

	virtual ~FdoSmPhSpatialContextCollection(void);

    // Given a Spatial Context ID, return the spatial context.
    // Returns NULL if spatial context not found.
    FdoSmPhSpatialContextP FindItemById( FdoInt64 scid );

    // Overridden to update spatial context id to name map.
    virtual FdoInt32 Add( FdoSmPhSpatialContext* value);

	FdoInt32 FindExistingSC( FdoSmPhSpatialContextP sc );

    // Generates a name for the next spatial context
    // to add to this collection. Ensures that the name is 
    // not already used by an item in this collection.
    FdoStringP AutoGenName();

private:
    // Functions for maintaining the lookup by id map.
    void AddToIdMap( FdoSmPhSpatialContext* sc );
    void RemoveFromIdMap( FdoSmPhSpatialContext* sc );

    // MAP for fast lookup of spatial contexts by id.
    FdoDictionaryP mIdMap;

    FdoInt32 mNextAutoNum;
};

typedef FdoPtr<FdoSmPhSpatialContextCollection> FdoSmPhSpatialContextsP;


#endif


