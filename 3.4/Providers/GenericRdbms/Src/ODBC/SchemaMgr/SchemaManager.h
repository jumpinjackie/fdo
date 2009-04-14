#ifndef FDOSMOdbcSCHEMAMANAGER_H
#define FDOSMOdbcSCHEMAMANAGER_H     1
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
#include "../../SchemaMgr/SchemaManager.h"

// Schema Manager for Odbc Provider
class FdoOdbcSchemaManager : public FdoGrdSchemaManager
{
public:
    FdoOdbcSchemaManager(
        GdbiConnection* connection,     // allows access to the Odbc RDBMS
        FdoStringP oraSchemaName        // default physical schema for current Odbc connection.
    );


protected:
    virtual ~FdoOdbcSchemaManager(void);

    // Create Odbc Provider's LogicalPhysical Schema collection.
    virtual FdoSmLpSchemasP CreateLogicalPhysicalSchemas(FdoSmPhMgrP physMgr, FdoSmLpSpatialContextMgrP scMgr);

    // Create Odbc Provider's Physical Schema Manager.
    virtual FdoSmPhMgrP CreatePhysicalSchema();

    // Get the statement for locking F_SchemaInfo
	virtual FdoStringP GetSchemaLockStmt();

private:
    FdoStringP mOdbcSchemaName;
};

#endif

