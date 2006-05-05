#ifndef _FdoMultiThreadTest_H_
#define _FdoMultiThreadTest_H_


#ifdef _WIN32
#pragma once
#endif

#ifndef CPP_UNIT_FdoMultiThreadTest_H
#define CPP_UNIT_FdoMultiThreadTest_H
#ifdef _WIN32
#include <crtdbg.h>
#include <Windows.h>
#endif // _WIN32

#ifdef _WIN32
#define SDF_FILE  L"..\\..\\TestData\\PARCEL.SDX"
#define SDF_FILE_S  "..\\..\\TestData\\PARCEL.SDX"
#else
#define SDF_FILE  L"../../data/PARCEL_linuxtest.SDX"
#define SDF_FILE_S  "../../data/PARCEL_linuxtest.SDX"
#endif

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Fdo.h"

struct ConnectInfo {
    int   connectionId;
    FdoIConnection  *mConn;
};

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
#ifdef _WIN32
  CPPUNIT_TEST( InsertTest );
  CPPUNIT_TEST( CombinationTest );
#endif
  CPPUNIT_TEST_SUITE_END();

public:
    FdoMultiThreadTest(void);
    virtual ~FdoMultiThreadTest(void);
    virtual void setUp ();

protected:
	void StartTest (FunctionInfo *funcInfo );
	void OpenConnection(FdoIConnection* conn, const wchar_t* path );
    void QueryTest();
    void InsertTest();
    void CombinationTest();
#ifdef _WIN32
	virtual void InitInsertFunction( LPTHREAD_START_ROUTINE & Funct );
#else
	virtual void InitInsertFunction( void* (*Funct)(void *) );
#endif
};


#endif
#endif
