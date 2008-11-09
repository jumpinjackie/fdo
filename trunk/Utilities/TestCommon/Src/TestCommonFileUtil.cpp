/*
 * 
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
* 
 * Revision Control Modification History
 *
 *         $Id:  $
 *     $Author:  $
 *   $DateTime:  $
 *     $Change:  $
 *
 */

#include "stdafx.h"
#include "TestCommonFileUtil.h"
#include "FdoCommonFile.h"


void TestCommonFileUtil::CompareFilesAndThrow( const char* masterFileName, const char* outFileName )
{
	if ( CompareFiles( masterFileName, outFileName ) != 0 ) {
		char buffer[5000];
		sprintf( buffer, "Output file %s differs from expected output file %s", outFileName, masterFileName );
        CPPUNIT_FAIL (buffer);
	}
}

FdoInt32 TestCommonFileUtil::CompareFiles( const char* file1Name, const char* file2Name )
{
	char buffer[500];
	char buffer1[5000];
	char buffer2[5000];

	FdoInt32 retcode = -1;

	FILE* fp1 = fopen( file1Name, "r" );
	FILE* fp2 = fopen( file2Name, "r" );
	
	if ( fp1 == NULL ) {
		sprintf( buffer, "TestCommonFileUtil::CompareFiles: failed to open file %s", file1Name );
        CPPUNIT_FAIL (buffer);
	}

	if ( fp2 == NULL ) {
    	fclose(fp1);
		sprintf( buffer, "TestCommonFileUtil::CompareFiles: failed to open file %s", file2Name );
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


void TestCommonFileUtil::CompareFiles (const wchar_t* file1Name, const wchar_t* file2Name, unsigned long startByteOffset, unsigned long bytesToCompare)
{
    FdoCommonFile file1;
    FdoCommonFile file2;
    FdoCommonFile::ErrorCode code;
    unsigned char buffer1;
    unsigned char buffer2;
    unsigned long count;

    if (file1.OpenFile (file1Name, FdoCommonFile::IDF_OPEN_READ, code))
        if (file2.OpenFile (file2Name, FdoCommonFile::IDF_OPEN_READ, code))
        {
            file1.SetFilePointer (startByteOffset);
            file2.SetFilePointer (startByteOffset);
            if (-1L == bytesToCompare)
            {
                file2.GetFileSize (bytesToCompare);
                file1.GetFileSize (count);
                if (count > bytesToCompare)
                    bytesToCompare = count;
            }
            count = 0;
            while (file1.ReadFile (&buffer1, 1L))
                if (file2.ReadFile (&buffer2, 1L))
                {
					// The 29th byte is LDID. The original file might not have it set but (i.e. is 0) but the 
					// copy file does (takes the locale into account).
					bool isLDID = ((count + startByteOffset) == 29);
                    if ((buffer1 != buffer2) && (buffer1 != 0 && isLDID))
                    {
                        char message[1024];
                        sprintf (message, "compare error at offset 0x%x, expected 0x%x, got 0x%x", count + startByteOffset, buffer2, buffer1);
                        CPPUNIT_FAIL (message);
                    }
                    count++;
                    if (count > bytesToCompare)
                        return;
                }
                else
                    CPPUNIT_FAIL ("reference has fewer bytes");
            // check the file2 file has been sucked dry too
            CPPUNIT_ASSERT_MESSAGE ("reference has more bytes", !file2.ReadFile (&buffer2, 1L));

        }
        else
            CPPUNIT_FAIL ("can't open reference file");
    else
        CPPUNIT_FAIL ("can't open target file");
}

