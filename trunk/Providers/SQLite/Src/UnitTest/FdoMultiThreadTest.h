// Copyright (C) 2004-2007  Autodesk, Inc.
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

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Fdo.h"
#include <TestCommonMiscUtil.h>

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
  CPPUNIT_TEST( InsertQueryTest );
  CPPUNIT_TEST_SUITE_END();

public:
    FdoMultiThreadTest(void);
    virtual ~FdoMultiThreadTest(void);
    virtual void setUp ();

protected:
	void StartTest (FunctionInfo *funcInfo );
	void OpenConnection(FdoIConnection* conn, const wchar_t* path );
    void QueryTest();
    void InsertQueryTest();
#ifdef _WIN32
	virtual void InitInsertFunction( LPTHREAD_START_ROUTINE & Funct );
#else
	virtual void InitInsertFunction( void* (*Funct)(void *) );
#endif
};


#endif
#endif
