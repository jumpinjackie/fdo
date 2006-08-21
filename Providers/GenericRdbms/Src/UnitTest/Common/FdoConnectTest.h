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

#ifndef CPP_UNIT_MULTIPLECONNECTTEST_H
#define CPP_UNIT_MULTIPLECONNECTTEST_H

/*
 * A test case for multiple connection objects
 */
class FdoConnectTest : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE( FdoConnectTest );
	CPPUNIT_TEST ( connectPropDictionaryTest );
	CPPUNIT_TEST ( connectWithInvalidString );
    CPPUNIT_TEST( query );
    CPPUNIT_TEST_SUITE_END();

public:
    FdoConnectTest(void);
    virtual ~FdoConnectTest(void);
    void setUp ();
    void tearDown ();

protected:
    virtual void  set_provider() {};   
    virtual void connect ();
    virtual void query ();
	virtual void connectPropDictionaryTest(){};
	virtual void connectWithInvalidString();

    FdoIConnection* mConnection;
    void ReadAcDbPolyline( int connNumber, FdoIFeatureReader *myReader );

protected:
	static bool HasGeometry(FdoIConnection * connection,FdoString * className);
};

#endif // CPP_UNIT_MULTIPLECONNECTTEST_H
