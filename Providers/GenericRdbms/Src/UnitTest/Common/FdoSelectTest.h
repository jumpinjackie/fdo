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

#ifndef CPP_UNIT_FDOSELECTTEST_H
#define CPP_UNIT_FDOSELECTTEST_H

/*
 * A test case for the connection.
 */
class FdoSelectTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( FdoSelectTest );
#if 0
  CPPUNIT_TEST(feature_subset_query2);
#endif
  CPPUNIT_TEST(non_feature_subset_query);
  CPPUNIT_TEST( feature_subset_query );
  CPPUNIT_TEST( feature_query );
  CPPUNIT_TEST( non_feature_query );
 // CPPUNIT_TEST( distance_query );
  CPPUNIT_TEST( spatial_query );
 // CPPUNIT_TEST(feature_object_query);
//  CPPUNIT_TEST(feature_select_obj_distinct);
  CPPUNIT_TEST(feature_geom_query);
  CPPUNIT_TEST(concurrent_select);
  CPPUNIT_TEST_SUITE_END();

public:
    FdoSelectTest(void);
    FdoSelectTest(char* suffix);
    virtual ~FdoSelectTest(void);
    void setUp ();
    void tearDown ();

    virtual void connect ();
    virtual void feature_query ();
    virtual void non_feature_query ();
    virtual void distance_query ();
    virtual void spatial_query ();
    virtual void feature_object_query();

    virtual void feature_subset_query ();
    virtual void non_feature_subset_query ();
    virtual void feature_subset_query2();

    virtual void feature_select_obj_distinct();

    virtual void feature_geom_query ();
    virtual void concurrent_select ();
	virtual FdoString* GetClassName (){return L"Acad:testClass";}

    // Tests that are common to some providers but not others;
    // Each provider that wants to run a common test should add a CPPUNIT_TEST() macro entry for it:
    virtual void TestDateFilter();

protected:
    FdoPtr<FdoIConnection> mConnection;
    virtual void  set_provider() {};   

	static FdoFilter* CreateFilter();

private:

    void read_feature_data( FdoIFeatureReader *myReader, bool subset = false );
    void read_non_feature_data( FdoIFeatureReader *myReader, bool subset = false );
    void read_feature_data2( FdoIFeatureReader *myReader );

    char mSuffix[12];
};

#endif // CPP_UNIT_FDOSELECTTEST_H
