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

#ifndef MYSQL_SCHEMAMGRTESTS_H
#define MYSQL_SCHEMAMGRTESTS_H

#include "SchemaMgrTests.h"

class MySqlSchemaMgrTests : public SchemaMgrTests
{
    CPPUNIT_TEST_SUB_SUITE (MySqlSchemaMgrTests, SchemaMgrTests);
    CPPUNIT_TEST (testWideConstraint);
    CPPUNIT_TEST_SUITE_END ();

    void testWideConstraint();
    
	virtual StaticConnection* CreateStaticConnection();
    virtual FdoIoStream* OverrideBend( FdoIoStream* stream1, FdoStringP oldOwnerPrefix, FdoStringP newOwnerPrefix );
    virtual void AddProviderColumns( FdoSmPhTableP table );
    virtual void VldGenGeom( FdoClassDefinitionP classDef );
    virtual void testConfigError(){};
};

#endif // MYSQL_SCHEMAMGRTESTS_H
