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

#ifndef RFPTESTRASTERCONVERSION_H
#define RFPTESTRASTERCONVERSION_H

#ifdef _WIN32
#pragma once
#endif //_WIN32


class RfpTestRasterConversion : public RfpTestCase
{
	//FDO_CPPUNIT_DEFINE(testScaling);			
	FDO_CPPUNIT_DEFINE(testRgbToBitonal);			
	FDO_CPPUNIT_DEFINE(testRgbToGray);			
	//FDO_CPPUNIT_DEFINE(testScalingAndToGray);			
	FDO_CPPUNIT_DEFINE(testRgbToPalette);			

	CPPUNIT_TEST_SUITE(RfpTestRasterConversion);	
	//CPPUNIT_TEST(testScaling);
	CPPUNIT_TEST(testRgbToBitonal);
	CPPUNIT_TEST(testRgbToGray);
	//CPPUNIT_TEST(testScalingAndToGray);
	CPPUNIT_TEST(testRgbToPalette);
	CPPUNIT_TEST_SUITE_END();

public:
	RfpTestRasterConversion(void);
	~RfpTestRasterConversion(void);

	virtual void _setUp();
	virtual void _tearDown();

	void testScaling();
	void testRgbToBitonal();
	void testRgbToGray();
	void testScalingAndToGray();
	void testRgbToPalette();

private:
	FdoPtr<FdoIConnection> m_connection;
	FdoPtr<FdoIRaster> m_raster;

};

#endif
