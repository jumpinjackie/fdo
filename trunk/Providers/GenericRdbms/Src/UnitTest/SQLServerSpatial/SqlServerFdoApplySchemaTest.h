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

#ifndef SQLSERVER_FDOAPPLYSCHEMATEST_H
#define SQLSERVER_FDOAPPLYSCHEMATEST_H

#include "FdoApplySchemaTest.h"
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyMappingSingle.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPropertyMappingConcrete.h>
#include <Rdbms/Override/SQLServerSpatial/SqlServerOvPhysicalSchemaMapping.h>

class SqlServerFdoApplySchemaTest : public FdoApplySchemaTest
{
public:
    CPPUNIT_TEST_SUB_SUITE (SqlServerFdoApplySchemaTest, FdoApplySchemaTest);
    CPPUNIT_TEST_SUITE_END ();

    void  set_provider();

	virtual void CreateNLSSchema( FdoIConnection* connection, StaticConnection* staticConn );

    // Helper methods for overrides tests; overriden by each provider:
   	virtual FdoRdbmsOvPhysicalSchemaMapping* CreateOverrideDefaults( FdoIConnection* connection, int passNum );
    virtual void CreateRdbmsSpecificElements(FdoIConnection* connection, FdoString* wDatastore);
    virtual FdoRdbmsOvClassDefinition* CreateOvClassDefinition(FdoString *name = NULL);
    virtual FdoRdbmsOvTable* CreateOvTable(FdoString* name);
    virtual FdoRdbmsOvColumn* CreateOvColumn(FdoString* name);

    virtual int GetLtLockMethod()
    {
        // TODO: Change back to 1 when long transactions supported by SqlServer 
        // provider.
        return 0;
    }

    virtual bool CanApplyWithoutMetaSchema();
    virtual bool CanAddNotNullCol();
    virtual bool CanDropCol();

    virtual FdoStringP SchemaTestErrFile( int fileNum, bool isMaster );
    virtual FdoStringP SchemaOvErrFile( int fileNum, bool isMaster );
    virtual FdoStringP LogicalPhysicalBend( FdoString* inFile );
    virtual FdoStringP LogicalPhysicalFormat( FdoString* inFile );
};


#endif // SQLSERVER_FDOAPPLYSCHEMATEST_H
