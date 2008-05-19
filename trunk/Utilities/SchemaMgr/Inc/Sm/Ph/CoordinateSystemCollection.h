#ifndef FDOSMPHCOORDINATESYSTEMCOLLECTION_H
#define FDOSMPHCOORDINATESYSTEMCOLLECTION_H		1
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

#include <Sm/Ph/CoordinateSystem.h>
#include <Sm/NamedCollection.h>

// Represents a collection of Coordinate Systems.
class FdoSmPhCoordinateSystemCollection : public FdoSmNamedCollection<FdoSmPhCoordinateSystem>
{
public:
    /// Create an empty collection.
	FdoSmPhCoordinateSystemCollection(void);

	virtual ~FdoSmPhCoordinateSystemCollection(void);

    // Given a  Spatial Reference ID, return the coordinate system.
    // Returns NULL if coordinate system not found.
    FdoSmPhCoordinateSystemP FindItemById( FdoInt64 srid );

    // Given a  well-known text string, return the coordinate system.
    // Returns NULL if coordinate system not found.
    FdoSmPhCoordinateSystemP FindItemByWkt( FdoStringP wkt );

    // Overridden to update coordinate system id to name map.
    virtual FdoInt32 Add( FdoSmPhCoordinateSystem* value);

private:
    // Functions for maintaining the lookup by id map.
    void AddToIdMap( FdoSmPhCoordinateSystem* coordSys );
    void RemoveFromIdMap( FdoSmPhCoordinateSystem* coordSys );

    // MAP for fast lookup of coordinate systems by id.
    FdoDictionaryP mIdMap;
};

typedef FdoPtr<FdoSmPhCoordinateSystemCollection> FdoSmPhCoordinateSystemsP;


#endif


