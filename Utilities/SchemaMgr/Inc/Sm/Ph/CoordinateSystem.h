#ifndef FDOSMPHCOORDINATESYSTEM_H
#define FDOSMPHCOORDINATESYSTEM_H		1
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

#include <Sm/Ph/Mgr.h>

// This Physical class encapsulates a Coordinate System from the RDBMS.
class FdoSmPhCoordinateSystem : public FdoSmPhSchemaElement
{
public:
    ~FdoSmPhCoordinateSystem(void);

    /// Returns the Well-know text defining the coordinate system
    FdoString* GetWkt();

    /// returns the Spatial Reference Identifier
    FdoInt64 GetSrid();

    /// Constructs an instance from the given attributes.
	FdoSmPhCoordinateSystem(
        FdoSmPhMgrP phMgr,
        FdoString* name,
        FdoString* description,
        FdoInt64 srid,
        FdoString* Wkt
    );

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhCoordinateSystem() {}

private:
    FdoStringP                  mWkt;
	FdoInt64                    mSrid;			// Coordinate system SRID. 
};

typedef FdoPtr<FdoSmPhCoordinateSystem> FdoSmPhCoordinateSystemP;

#endif


