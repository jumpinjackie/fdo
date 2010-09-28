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

#ifndef SQLSERVER_SCHEMAMGRTESTS_H
#define SQLSERVER_SCHEMAMGRTESTS_H

#include "SchemaMgrTests.h"

class SqlServerSchemaMgrTests : public SchemaMgrTests
{
    CPPUNIT_TEST_SUB_SUITE (SqlServerSchemaMgrTests, SchemaMgrTests);
    CPPUNIT_TEST (testSpatialContexts);
    CPPUNIT_TEST (testSpatialContextsGeog);
    CPPUNIT_TEST_SUITE_END ();

    void testSpatialContextsGeog();

    virtual int GenKeysCreateSpecific( FdoSmPhGrdOwner* grdOwner );
    virtual void GenKeysVldSpecific( FdoClassCollection* classes );

    virtual StaticConnection* CreateStaticConnection();
    virtual void CreateMultiGeomTable( FdoSmPhOwnerP owner, FdoStringP tableName, FdoInt32 colCount, FdoInt32 indexMask, FdoInt32 nnullMask );

    virtual void InsertSridRow( FdoIConnection* fdoConn, FdoStringP tableName, FdoStringP geomColumnName, int sridIndex, int expectedCount );
    virtual FdoIoStream* OverrideBend( FdoIoStream* stream1, FdoStringP oldOwnerPrefix, FdoStringP newOwnerPrefix );
    virtual void AddProviderColumns( FdoSmPhTableP table );
    virtual void VldGenGeom( FdoIConnection* conn, FdoClassDefinitionP classDef );
    void SqlServerSchemaMgrTests::VldGeomSC( 
        FdoIConnection* conn, 
        FdoClassDefinitionP classDef,
        FdoStringP propName,
        FdoStringP expCoordSys,
        double expminx,
        double expminy,
        double expmaxx,
        double expmaxy
    );
    virtual FdoStringP table2class( FdoSmPhGrdMgrP mgr, FdoStringP tableName );
    virtual FdoStringP table2qclass( FdoSmPhGrdMgrP mgr, FdoStringP datastoreName, FdoStringP tableName );
    virtual bool SupportsBaseObjects();

    virtual FdoInt64 GetSrid( int index );

};

#endif // SQLSERVER_SCHEMAMGRTESTS_H
