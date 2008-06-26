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

#ifndef CPP_UNIT_FDOSPATIALCONTEXTTEST_H
#define CPP_UNIT_FDOSPATIALCONTEXTTEST_H

class StaticConnection;

/* 
 * A test case for the schema.
 * Loads a set of Feature schemas, in logical-physical 
 * form, from a database.
 *
 * The schemas are then written to an XML file.
 */
class FdoSpatialContextTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( FdoSpatialContextTest );
  CPPUNIT_TEST( testMeta );
  CPPUNIT_TEST( testNoMeta );
  CPPUNIT_TEST_SUITE_END();

public:
    FdoSpatialContextTest(void);
    virtual ~FdoSpatialContextTest(void);
    void setUp ();

protected:
    virtual void set_provider() {};

    // Unit test methods:
    virtual void testMeta ();
	virtual void testNoMeta();

    // Helper methods:
    virtual void DoTest( bool hasMetaSchema );
    virtual void CreateSchema( FdoIConnection* connection, bool hasMetaSchema );
    virtual void InsertFeatures( FdoIConnection* connection );

    virtual void CreateTables( StaticConnection* staticConn, FdoStringP datastore );

    FdoStringP GenFileName( int fileNum, bool hasMetaSchema, bool isMaster );

    static FdoString* FdoSpatialContextTest::DB_NAME_SUFFIX;

};

#endif // CPP_UNIT_FDOSPATIALCONTEXTTEST_H
