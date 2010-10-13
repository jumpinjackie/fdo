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

#ifndef CPP_UNIT_ADVANCEDSELECTTEST_H
#define CPP_UNIT_ADVANCEDSELECTTEST_H

/* 
 * A test case for the connection.
 */
class FdoAdvancedSelectTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( FdoAdvancedSelectTest );
  CPPUNIT_TEST( compIdentFilterTest );
  CPPUNIT_TEST( compIdentPropertyTest );
  CPPUNIT_TEST( orderByTest );
  CPPUNIT_TEST( groupByTest );
  CPPUNIT_TEST( functionTest );
  CPPUNIT_TEST( selectDistinctTest );
  CPPUNIT_TEST (TestDefect779194);
  CPPUNIT_TEST (TestDefect785616);
  CPPUNIT_TEST (getDataTypeTest);
  //CPPUNIT_TEST( orderByTest2 );
  CPPUNIT_TEST( groupByorderByTest );
  CPPUNIT_TEST( checkDataReaderContentOnSelAggRequestWithAggrFunction );
  CPPUNIT_TEST( checkDataReaderContentOnSelAggRequestWithNumCharFunction );
  CPPUNIT_TEST( checkFeatureReaderContentOnSelRequestWithAggrFunction );
  CPPUNIT_TEST( checkFeatureReaderContentOnSelRequestWithNumCharFunction );
  CPPUNIT_TEST( TestMaxBoolProperty );
  CPPUNIT_TEST( TestUpperDateProperty );
  CPPUNIT_TEST( TestCeillInt64Property );
  CPPUNIT_TEST( TestLowerOnStringProperty );
  CPPUNIT_TEST_SUITE_END();

public:
    FdoAdvancedSelectTest(void);
    virtual ~FdoAdvancedSelectTest(void);
	void setUp ();
	void tearDown ();

    virtual void compIdentFilterTest();
    virtual void compIdentPropertyTest();
    virtual void orderByTest();
    virtual void groupByTest();
    virtual void functionTest();
    virtual void selectDistinctTest();
    virtual void TestDefect779194();
    virtual void TestDefect785616();
	virtual void groupByorderByTest();
	virtual void orderByTest2();
    virtual void getDataTypeTest();
    virtual void checkDataReaderContentOnSelAggRequestWithAggrFunction();
    virtual void checkDataReaderContentOnSelAggRequestWithNumCharFunction();
    virtual void checkFeatureReaderContentOnSelRequestWithAggrFunction();
    virtual void checkFeatureReaderContentOnSelRequestWithNumCharFunction();
    virtual void TestMaxBoolProperty();
    virtual void TestUpperDateProperty();
    virtual void TestCeillInt64Property();
    virtual void TestLowerOnStringProperty();

    // Applicable only to PostgreSQL and SQLServerSpatial providers.
    void isValidTest();

    virtual FdoStringP GetSchemaName(){return L"Acad";};
    virtual FdoStringP AcDb3dPolylineName(){return L"AcDb3dPolyline";};
    virtual FdoStringP GetDateTimePropName(){return L"datetime";};
protected:
	virtual void connect ();
    
    virtual void  set_provider() {};   

    FdoPtr<FdoIConnection> mConnection;

private:


    FdoFilter* CreateFilter();
    void read_feature_data( FdoIFeatureReader *myReader, bool subset=false );
};


#endif // CPP_UNIT_ADVANCEDSELECTTEST_H
