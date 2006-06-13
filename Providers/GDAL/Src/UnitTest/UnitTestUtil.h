#ifndef CPP_FDOGRFP_UNIT_TESTUTIL_H
#define CPP_FDOGRFP_UNIT_TESTUTIL_H
/*
 * (C) Copyright 2003 by Autodesk, Inc. All Rights Reserved.
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
 *         $Id: //providers/Shp/src/UnitTest/UnitTestUtil.h#2 $
 *     $Author: derrick $
 *   $DateTime: 2005/05/18 09:40:09 $
 *     $Change: 7019 $
 *
 */
#ifdef _WIN32
#pragma once
#endif

// Various utility functions to help with unit testing.
class UnitTestUtil
{
public:
	UnitTestUtil(void);
   ~UnitTestUtil(void);

public:
	// Compare an output file against its master (expected) copy.
	// Fail if they are different.
	static void CheckOutput( const char* masterFileName, const char* outFileName );

	// Compare two files.
	// Returns:
	//		0 - they are the same
	//	   -1 - they are different.
	static FdoInt32 CompareFiles( const char* file1Name, const char* file2Name );
};

#endif // CPP_FDOGRFP_UNIT_TESTUTIL_H
