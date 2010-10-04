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

#ifndef MYSQL_FDOAPPLYSCHEMATEST_H
#define MYSQL_FDOAPPLYSCHEMATEST_H

#include "FdoApplySchemaTest.h"
#include <Rdbms/Override/MySQL/MySqlOvPropertyMappingSingle.h>
#include <Rdbms/Override/MySQL/MySqlOvPropertyMappingConcrete.h>
#include <Rdbms/Override/MySQL/MySqlOvPhysicalSchemaMapping.h>

class MySqlFdoApplySchemaTest : public FdoApplySchemaTest
{
public:
    CPPUNIT_TEST_SUB_SUITE (MySqlFdoApplySchemaTest, FdoApplySchemaTest);
    CPPUNIT_TEST( TestCharSize );
    CPPUNIT_TEST_SUITE_END ();
	
public:
	void  set_provider();
	
    void TestCharSize();

    void TestCharSize_AddToExpected(
        FdoDictionaryP expected, 
        FdoString* key,
        FdoString* value
    );

    void TestCharSize_Validate(
        FdoDictionaryP expected,
        FdoFeatureSchemaP schema, 
        FdoSmPhOwnerP owner
    );

    // Helper methods for overrides tests; overriden by each provider:
   	virtual FdoRdbmsOvPhysicalSchemaMapping* CreateOverrideDefaults( FdoIConnection* connection, int passNum );
    virtual void CreateRdbmsSpecificElements(FdoIConnection* connection, FdoString* wDatastore);
    virtual FdoRdbmsOvClassDefinition* CreateOvClassDefinition(FdoString *name = NULL);
    virtual FdoRdbmsOvTable* CreateOvTable(FdoString* name);

    virtual void VldClassCapabilities( int ltMode, int lckMode, FdoClassDefinition* pClass );

    virtual FdoStringP SchemaTestErrFile( int fileNum, bool isMaster );
    virtual FdoStringP LogicalPhysicalBend( FdoString* inFile );
    virtual FdoStringP LogicalPhysicalFormat( FdoString* inFile );

  	virtual FdoStringP GetParcelFirstName();
	virtual FdoStringP GetParcelLastName();

    virtual bool DelayNLSSchema() {return false;};

    static FdoString*      DB_NAME_CHARSIZE_SUFFIX;

};


#endif // MYSQL_FDOAPPLYSCHEMATEST_H
