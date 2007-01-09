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

#include <TestCommonConstraints.h>

#define DBG(x)		//x
#define DBG_MAX(x)		//x

class FdoConstraintsTest : public TestCommonConstraints
{
    CPPUNIT_TEST_SUB_SUITE (FdoConstraintsTest, TestCommonConstraints);

	CPPUNIT_TEST( TestParser );

	CPPUNIT_TEST_SUITE_END();

public:
	void TestParser(void);

	bool TestParser(FdoString* clause);

	FdoConstraintsTest(void);
    virtual ~FdoConstraintsTest(void);

    void setUp();

protected:
    virtual FdoIConnection* CreateConnection( FdoBoolean recreateDb = false );
    virtual FdoBoolean CanRestrictCheckConstraint();
    virtual void  set_provider() {};

};

#endif	//CPP_UNIT_FDOCONSTRAINTSTEST_H
