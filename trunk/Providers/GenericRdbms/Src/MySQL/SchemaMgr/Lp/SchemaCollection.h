#ifndef FDOSMLPMYSQLSCHEMACOLLECTION_H
#define FDOSMLPMYSQLSCHEMACOLLECTION_H        1
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

#include "Schema.h"
#include <Sm/Lp/SchemaCollection.h>

// MySql Provider's LogicalPhysical schema collection definition
class FdoSmLpMySqlSchemaCollection : public FdoSmLpSchemaCollection
{
public:
    // Create an empty collection.
    FdoSmLpMySqlSchemaCollection(void);

    // Create a populated collection.
    //
    // Parameters:
    //      mgr: Physical Schema Manager
    FdoSmLpMySqlSchemaCollection( FdoSmPhMgrP physicalSchema, FdoSmLpSpatialContextMgrP scMgr );

    ~FdoSmLpMySqlSchemaCollection(void);


protected:
    // Create a new schema from a schema reader
    virtual FdoSmLpSchemaP NewSchema( FdoSmPhSchemaReaderP rdr );

	// Create a new logicalphysical feature schema from an FDO feature schema
    virtual FdoSmLpSchemaP NewSchema(
		FdoFeatureSchema* pFeatSchema,
//        FdoMySqlOvPhysicalSchemaMapping* pOverrides,
        bool bIgnoreStates
	);
};

typedef FdoPtr<FdoSmLpMySqlSchemaCollection> FdoSmLpMySqlSchemasP;

#endif
