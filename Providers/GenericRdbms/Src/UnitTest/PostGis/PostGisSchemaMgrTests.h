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

#ifndef POSTGIS_SCHEMAMGRTESTS_H
#define POSTGIS_SCHEMAMGRTESTS_H

#include "SchemaMgrTests.h"

class PostGisSchemaMgrTests : public SchemaMgrTests
{
    CPPUNIT_TEST_SUB_SUITE (PostGisSchemaMgrTests, SchemaMgrTests);
    CPPUNIT_TEST_SUITE_END ();

    // TestViews N/A since tests views with cross-database dependencies.
    virtual void testViews() {};
    // TODO: re-add test when PostgreSQL provider handles foreign keys.
    virtual void testFKeys() {};

	virtual StaticConnection* CreateStaticConnection();
    virtual FdoIoStream* OverrideBend( FdoIoStream* stream1, FdoStringP oldOwnerPrefix, FdoStringP newOwnerPrefix );
    virtual void AddProviderColumns( FdoSmPhTableP table );
    virtual void VldGenGeom( FdoClassDefinitionP classDef );
    virtual void testConfigError(){};

    virtual bool SupportsFkeyDoubleDecimal() {return false;};
    virtual bool SupportsCrossDatastoreDependencies() {return false;};
    virtual bool SupportsBaseObjects() {return true;};

    virtual FdoStringP table2class( FdoSmPhGrdMgrP mgr, FdoStringP tableName );

    virtual FdoStringP table2qclass( FdoSmPhGrdMgrP mgr, FdoStringP datastoreName, FdoStringP tableName );
    
    virtual FdoStringP GetIndexName( FdoSmPhMgrP mgr, FdoStringP indexName );
};

#endif // PostGis_SCHEMAMGRTESTS_H
