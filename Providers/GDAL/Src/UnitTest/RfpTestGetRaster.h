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

#ifndef RFPTESTGETRASTER_H
#define RFPTESTGETRASTER_H

#ifdef _WIN32
#pragma once
#endif

class RfpTestGetRaster : public RfpTestCase
{
	FDO_CPPUNIT_DEFINE(testGetClippedAndScaledRaster);
	FDO_CPPUNIT_DEFINE(testGetRaster);		// 4 * 4	
	FDO_CPPUNIT_DEFINE(testGetRasterTileByTile);		// 4 * 4	
	FDO_CPPUNIT_DEFINE(testGetRasterAllInOnce);
	FDO_CPPUNIT_DEFINE(testGetRasterRGB24);
	FDO_CPPUNIT_DEFINE(testGetRotatedRaster);

	CPPUNIT_TEST_SUITE(RfpTestGetRaster);	
	CPPUNIT_TEST(testGetClippedAndScaledRaster);
	CPPUNIT_TEST(testGetRaster);
	CPPUNIT_TEST(testGetRasterRGB24);
	CPPUNIT_TEST(testGetRasterTileByTile);
	CPPUNIT_TEST(testGetRasterAllInOnce);
//	CPPUNIT_TEST(testGetRotatedRaster);
	CPPUNIT_TEST_SUITE_END();

public:
	RfpTestGetRaster(void);
	~RfpTestGetRaster(void);

	virtual void _setUp();
	virtual void _tearDown();
	
	// buffer size = 8192 Bytes.
	void testGetRaster();
	void testGetRasterTileByTile();

	// Get raster data to buffer all in once.	
	// size = 800*600*4 
	void testGetRasterAllInOnce();

	void testGetRasterRGB24();

	void testGetClippedAndScaledRaster();

    void testGetRotatedRaster();
private:
	FdoPtr<FdoIConnection> m_connection;
};

#endif //RFPTESTGETRASTER_H
