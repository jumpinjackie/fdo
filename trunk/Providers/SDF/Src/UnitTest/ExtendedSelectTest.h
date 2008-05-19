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

#ifndef _ExtendedSelectTest_H_
#define _ExtendedSelectTest_H_


#ifdef _WIN32
#pragma once
#endif



#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Fdo.h"

class ExtendedSelectTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( ExtendedSelectTest );
  CPPUNIT_TEST( FileBasedTest );
  CPPUNIT_TEST( MemoryBasedTest );
  CPPUNIT_TEST( ExtendedPropTest );
  CPPUNIT_TEST( UpdateTest );
  CPPUNIT_TEST( WorldTest_SDF );
  CPPUNIT_TEST( WorldTest_SDF_name1 );
  CPPUNIT_TEST( WorldTest_SDF_name2 );
  CPPUNIT_TEST( WorldTest_SDF_name3 );
  CPPUNIT_TEST( WorldTest_SDF_name4 );
  CPPUNIT_TEST( CreateSDF_SPchars );
  CPPUNIT_TEST( Test_Fix823645 );
  CPPUNIT_TEST_SUITE_END();

public:
    ExtendedSelectTest(void);
    virtual ~ExtendedSelectTest(void);
    virtual void setUp ();

protected:
	void MainTest(bool fileSort, bool extendProps);
	void FileBasedTest() { MainTest(true,false); }
	void MemoryBasedTest() { MainTest(false,false); }
	void ExtendedPropTest() { MainTest(true,true); }
	void WorldTest(FdoIConnection* conn);
	void WorldTest_SDF();
	void WorldTest_SDF_name1();
	void WorldTest_SDF_name2();
	void WorldTest_SDF_name3();
	void WorldTest_SDF_name4();
	void CreateSDF_SPchars();
    void Test_Fix823645();
	void UpdateTest();
	void OpenConnection(FdoIConnection* conn, const wchar_t* path, bool placeDQ = false);
};


#endif
