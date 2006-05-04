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
 */

#include "Pch.h"
#include "UnitTestUtil.h"
#ifdef _WIN32
#include <io.h>
#include <stddef.h>
#else
#include <unistd.h>
#endif

UnitTestUtil::UnitTestUtil(void)
{
}

UnitTestUtil::~UnitTestUtil(void)
{
}


void UnitTestUtil::FailOnException( FdoException* e )
{

	PrintException( e );

	FdoStringP exceptions = Exception2String( e );

    e->Release ();

	CPPUNIT_FAIL( (const char*) exceptions );
}


FdoStringP UnitTestUtil::Exception2String( FdoException* e )
{
	FdoPtr<FdoException> innerE = e;
	// Add ref to prevent smart pointer from destroying exception.
	FDO_SAFE_ADDREF(e);					
    FdoStringP retString;

	while ( innerE->GetCause() )
		innerE = innerE->GetCause();

	if ( innerE == e ) 
        retString += FdoStringP::Format( L"%ls", e->GetExceptionMessage() );
	else
		retString += FdoStringP::Format( L"%ls ... %ls", innerE->GetExceptionMessage(), e->GetExceptionMessage() );

    return retString;
}

void UnitTestUtil::PrintException( FdoException* e, FdoIoStream* stream, FdoBoolean stripLineNo )
{
    FdoIoTextWriterP writer = 
        stream ?
            FdoIoTextWriter::Create(stream) :
            FdoIoTextWriter::Create( FdoIoFileStreamP(FdoIoFileStream::Create(stdout)) );
	
            FdoPtr<FdoException> currE = e;
	// Add ref to prevent smart pointer from destroying exception.
	FDO_SAFE_ADDREF(e);

	while ( currE ) {
        
        FdoString* pMessage = NULL;
        if ( stripLineNo ) // The following removes the function and line number info from error messages.
		{
			pMessage = wcschr( currE->GetExceptionMessage(), ')' );
			if (NULL != pMessage)
				pMessage += 2;
		}
		if (NULL == pMessage)
            pMessage = currE->GetExceptionMessage();

        writer->WriteLine( pMessage );
		currE = currE->GetCause();
	}
}

void UnitTestUtil::PrintException( FdoException* e, const char* fileName, FdoBoolean stripLineNo )
{
    FdoStringP wFileName( fileName );
    FdoIoFileStreamP stream = FdoIoFileStream::Create( wFileName, L"w+t" );

	PrintException( e, stream, stripLineNo );
}

void UnitTestUtil::CheckOutput( const char* masterFileName, const char* outFileName )
{
	if ( CompareFiles( masterFileName, outFileName ) != 0 ) {
		char buffer[5000];
		sprintf( buffer, "Output file %s differs from expected output file %s", outFileName, masterFileName );
		CPPUNIT_FAIL( buffer );
	}
}

int UnitTestUtil::CompareFiles( const char* file1Name, const char* file2Name )
{
	char buffer[500];
	char buffer1[5000];
	char buffer2[5000];

	int retcode = -1;

	FILE* fp1 = fopen( file1Name, "r" );
	FILE* fp2 = fopen( file2Name, "r" );
	
	if ( fp1 == NULL ) {
		sprintf( buffer, "UnitTestUtil::CompareFiles: failed to open file %s", file1Name );
		CPPUNIT_FAIL( buffer );
	}

	if ( fp2 == NULL ) {
		sprintf( buffer, "UnitTestUtil::CompareFiles: failed to open file %s", file2Name );
		CPPUNIT_FAIL( buffer );
	}

	while ( fgets( buffer1, 4999, fp1 ) != NULL ) {
		if ( !fgets( buffer2, 4999, fp2  ) ) 
			// different: file2 has fewer lines.
			goto the_exit;

		if ( strcmp( buffer1, buffer2 ) )
			// different: a line is different
			goto the_exit;
	}

	if ( fgets( buffer2, 4999, fp2 ) ) 
		// different: file2 has more lines.
		goto the_exit;

	retcode = 0;

the_exit:
	fclose(fp1);
	fclose(fp2);

	return( retcode );
}

bool UnitTestUtil::IsRedirected( FILE* fp )
{
    // Cloned from Oracle Provider SysAdminArx
#ifdef _WIN32

    HANDLE h;
    bool ret;

    ret = false;

    if ( fp == stdin ) 
        h = GetStdHandle( STD_INPUT_HANDLE );
    else if ( fp == stdout ) 
        h = GetStdHandle( STD_OUTPUT_HANDLE );
    else if ( fp == stderr ) 
        h = GetStdHandle( STD_ERROR_HANDLE );
    else
        return(false);

    if (INVALID_HANDLE_VALUE != h )
        ret = (FILE_TYPE_CHAR != GetFileType (h));

    return (ret);
#else
    return !isatty(fileno(fp));
#endif
}


