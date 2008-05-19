//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

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

