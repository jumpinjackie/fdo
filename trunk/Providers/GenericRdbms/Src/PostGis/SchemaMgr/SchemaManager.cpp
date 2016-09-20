//
// Copyright (C) 2006 Refractions Research, Inc.
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
//
#include "stdafx.h"
#include "SchemaManager.h"
#include "Lp/SchemaCollection.h"
#include "Ph/Mgr.h"

#include <cassert>

FdoPostGisSchemaManager::FdoPostGisSchemaManager()
    : FdoGrdSchemaManager(NULL)
{
    // idle
}

FdoPostGisSchemaManager::FdoPostGisSchemaManager(GdbiConnection* connection,
    FdoStringP schemaName)
    : FdoGrdSchemaManager(connection),
      mPostGisSchemaName(schemaName)
{
    assert(NULL != connection);
}

FdoPostGisSchemaManager::~FdoPostGisSchemaManager()
{
    // idle
}

FdoSmLpSchemasP FdoPostGisSchemaManager::CreateLogicalPhysicalSchemas(FdoSmPhMgrP physMgr,
        FdoSmLpSpatialContextMgrP scMgr)
{
    return new FdoSmLpPostGisSchemaCollection(physMgr, scMgr);
}

FdoSmPhMgrP FdoPostGisSchemaManager::CreatePhysicalSchema()
{
    FdoSmPhPostGisMgr* pgMgr = NULL;
    pgMgr = new FdoSmPhPostGisMgr(GetGdbiConnection(), mPostGisSchemaName);
    return pgMgr;
}

