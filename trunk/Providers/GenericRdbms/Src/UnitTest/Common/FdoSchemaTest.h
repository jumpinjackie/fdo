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

#ifndef CPP_UNIT_FDOSCHEMATEST_H
#define CPP_UNIT_FDOSCHEMATEST_H

/* 
 * A test case for the schema.
 * Loads a set of Feature schemas, in logical-physical 
 * form, from a database.
 *
 * The schemas are then written to an XML file.
 */
class FdoSchemaTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( FdoSchemaTest );
  CPPUNIT_TEST( schema );
//  CPPUNIT_TEST( UnconnectedTest );
  CPPUNIT_TEST( OverrideXmlAllTest );
  CPPUNIT_TEST( OverrideXmlErrTest );
  CPPUNIT_TEST( OverrideTablesWithOddAsciiChars );
  CPPUNIT_TEST( Test_Defect802102 );
  CPPUNIT_TEST_SUITE_END();

public:
    FdoSchemaTest(void);
    virtual ~FdoSchemaTest(void);
    void setUp ();

protected:
    virtual void set_provider() {};
	virtual void DropDatastore();

    // Unit test methods:
    virtual void schema ();
	virtual void UnconnectedTest();
	virtual void OverrideXmlAllTest();
	virtual void OverrideXmlErrTest();
    virtual void OverrideTablesWithOddAsciiChars();
    void Test_Defect802102();

    // Helper methods:
    virtual void CreateFreshDb();
    void eachOvXmlErr( FdoXmlFlags::ErrorLevel level, const char* masterFile, const char* outFile);
//	void LoadTestData(FdoIConnection* connection, FdoRdbmsSchemaManager* sm);

    bool m_bDatabaseCreated;
	static const wchar_t* mInputData[];
};

#endif // CPP_UNIT_FDOSCHEMATEST_H
