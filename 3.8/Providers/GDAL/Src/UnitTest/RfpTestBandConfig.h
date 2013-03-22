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
 */

#ifndef RFPTESTBANDCONFIG_H
#define RFPTESTBANDCONFIG_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

class RfpTestBandConfig : public RfpTestCase
{
	FDO_CPPUNIT_DEFINE(testOverrides);
	FDO_CPPUNIT_DEFINE(testGeoReference);
	FDO_CPPUNIT_DEFINE(testNoGeoReference);
	FDO_CPPUNIT_DEFINE(testMixDataModel1);
	FDO_CPPUNIT_DEFINE(testMixDataModel2);
	FDO_CPPUNIT_DEFINE(testGetNullValue1);
	FDO_CPPUNIT_DEFINE(testGetNullValue2);
	FDO_CPPUNIT_DEFINE(testResolution);
	FDO_CPPUNIT_DEFINE(testMissingXmlTag);

	CPPUNIT_TEST_SUITE(RfpTestBandConfig);
	CPPUNIT_TEST(testOverrides);
//	CPPUNIT_TEST(testGeoReference);
	CPPUNIT_TEST(testNoGeoReference);
	CPPUNIT_TEST(testMixDataModel1);
	CPPUNIT_TEST(testMixDataModel2);
	CPPUNIT_TEST(testGetNullValue1);
	CPPUNIT_TEST(testGetNullValue2);
	CPPUNIT_TEST(testResolution);
	CPPUNIT_TEST(testMissingXmlTag);
	CPPUNIT_TEST_SUITE_END();

public:
	RfpTestBandConfig(void);
	~RfpTestBandConfig(void);

	virtual void _setUp();
	virtual void _tearDown();

	void testOverrides();
	void testGeoReference();
	void testNoGeoReference();
	void testMixDataModel1();
	void testMixDataModel2();
	void testGetNullValue1();
	void testGetNullValue2();
	void testResolution();
	void testMissingXmlTag();
};

#endif
