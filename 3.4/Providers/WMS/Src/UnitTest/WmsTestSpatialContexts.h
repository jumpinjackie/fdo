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

#ifndef WMSTESTSPATIALCONTEXTS_H
#define WMSTESTSPATIALCONTEXTS_H

#ifdef WIN32
#pragma once
#endif

#include "WmsTests.h"

class WmsTestSpatialContext : public WmsTests
{
	CPPUNIT_TEST_SUITE (WmsTestSpatialContext);
#ifdef _FDO_RUN_INTERNAL_TEST
	CPPUNIT_TEST (testServer1);
#endif
	CPPUNIT_TEST (testServer2);
	CPPUNIT_TEST_SUITE_END ();

public:
	WmsTestSpatialContext(void);
	~WmsTestSpatialContext(void);

	void _setUp () {}
	void _tearDown () {}
	void testServer1 ();
	void testServer2 ();

private:
	void testServer (FdoString* feaureServer, FdoInt32 cntSpatialContexts);
};


#endif//WMSTESTSPATIALCONTEXTS_H
