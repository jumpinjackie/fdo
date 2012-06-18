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

#ifndef RFPTESTGEOTIFF_H
#define RFPTESTGEOTIFF_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

class RfpTestGeotiff : public RfpTestCase
{
	FDO_CPPUNIT_DEFINE(testGeotiff);			

	CPPUNIT_TEST_SUITE(RfpTestGeotiff);	
	CPPUNIT_TEST(testGeotiff);
	CPPUNIT_TEST_SUITE_END();

public:
	RfpTestGeotiff(void);
	~RfpTestGeotiff(void);

	virtual void _setUp();
	virtual void _tearDown();

	void testGeotiff();
	
};

#endif
