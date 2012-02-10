/*
 * Copyright (C) 2004-2007  Autodesk, Inc.
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

#ifndef FDOWmsTestCustomCommands_H
#define FDOWmsTestCustomCommands_H

#ifdef WIN32
#pragma once
#endif

#include "WmsTests.h"


class WmsTestCustomCommands : public WmsTests
{
	CPPUNIT_TEST_SUITE (WmsTestCustomCommands);

#ifdef _FDO_RUN_INTERNAL_TEST	

#endif

    CPPUNIT_TEST (testGetImageFormats);
    CPPUNIT_TEST (testGetFeatureStyles);
    CPPUNIT_TEST (testGetFeatureCRSNames);
    CPPUNIT_TEST (testSpacialFeatureClassName);
	CPPUNIT_TEST (testInvalidParamForStyle);
	CPPUNIT_TEST (testInvalidParamForCRS);

	CPPUNIT_TEST_SUITE_END ();

private:
    void ExecuteSelectServer(FdoString* server, FdoString* classname);

public:
	WmsTestCustomCommands(void);
	~WmsTestCustomCommands(void);

	void _setUp () {}
	void _tearDown () {}
	void testGetImageFormats ();
	void testGetFeatureStyles ();
    void testGetFeatureCRSNames ();
    void testSpacialFeatureClassName();
	void testInvalidParamForStyle ();
	void testInvalidParamForCRS ();
};

#endif//FDOWmsTestCustomCommands_H
