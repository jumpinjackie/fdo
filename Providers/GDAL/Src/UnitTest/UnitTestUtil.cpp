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
 *         $Id: //providers/Shp/src/UnitTest/UnitTestUtil.cpp#3 $
 *     $Author: derrick $
 *   $DateTime: 2005/05/18 09:40:09 $
 *     $Change: 7019 $
 *
 */

#include "UnitTest.h"
#include "UnitTestUtil.h"
#ifdef _WIN32
#include <io.h>
#include <stddef.h>
#endif

UnitTestUtil::UnitTestUtil(void)
{
}

UnitTestUtil::~UnitTestUtil(void)
{
}

void UnitTestUtil::CheckOutput( const char* masterFileName, const char* outFileName )
{
	if ( CompareFiles( masterFileName, outFileName ) != 0 ) {
		char buffer[5000];
		sprintf( buffer, "Output file %s differs from expected output file %s", outFileName, masterFileName );
        CPPUNIT_FAIL (buffer);
	}
}

FdoInt32 UnitTestUtil::CompareFiles( const char* file1Name, const char* file2Name )
{
	char buffer[500];
	char buffer1[5000];
	char buffer2[5000];

	FdoInt32 retcode = -1;

	FILE* fp1 = fopen( file1Name, "r" );
	FILE* fp2 = fopen( file2Name, "r" );
	
	if ( fp1 == NULL ) {
		sprintf( buffer, "UnitTestUtil::CompareFiles: failed to open file %s", file1Name );
        CPPUNIT_FAIL (buffer);
	}

	if ( fp2 == NULL ) {
    	fclose(fp1);
		sprintf( buffer, "UnitTestUtil::CompareFiles: failed to open file %s", file2Name );
        CPPUNIT_FAIL (buffer);
	}

	while ( fgets( buffer1, sizeof(buffer1-1), fp1 ) != NULL ) {
		if ( !fgets( buffer2, sizeof(buffer2-1), fp2  ) ) 
			// different: file2 has fewer lines.
			goto the_exit;

		if ( strcmp( buffer1, buffer2 ) )
			// different: a line is different
			goto the_exit;
	}

	if ( fgets( buffer2, sizeof(buffer2-1), fp2 ) ) 
		// different: file2 has more lines.
		goto the_exit;

	retcode = 0;

the_exit:
	fclose(fp1);
	fclose(fp2);

	return( retcode );
}

