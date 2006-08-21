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

#ifndef CPP_UNIT_FDOMULTITHREADTEST_H
#define CPP_UNIT_FDOMULTITHREADTEST_H

struct FunctionInfo {
#ifdef _WIN32
    LPTHREAD_START_ROUTINE Function1;
    LPTHREAD_START_ROUTINE Function2;
#else
     void* (*Function1)(void *);
     void* (*Function2)(void *);
#endif
};

class FdoMultiThreadTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( FdoMultiThreadTest );
  CPPUNIT_TEST( QueryTest );
  CPPUNIT_TEST( InsertTest );
  //CPPUNIT_TEST( UpdateTest );  // Cannot work as one thread modify the data and the second fails as the data is no longer as expected.
  CPPUNIT_TEST( CombinationTest );
  CPPUNIT_TEST_SUITE_END();

public:
    FdoMultiThreadTest(void);
    virtual ~FdoMultiThreadTest(void);
    void setUp ();

protected:
    void StartTest (FunctionInfo *funcInfo );
	virtual FdoIConnection * GetConnection();
    virtual void QueryTest();
    virtual void InsertTest();
    virtual void UpdateTest();
    virtual void CombinationTest();
    virtual void  set_provider() {};

protected:
	static void ReadAcDbPolyline( int threadId, FdoIFeatureReader *myReader );
#ifdef _WIN32
	static DWORD WINAPI StartQuery(LPVOID lpParameter);
	static DWORD WINAPI StartInsert(LPVOID lpParameter);
	static DWORD WINAPI StartUpdate(LPVOID lpParameter);
#else
	static void* StartQuery( void *lpParameter);
	static void* StartInsert( void *lpParameter);
	static void* StartUpdate( void *lpParameter);
#endif

};

#endif // CPP_UNIT_FDOMULTITHREADTEST_H
