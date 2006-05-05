#ifndef FDOSMGRDSCHEMAMANAGER_H
#define FDOSMGRDSCHEMAMANAGER_H     1
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
#include <Sm/SchemaManager.h>
#include <Inc/Rdbi/context.h>
#include "../Gdbi/GdbiTypes.h"
#include "../Gdbi/GdbiQueryResult.h"
#include "../Gdbi/GdbiStatement.h"
#include "../Gdbi/GdbiConnection.h"

// This is the base class for RDBMS type provider Schema Managers
class FdoGrdSchemaManager : public FdoSchemaManager
{
public:
	// Apply updates to the given schema to the FDO DataStore.
    //
    // Parameters
    //      pFeatSchema: the Feature schema.
    //      pOverrides: Fdo to physical mapping overrides for the feature schema.
	virtual void ApplySchema( 
        FdoFeatureSchemaP pFeatSchema, 
        FdoPhysicalSchemaMappingP pOverrides,
        bool bIgnoreStates = false
    );

    // Synchronize the physical tables and columns with the MetaSchema.
    // Parameters:
    //      schemaName - the feature schema to synchronize. If "" then all feature schemas
    //          are synchronized.
    //      bRollbackOnly - 
    //          true - only synchronize elements that may have been rolled back..
    //          false - synchronize all elements.
    void SynchPhysical( const wchar_t* schemaName = L"", bool bRollbackOnly = true );
	
    // Returns the RDBMS connection being managed.
    GdbiConnection* GetGdbiConnection()
    {
        return mGdbiConnection;
    }

protected:
    // unused constructor needed only to build Linux
    FdoGrdSchemaManager() {}

    // Create a Schema Manager
    FdoGrdSchemaManager(
        GdbiConnection* connection // for accessing the RDBMS
    );


    virtual ~FdoGrdSchemaManager(void);

    // Get the statement for locking F_SchemaInfo
	virtual FdoStringP GetSchemaLockStmt();


private:
    GdbiConnection* mGdbiConnection;
};

#endif

