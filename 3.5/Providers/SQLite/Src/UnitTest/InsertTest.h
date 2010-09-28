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

#ifndef INSERTTEST_H
#define INSERTTEST_H

#ifdef _WIN32
#pragma once
#endif

#include "Fdo.h"
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <TestCommonInsert.h>

class InsertTest : public TestCommonInsert
{
	CPPUNIT_TEST_SUB_SUITE( InsertTest, TestCommonInsert );
    CPPUNIT_TEST( TestInsInvalidGeomTypes );
    CPPUNIT_TEST( TestConstraints1 );
    CPPUNIT_TEST( TestConstraints2 );
    CPPUNIT_TEST( TestCompPK );
    CPPUNIT_TEST( TestNoPK );
	CPPUNIT_TEST_SUITE_END();

public:
    InsertTest(void);
    virtual ~InsertTest(void);
    void setUp ();
	void tearDown ();

    void TestInsInvalidGeomTypes ();

    void TestConstraints1 ();
    void TestConstraints2 ();
    void TestCompPK ();
    void TestNoPK ();
protected:
    virtual FdoString* GetDefaultSchemaName(void);

    virtual void CreateConnection( Context& context, FdoBoolean recreateDb = false );

    virtual bool InsertsDefaultValues();

    virtual FdoDataType GetAutoIncrementType();
private:
    bool TestForDateValue(FdoIConnection* conn, FdoString* clsName, FdoString* propName, FdoInt32 id, FdoDateTime* expVal = NULL);
    void TestInsertData(FdoIConnection* conn, FdoString* className, FdoInt32 expectedProps = -1, FdoIdentifierCollection* propNames = NULL);
};

#endif	//INSERTTEST_H
