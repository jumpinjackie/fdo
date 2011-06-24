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
#ifndef CPP_UNIT_FDOTRANSACTION_H
#define CPP_UNIT_FDOTRANSACTION_H
/*
 * A test case for transaction.
 */
class FdoTransactionTest : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE( FdoTransactionTest );
    CPPUNIT_TEST( TestBeginTransaction );
    CPPUNIT_TEST( TestCommit );
    CPPUNIT_TEST( TestRollback );
    CPPUNIT_TEST( TestAddSavePoint );
    CPPUNIT_TEST( TestRollbackSavePoint );
    CPPUNIT_TEST( TestReleaseSavePoint );
    CPPUNIT_TEST_SUITE_END();

 public:
    FdoTransactionTest(void);
    ~FdoTransactionTest(void);
    void setUp ();
	void tearDown ();
    
    virtual void TestBeginTransaction();
    virtual void TestCommit();
    virtual void TestRollback();
    virtual void TestAddSavePoint();
    virtual void TestRollbackSavePoint();
    virtual void TestReleaseSavePoint();
    virtual void connect();

 protected:
    virtual void CreateTestSchema();
    virtual void DropTestSchema();
    virtual void set_provider() {};
    FdoPtr<FdoIConnection> mConnection;
    FdoPtr<FdoFeatureSchema> testSchema;
};
#endif

