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

#ifndef SCHEMAMGRTESTS_H
#define SCHEMAMGRTESTS_H

#include <Sm/SchemaManager.h>
#include "../SchemaMgr/Ph/Mgr.h"
class StaticConnection;

class SchemaMgrTests : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE (SchemaMgrTests);
    CPPUNIT_TEST (testGenDefault);
    CPPUNIT_TEST (testGenGeom);
    CPPUNIT_TEST (testGenConfig1);
    CPPUNIT_TEST (testGenKeys);
    CPPUNIT_TEST (testConfigError);
//    CPPUNIT_TEST (constraints);
//    CPPUNIT_TEST (delFdoBr);
    CPPUNIT_TEST_SUITE_END ();

public:
    SchemaMgrTests (void);
    virtual ~SchemaMgrTests (void);
    void setUp ();
    void tearDown ();

protected:
    void testGenDefault ();
    void testGenGeom ();
    void testGenConfig1 ();
    void testGenConfigGeom ();
    void testGenKeys();
    virtual void testConfigError();

    void constraints ();
    void delFdoBr ();
    virtual FdoIoStream* OverrideBend( FdoIoStream* stream1, FdoStringP oldOwnerPrefix, FdoStringP newOwnerPrefix )
    {
        return NULL;
    }

    virtual StaticConnection* CreateStaticConnection()
    {
        return NULL;
    }

    virtual FdoIoStreamP CvtConf( FdoIoStreamP streamIn ); 

    virtual void AddProviderColumns( FdoSmPhTableP table );

    void CreateMultiGeomTable( FdoSmPhOwnerP owner, FdoStringP tableName, FdoInt32 colCount, FdoInt32 indexMask, FdoInt32 nnullMask );

    virtual void VldGenGeom( FdoClassDefinitionP classDef )
    {
    }

    virtual FdoStringP table2class( FdoSmPhGrdMgrP mgr, FdoStringP tableName );

    virtual void VldLtReaders( StaticConnection* conn, FdoStringP datastore ) 
    {
    }

private:
    void CreateTableGroup( FdoSmPhOwnerP owner, FdoStringP prefix, FdoInt32 count, int lt_mode );
    void CreateFkey( FdoSmPhOwnerP owner, FdoStringP fTableName, FdoStringP pTableName );
    FdoSmPhTableP CreateIxTable( FdoSmPhOwnerP owner, FdoStringP tableName, int lt_mode );
    void AddPkey( FdoSmPhTableP table );
    void AddIndex( FdoSmPhTableP table, bool unique, FdoStringP indexName, FdoStringP columns );
    void SetLtLck( FdoSmPhTableP table, int lt_mode );
};

#endif // SCHEMAMGRTESTS_H

