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

#ifndef FDOWMSTESTSELECT_H
#define FDOWMSTESTSELECT_H

#ifdef WIN32
#pragma once
#endif

#include "WmsTests.h"


class WmsTestSelect : public WmsTests
{
	CPPUNIT_TEST_SUITE (WmsTestSelect);

#ifdef _FDO_RUN_INTERNAL_TEST		
	CPPUNIT_TEST (testServer1);
	CPPUNIT_TEST (testNestedClip);
	CPPUNIT_TEST (testGetBounds);
	// disabled because the server cannot connect
	//CPPUNIT_TEST (testHttpBasicAuthentification);
#endif

    CPPUNIT_TEST (testURLEcoding);
    CPPUNIT_TEST (testServer2);
	CPPUNIT_TEST (testServer3);
	CPPUNIT_TEST (testServer4);
    CPPUNIT_TEST (testResample);
    CPPUNIT_TEST (testClip);
    CPPUNIT_TEST (testDefaultHeight);
    CPPUNIT_TEST (testNASAServer);      // Todo: might need to replace server
    CPPUNIT_TEST (testNASAServer2);     // Todo: might need to replace server
    // disabled because the server cannot connect
    //CPPUNIT_TEST (testIntegraphWorld);
    CPPUNIT_TEST (testCubeServer);
    CPPUNIT_TEST (testMultiLayers);     // Todo: might need to replace server
    CPPUNIT_TEST (testTerraService);
    //CPPUNIT_TEST (testDefect786029);
    // disabled because the server cannot connect
    //CPPUNIT_TEST (testEusoils);
    //CPPUNIT_TEST (testEusoilsPesera);
    //CPPUNIT_TEST (testEusoilsMeusis);
    CPPUNIT_TEST (testSelectSpatialExtents);
    CPPUNIT_TEST (testTerraServiceThumbnail);
    CPPUNIT_TEST (testNS_TOPO_1000);
    CPPUNIT_TEST (testNS_CRS);
    CPPUNIT_TEST (testMapConnect_SDE);
    CPPUNIT_TEST (testNS_TOPO_5000);
    CPPUNIT_TEST (testNASAServerDefaultOverrides);
    CPPUNIT_TEST (testCeoware2);
    CPPUNIT_TEST (testLioib);
    CPPUNIT_TEST (testKortPlandk);
    // disabled because the server cannot connect
    //CPPUNIT_TEST (testLibcwms);
    CPPUNIT_TEST (testStoboWms);     // Todo: might need to replace server
    CPPUNIT_TEST (testLinuxgurrl); 
    CPPUNIT_TEST (testOpenmaps);     // Todo: might need to replace server
    CPPUNIT_TEST (testquestionmarkend);     // Todo: might need to replace server

	CPPUNIT_TEST_SUITE_END ();

private:
    void ExecuteSelectServer(FdoString* server, FdoString* classname);

public:
	WmsTestSelect(void);
	~WmsTestSelect(void);

	void _setUp () {}
	void _tearDown () {}
	void testServer1 ();
	void testNestedClip ();
    void testGetBounds ();
	void testHttpBasicAuthentification ();
	void testServer2 ();
	void testResample ();
	void testClip ();
	void testDefaultHeight ();
	void testNASAServer ();
	void testNASAServer2 ();
	void testIntegraphWorld ();
    void testCubeServer ();
    void testMultiLayers ();
	void testTerraService ();
    void testDefect786029 ();
    void testEusoils ();
    void testEusoilsPesera();
    void testEusoilsMeusis();
    void testSelectSpatialExtents();
    void testTerraServiceThumbnail();
    void testNS_TOPO_1000();
    void testNS_CRS();
    void testMapConnect_SDE(); 
    void testNS_TOPO_5000();
    void testNASAServerDefaultOverrides();
    void testCeoware2();
    void testLioib ();
    void testKortPlandk ();
    void testLibcwms ();
    void testStoboWms ();
    void testLinuxgurrl ();
	void testOpenmaps ();
    void testquestionmarkend ();
    void testURLEcoding ();
	//test 1.3.0 version server with axis issue
	void testServer3();
	void testServer4();
};

#endif//FDOWMSTESTSELECT_H
