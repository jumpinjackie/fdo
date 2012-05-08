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

#ifndef RfpTestSpatialContext_H
#define RfpTestSpatialContext_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

class RfpTestSpatialContext : public RfpTestCase
{
	FDO_CPPUNIT_DEFINE(testSpatialContext1);
	FDO_CPPUNIT_DEFINE(testSpatialContext2);
	FDO_CPPUNIT_DEFINE(testSpatialContext3);
	FDO_CPPUNIT_DEFINE(testSpatialContext4);
	FDO_CPPUNIT_DEFINE(testSpatialContext5);
	FDO_CPPUNIT_DEFINE(testSpatialContext6);

	CPPUNIT_TEST_SUITE(RfpTestSpatialContext);
	CPPUNIT_TEST(testSpatialContext1);
	CPPUNIT_TEST(testSpatialContext2);
	CPPUNIT_TEST(testSpatialContext3);
	CPPUNIT_TEST(testSpatialContext4);
	CPPUNIT_TEST(testSpatialContext5);
	CPPUNIT_TEST(testSpatialContext6);
	CPPUNIT_TEST_SUITE_END();

public:
	RfpTestSpatialContext(void);
	~RfpTestSpatialContext(void);

	virtual void _setUp();
	virtual void _tearDown();

	void testSpatialContext1();
	void testSpatialContext2();
	void testSpatialContext3();
	void testSpatialContext4();

	void testSpatialContext5();
	void testSpatialContext6();

	static FdoRfpRect CreateRectFromGeometryAgf(const FdoPtr<FdoByteArray>& geoValue)
	{
		FdoPtr<FdoFgfGeometryFactory> agfFactory = FdoFgfGeometryFactory::GetInstance();
		FdoPtr<FdoIGeometry> geometry = agfFactory->CreateGeometryFromFgf(geoValue);
		FdoPtr<FdoIEnvelope> envelope = geometry->GetEnvelope();
		return 	FdoRfpRect(envelope->GetMinX(), envelope->GetMinY(), envelope->GetMaxX(), envelope->GetMaxY());
	}
};

#endif
