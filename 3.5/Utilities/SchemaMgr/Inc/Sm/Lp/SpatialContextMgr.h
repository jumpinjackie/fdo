#ifndef FDOSMLPSPATIALCONTEXTMGR_H
#define FDOSMLPSPATIALCONTEXTMGR_H		1
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

class FdoSmLpSpatialContextMgr;
typedef FdoPtr<FdoSmLpSpatialContextMgr> FdoSmLpSpatialContextMgrP;

#include <Sm/Lp/SpatialContextCollection.h>

class FdoSmLpSpatialContextMgr : public FdoSmDisposable
{
public:

    FdoSmLpSpatialContextMgr(FdoSmPhMgrP phMgr);
	~FdoSmLpSpatialContextMgr(void);

    // Given a Spatial Context id, return the spatial context.
    // Returns NULL if the spatial contexts does not exist.
    FdoSmLpSpatialContextP FindSpatialContext( FdoInt64 scId );

    // Given a Spatial Context name, return the spatial context.
    // Returns NULL if the spatial context does not exist.
    FdoSmLpSpatialContextP FindSpatialContext( FdoStringP scName );

    // Given database object and column name, returns the association to spatial context.
    // Returns NULL if the column is not geometric or has no associated spatial context.
    FdoSmLpSpatialContextGeomP FindSpatialContextGeom( FdoStringP dbObjectName, FdoStringP columnName );

    // Gets all spatial contexts
    FdoSmLpSpatialContextsP GetSpatialContexts();

    // Clears the spatial context cache
    void Clear();

protected:
    FdoSmLpSpatialContextMgr() {}

    void CreateSpatialContexts();
    virtual FdoSmLpSpatialContextsP NewSpatialContexts(FdoSmPhMgrP phMgr);

private:
    FdoSmPhMgrP             mPhMgr;
    FdoSmLpSpatialContextsP mSpatialContexts;
};


#endif


