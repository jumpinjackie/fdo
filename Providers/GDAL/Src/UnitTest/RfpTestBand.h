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

#ifndef RFPTESTBAND_H
#define RFPTESTBAND_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

class RfpTestBand : public RfpTestCase
{
	FDO_CPPUNIT_DEFINE(testSelect);

    // TODO: Enable when MOSAIC and CLIP supported
	//FDO_CPPUNIT_DEFINE(testClippedMosaic);
	//FDO_CPPUNIT_DEFINE(testMosaic4by4);

	CPPUNIT_TEST_SUITE(RfpTestBand);
	CPPUNIT_TEST(testSelect);

    // TODO: Enable when MOSAIC and CLIP supported
    //CPPUNIT_TEST(testClippedMosaic);
	//CPPUNIT_TEST(testMosaic4by4);

    CPPUNIT_TEST_SUITE_END();

public:
	RfpTestBand(void);
	~RfpTestBand(void);

	virtual void _setUp();
	virtual void _tearDown();

	void testSelect();
	void testClippedMosaic();
	void testMosaic4by4();

};

#endif
