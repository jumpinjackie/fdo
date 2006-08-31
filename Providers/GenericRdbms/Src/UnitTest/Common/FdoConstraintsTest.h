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

#ifndef CPP_UNIT_FDOCONSTRAINTSTEST_H
#define CPP_UNIT_FDOCONSTRAINTSTEST_H

class FdoConstraintsTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( FdoConstraintsTest );

	CPPUNIT_TEST( TestParser );
	CPPUNIT_TEST( TestCreateConstraints );
	CPPUNIT_TEST( TestDescribeConstraints );
	CPPUNIT_TEST( TestUpdateUniqueConstraints );
	CPPUNIT_TEST( TestUpdateCheckConstraints );

	CPPUNIT_TEST_SUITE_END();

public:
    void TestCreateConstraints(void);
	void TestDescribeConstraints(void);
	void TestUpdateUniqueConstraints(void);
	void TestUpdateCheckConstraints(void);
	void TestParser(void);

    void CreateConstraintsSchema(FdoIConnection * connection);
	void DescribeConstraintsSchema(FdoIConnection * connection, FdoString *className, int numUkeys, int numCkeys);
	void UpdateCheckConstraints(FdoIConnection * connection);
	void UpdateUniqueConstraints(FdoIConnection * connection);
	bool TestParser(FdoString* clause);

	FdoConstraintsTest(void);
    virtual ~FdoConstraintsTest(void);
    void setUp ();

protected:
    virtual void  set_provider() {};   

	void TestConstraints();

};

#endif	//CPP_UNIT_FDOCONSTRAINTSTEST_H
