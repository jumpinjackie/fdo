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

#include "stdafx.h"
#include "SchemaManager.h"
#include "Lp/SpatialContextMgr.h"
#include "Lp/SchemaCollection.h"
#include "Ph/Mgr.h"

FdoSqsSchemaManager::FdoSqsSchemaManager(GdbiConnection* connection, FdoStringP SqsSchemaName) :
    FdoGrdSchemaManager( connection ),
    mSqsSchemaName(SqsSchemaName)
{
}

FdoSqsSchemaManager::~FdoSqsSchemaManager(void)
{
}

FdoSmLpSchemasP FdoSqsSchemaManager::CreateLogicalPhysicalSchemas(FdoSmPhMgrP physMgr, FdoSmLpSpatialContextMgrP scMgr)
{
    return new FdoSmLpSqsSchemaCollection(physMgr, scMgr);
}

FdoSmLpSpatialContextMgrP FdoSqsSchemaManager::CreateLpSpatialContextMgr( FdoSmPhMgrP physMgr)
{
	return new FdoSmLpSqsSpatialContextMgr(physMgr);
}

FdoSmPhMgrP FdoSqsSchemaManager::CreatePhysicalSchema()
{
    return new FdoSmPhSqsMgr(GetGdbiConnection(), mSqsSchemaName);
}

FdoStringP FdoSqsSchemaManager::GetSchemaLockStmt()
{
    return L"select schemaname from f_schemainfo with (UPDLOCK)";
}
