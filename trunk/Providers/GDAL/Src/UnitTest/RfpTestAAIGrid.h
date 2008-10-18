/*
 * (C) Copyright 2004 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 *
 * Revision Control Modification History
 *
 *         $Id: //Providers/RasterFile/src/UnitTest/RfpTestAAIGird.h#1 $
 *     $Author: miaoya $
 *   $DateTime: 2005/03/17 19:58:08 $
 *     $Change: 6738 $
 *
 */

#ifndef RfpTestAAIGird_H
#define RfpTestAAIGird_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

class RfpTestAAIGird : public RfpTestCase
{
	// TODO: Enable test function once SAMPLE data is validated and redistributable
	// FDO_CPPUNIT_DEFINE(testDataStream);
	// FDO_CPPUNIT_DEFINE(testGetNullValue);

    // TODO: Enable test function once MOSIAC is supported
	// FDO_CPPUNIT_DEFINE(testClippedMosaic);
	
	CPPUNIT_TEST_SUITE(RfpTestAAIGird);	

	// TODO: Enable test function once SAMPLE data is validated and redistributable
	// CPPUNIT_TEST(testDataStream);
	// CPPUNIT_TEST(testGetNullValue);

    // TODO: Enable test function once MOSIAC is supported
	// CPPUNIT_TEST(testClippedMosaic);

	CPPUNIT_TEST_SUITE_END();

public:
	RfpTestAAIGird(void);
	~RfpTestAAIGird(void);

	virtual void _setUp();
	virtual void _tearDown();

	void testDataStream();
	void testGetNullValue();
	void testClippedMosaic();	
};

#endif
