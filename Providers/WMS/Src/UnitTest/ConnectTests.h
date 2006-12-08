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

#ifndef WMS_CPP_UNIT_CONNECTTESTS_H
#define WMS_CPP_UNIT_CONNECTTESTS_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "WmsTests.h"

class ConnectTests :  public WmsTests
{
    CPPUNIT_TEST_SUITE (ConnectTests);
    CPPUNIT_TEST (TestConnection1);
    CPPUNIT_TEST (TestConnection2);
    CPPUNIT_TEST (TestConnection3);
    CPPUNIT_TEST (TestConnection4);
    CPPUNIT_TEST (TestConnection5);
    CPPUNIT_TEST (TestConnection6);
    CPPUNIT_TEST (TestConnection7);
    CPPUNIT_TEST (TestConnection8);
    CPPUNIT_TEST (TestConnection9);
    CPPUNIT_TEST (TestConnection10);
    CPPUNIT_TEST (TestConnection11);
    CPPUNIT_TEST (TestConnection12);
    CPPUNIT_TEST (TestConnection13);
    // disabled because the server respond with page not found
    //CPPUNIT_TEST (TestConnection14);

    // Currently the provider does not support "transfer" timeout. And connecting
    // to this server always cause timeout.
    // CPPUNIT_TEST (TestConnectionTimeout);
    CPPUNIT_TEST_SUITE_END ();

public:
    ConnectTests (void);
    virtual ~ConnectTests (void);

protected:
    void TestConnection(FdoString* connString, bool expectedExc = false);
	
    void TestConnection1 ();
	void TestConnection2 ();
	void TestConnection3 ();
	void TestConnection4 ();
	void TestConnection5 ();
	void TestConnection6 ();
	void TestConnection7 ();
	void TestConnection8 ();
	void TestConnection9 ();
	void TestConnection10 ();
	void TestConnection11 ();
	void TestConnection12 ();
	void TestConnection13 ();
	void TestConnection14 ();
    void TestConnectionTimeout ();
};

#endif // WMS_CPP_UNIT_CONNECTTESTS_H
