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

#include "stdafx.h"
#include <Sm/Lp/SpatialContextMgr.h>


FdoSmLpSpatialContextMgr::FdoSmLpSpatialContextMgr(FdoSmPhMgrP phMgr)
{
    mPhMgr = phMgr;
}

FdoSmLpSpatialContextMgr::~FdoSmLpSpatialContextMgr(void)
{
}

FdoSmLpSpatialContextP FdoSmLpSpatialContextMgr::FindSpatialContext( FdoInt64 scId )
{
    CreateSpatialContexts();

    return mSpatialContexts->FindSpatialContext(scId);
}

FdoSmLpSpatialContextP FdoSmLpSpatialContextMgr::FindSpatialContext( FdoStringP scName )
{
    CreateSpatialContexts();

    return mSpatialContexts->FindSpatialContext(scName);
}

FdoSmPhSpatialContextGeomP FdoSmLpSpatialContextMgr::FindSpatialContextGeom( FdoStringP dbObjectName, FdoStringP columnName )
{
    CreateSpatialContexts();

    return mSpatialContexts->FindSpatialContextGeom(dbObjectName, columnName);
}

FdoSmLpSpatialContextsP FdoSmLpSpatialContextMgr::GetSpatialContexts()
{
    CreateSpatialContexts();
    mSpatialContexts->Load();

    return mSpatialContexts;
}

void FdoSmLpSpatialContextMgr::CreateSpatialContexts()
{
    if ( !mSpatialContexts )
        mSpatialContexts = NewSpatialContexts(mPhMgr);
}

FdoSmLpSpatialContextsP FdoSmLpSpatialContextMgr::NewSpatialContexts(FdoSmPhMgrP phMgr)
{
    return new FdoSmLpSpatialContextCollection(phMgr);
}

void FdoSmLpSpatialContextMgr::Clear()
{
    mSpatialContexts = NULL;
}
