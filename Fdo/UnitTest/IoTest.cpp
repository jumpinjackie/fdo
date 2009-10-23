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

#include "Pch.h"
#include "IoTest.h"
#include "UnitTestUtil.h"
#include <Common/Io/ByteStreamReader.h>
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#include <sys/stat.h>

#ifdef _DEBUG
//#define DEBUG_DETAIL  1
#endif

#define LINE_COUNT 5000

CPPUNIT_TEST_SUITE_REGISTRATION (FdoIoTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (FdoIoTest, "FdoIoTest");

FdoIoTest::FdoIoTest(void)
{
}

FdoIoTest::~FdoIoTest(void)
{
}

void FdoIoTest::setUp()
{
    // Create a file used for various file tests.
    FILE* fp = fopen( "testFile.txt", "w" );
    FdoIoFileStreamP fileStream = FdoIoFileStream::Create( fp );

    fileStream->Write( (FdoByte*) "test file contents", 18 );
    fileStream = NULL;
    fclose(fp);
}

void FdoIoTest::testStreams()
{
    try {
        FdoIoMemoryStreamP memStream1 = FdoIoMemoryStream::Create();
        FdoIoMemoryStreamP memStream2 = FdoIoMemoryStream::Create();
        FdoIoMemoryStreamP memStream3 = FdoIoMemoryStream::Create();
        FdoIoFileStreamP fileStream1 = FdoIoFileStream::Create( L"testStream1.txt", L"w+" );
        FdoIoFileStreamP fileStream2 = FdoIoFileStream::Create( L"testStream2.txt", L"w+" );

        populateStream(memStream1);
//        vldStream(memStream1);
        bufferedWrite( memStream1, fileStream1 );
        vldStream(fileStream1);
        streamedWrite( fileStream1, memStream2 );
        vldStream(memStream2);

        streamedWrite( memStream2, fileStream2 );
        vldStream(fileStream2);
        bufferedWrite( fileStream2, memStream3 );
        vldStream(memStream3);

		fdoArrayRead(memStream3);

    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void FdoIoTest::testFileOpen()
{
    try {
        FdoIoFileStreamP fileStream;

        // Try to open non-existent file for read, should fail.

        FdoBoolean bFailed = false;
        try {
            fileStream = FdoIoFileStream::Create( L"testNotExists", L"r" );
        }
        catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue)
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
			FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Failed to open file 'testNotExists' with access modes: 'r'. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
			FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Failed to open file 'testNotExists' with access modes: 'r'. ") == 0 );
#endif           
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Opening non-existent file should have failed." );

        // Try null file name.

        bFailed = false;
        try {
            fileStream = FdoIoFileStream::Create( NULL, L"r" );
        }
        catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue)
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode .
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad fileName value '(NULL)' passed to FdoIoFileStream::Create. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad fileName value '(NULL)' passed to FdoIoFileStream::Create. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Opening file with null name should have failed." );

                
        // Try null access modes.

        bFailed = false;
        try {
            fileStream = FdoIoFileStream::Create( L"aFile", NULL );
        }
        catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue)
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode .
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad accessModes value '(NULL)' passed to FdoIoFileStream::Create. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad accessModes value '(NULL)' passed to FdoIoFileStream::Create. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Opening file with null modes should have failed." );

        // Try null fp.

        bFailed = false;
        try {
            fileStream = FdoIoFileStream::Create( NULL );
        }
        catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue)
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode .
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad fp value '(NULL)' passed to FdoIoFileStream::Create. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad fp value '(NULL)' passed to FdoIoFileStream::Create. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Opening file with null fp should have failed." );

        fileStream = FdoIoFileStream::Create( L"testFile.txt", L"w" );

    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void FdoIoTest::testFileRead()
{
    try {
        FdoIoFileStreamP fileStream;
        FdoByte buffer[1000];
        FdoSize count;

        // Open file in write-only mode
        fileStream = FdoIoFileStream::Create( L"testFileRead.txt", L"w" );

        // try reading into null buffer, should fail.

        FdoBoolean bFailed = false;
        try {
            count = fileStream->Read( NULL, 5 );
        }
        catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue)
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode .
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad buffer value '(NULL)' passed to FdoIoFileStream::Read. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad buffer value '(NULL)' passed to FdoIoFileStream::Read. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Reading into null buffer should have failed." );


        // try reading from write-only file, should fail

        bFailed = false;
        try {
            count = fileStream->Read( buffer, 5 );
        }
        catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue)
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode .
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Cannot read 5 bytes from unreadable stream. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Cannot read 5 bytes from unreadable stream. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Reading from write-only file should have failed." );


    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void FdoIoTest::testFileWrite()
{
    try {
        FdoIoFileStreamP fileStream;
        FdoIoMemoryStreamP memoryStream;
        FdoByte buffer[1000];

        memoryStream = FdoIoMemoryStream::Create();
        memoryStream->Write( (FdoByte*) "stuff", 5 );
        memoryStream->Reset();

        // Open file in read-only mode
        fileStream = FdoIoFileStream::Create( L"testFile.txt", L"r" );

        // try writing from null buffer, should fail.

        FdoBoolean bFailed = false;
        try {
            fileStream->Write( (FdoByte*) NULL, 5 );
        }
        catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue)
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode .
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad buffer value '(NULL)' passed to FdoIoFileStream::Write. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad buffer value '(NULL)' passed to FdoIoFileStream::Write. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing from null buffer should have failed." );


        // try writing to read-only file, should fail

        bFailed = false;
        try {
            fileStream->Write( buffer, 5 );
        }
        catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue)
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode .
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Cannot write 5 bytes to unwritable stream. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Cannot write 5 bytes to unwritable stream. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing to read-only file should have failed." );


        // retry writing tests, using a stream as input

        bFailed = false;
        try {
            fileStream->Write( (FdoIoStream*) NULL, 5 );
        }
        catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue)
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode .
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad stream value '(NULL)' passed to FdoIoFileStream::Write. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad stream value '(NULL)' passed to FdoIoFileStream::Write. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing from null stream should have failed." );

        bFailed = false;
        try {
            fileStream->Write( memoryStream );
        }
        catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue)
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode .
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Cannot write 5 bytes to unwritable stream. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Cannot write 5 bytes to unwritable stream. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing to read-only file should have failed." );

    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void FdoIoTest::testFileContext()
{
    try {
        // Create a stream with no contextual support
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create( stdin );

        if ( !fileStream->HasContext() ) {
            // try changing length, should fail.

            FdoBoolean bFailed = false;
            try {
                fileStream->SetLength(1000);
            }
            catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue)
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode .
#ifdef _DEBUG
                FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
                FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Stream is not contextual (has no absolute position); FdoIoFileStream::SetLength cannot be called ") == 0 );
#else
				FdoString* pMessage = e->GetExceptionMessage();
                FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Stream is not contextual (has no absolute position); FdoIoFileStream::SetLength cannot be called ") == 0 );
#endif
#endif
                bFailed = true;
                e->Release();
            }
            if ( !bFailed ) 
                CPPUNIT_FAIL( "SetLength on stdin should have failed." );


            // try getting length, should fail.

            bFailed = false;
            try {
                FdoInt64 length = fileStream->GetLength();
            }
            catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue)
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode .
#ifdef _DEBUG
                FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
                FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Stream is not contextual (has no absolute position); FdoIoFileStream::GetLength cannot be called ") == 0 );
#else
				FdoString* pMessage = e->GetExceptionMessage();
                FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Stream is not contextual (has no absolute position); FdoIoFileStream::GetLength cannot be called ") == 0 );
#endif
#endif
                bFailed = true;
                e->Release();
            }
            if ( !bFailed ) 
                CPPUNIT_FAIL( "GetLength on stdin should have failed." );


            // try getting the index (file position), should fail

            bFailed = false;
            try {
                FdoInt64 posn = fileStream->GetIndex();
            }
            catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue)
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode .
#ifdef _DEBUG
                FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
                FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Stream is not contextual (has no absolute position); FdoIoFileStream::GetIndex cannot be called ") == 0 );
#else
				FdoString* pMessage = e->GetExceptionMessage();
                FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Stream is not contextual (has no absolute position); FdoIoFileStream::GetIndex cannot be called ") == 0 );
#endif
#endif
                bFailed = true;
                e->Release();
            }
            if ( !bFailed ) 
                CPPUNIT_FAIL( "GetIndex on stdin should have failed." );

            // try skipping ahead, should fail
            bFailed = false;
            try {
                fileStream->Skip( 10 ) ;
            }
            catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue)
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode .
#ifdef _DEBUG
                FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
                FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Stream is not contextual (has no absolute position); FdoIoFileStream::Skip cannot be called ") == 0 );
#else
				FdoString* pMessage = e->GetExceptionMessage();
                FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Stream is not contextual (has no absolute position); FdoIoFileStream::Skip cannot be called ") == 0 );
#endif
#endif
                bFailed = true;
                e->Release();
            }
            if ( !bFailed ) 
                CPPUNIT_FAIL( "Skip on stdin should have failed." );

            // try resetting to start of file, should fail
            bFailed = false;
            try {
                fileStream->Reset() ;
            }
            catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue)
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode .
#ifdef _DEBUG
                FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
                FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Stream is not contextual (has no absolute position); FdoIoFileStream::Reset cannot be called ") == 0 );
#else
				FdoString* pMessage = e->GetExceptionMessage();
                FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Stream is not contextual (has no absolute position); FdoIoFileStream::Reset cannot be called ") == 0 );
#endif
#endif
                bFailed = true;
                e->Release();
            }
            if ( !bFailed ) 
                CPPUNIT_FAIL( "Reset on stdin should have failed." );

        }
    }
    catch ( FdoException* e ) {
        UnitTestUtil::FailOnException( e );
    }
}

void FdoIoTest::testFileCapabilities()
{
    try {
        FdoIoFileStreamP fileStream;

        fileStream = FdoIoFileStream::Create( L"testFile.txt", L"r" );
        FDO_CPPUNIT_ASSERT( fileStream->CanRead() == true );
        FDO_CPPUNIT_ASSERT( fileStream->CanWrite() == false );
        FDO_CPPUNIT_ASSERT( fileStream->HasContext() == true );

        fileStream = FdoIoFileStream::Create( L"testFileWrite.txt", L"w" );
        FDO_CPPUNIT_ASSERT( fileStream->CanRead() == false );
        FDO_CPPUNIT_ASSERT( fileStream->CanWrite() == true );
        FDO_CPPUNIT_ASSERT( fileStream->HasContext() == true );

        fileStream = FdoIoFileStream::Create( L"testFile.txt", L"r+" );
        FDO_CPPUNIT_ASSERT( fileStream->CanRead() == true );
        FDO_CPPUNIT_ASSERT( fileStream->CanWrite() == true );
        FDO_CPPUNIT_ASSERT( fileStream->HasContext() == true );

        fileStream = FdoIoFileStream::Create( L"testFile.txt", L"w+" );
        FDO_CPPUNIT_ASSERT( fileStream->CanRead() == true );
        FDO_CPPUNIT_ASSERT( fileStream->CanWrite() == true );
        FDO_CPPUNIT_ASSERT( fileStream->HasContext() == true );

        FILE* fp = fopen( "testFile.txt", "r" );
        fileStream = FdoIoFileStream::Create( fp );
        FDO_CPPUNIT_ASSERT( fileStream->CanRead() == true );
        FDO_CPPUNIT_ASSERT( fileStream->CanWrite() == false );
        FDO_CPPUNIT_ASSERT( fileStream->HasContext() == true );

        fileStream = FdoIoFileStream::Create( stdin );
        FDO_CPPUNIT_ASSERT( fileStream->CanRead() == true );
        FDO_CPPUNIT_ASSERT( fileStream->CanWrite() == false );

        CheckContext( fileStream, stdin );

        fileStream = FdoIoFileStream::Create( stdout );
        FDO_CPPUNIT_ASSERT( fileStream->CanRead() == false );
        FDO_CPPUNIT_ASSERT( fileStream->CanWrite() == true );
        CheckContext( fileStream, stdout );

        fileStream = FdoIoFileStream::Create( stderr );
        FDO_CPPUNIT_ASSERT( fileStream->CanRead() == false );
        FDO_CPPUNIT_ASSERT( fileStream->CanWrite() == true );
        CheckContext( fileStream, stderr );
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void FdoIoTest::testMemoryStream()
{
    try {
        FdoIoMemoryStreamP memoryStream = FdoIoMemoryStream::Create();;
        memoryStream->Write( (FdoByte*) "stuff", 5 );
        memoryStream->Reset();

        // try reading to null buffer, should fail.

        FdoBoolean bFailed = false;
        try {
            memoryStream->Read( NULL, 5 );
        }
        catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue)
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode .
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad buffer value '(NULL)' passed to FdoIoMemoryStream::Read. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad buffer value '(NULL)' passed to FdoIoMemoryStream::Read. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Reading to null buffer should have failed." );


        // try writing from null buffer, should fail.

       bFailed = false;
        try {
            memoryStream->Write( (FdoByte*) NULL, 5 );
        }
        catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue)
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode .
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad buffer value '(NULL)' passed to FdoIoMemoryStream::Write. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad buffer value '(NULL)' passed to FdoIoMemoryStream::Write. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing from null buffer should have failed." );


        // try writing from null stream, should fail

        bFailed = false;
        try {
            memoryStream->Write( (FdoIoStream*) NULL, 5 );
        }
        catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue)
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode .
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad stream value '(NULL)' passed to FdoIoMemoryStream::Write. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad stream value '(NULL)' passed to FdoIoMemoryStream::Write. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing from null stream should have failed." );
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

// Defect 763288 regression test.
// Ensures that streams do not get confused by the crlf to \n conversion
// that Windows does when reading text files.
void FdoIoTest::testTextFile()
{
    int lineCount = 2000;
    int ix;

    // Create the test data.
    FILE* fp = fopen("nums.txt", "wt" );
    FDO_CPPUNIT_ASSERT( fp );

    for ( ix = 0; ix < lineCount; ix++ )
        fprintf( fp, "0123456789\n" );

    fclose(fp);

    // Copy to output file via MemoryStream. Try various buffer sizes
    // for the memory stream.
    copyViaMemoryStream( 10, lineCount );
    copyViaMemoryStream( 150, lineCount );
    copyViaMemoryStream( 4096, lineCount );

    // Copy directly from input to output file.
    FdoIoFileStreamP inFile = FdoIoFileStream::Create( L"nums.txt", L"rt" );
    FdoIoFileStreamP outFile = FdoIoFileStream::Create( L"numsB.txt", L"wt" );
    outFile->Write( inFile );
    outFile = NULL;

    // Verify that everything got copied
    char buffer[50];
    fp = fopen("numsB.txt", "rt" );
    FDO_CPPUNIT_ASSERT( fp != NULL );

    ix = 0;
    while ( fgets(buffer, 49, fp) ) {
        ix++;
        FDO_CPPUNIT_ASSERT( strcmp(buffer,"0123456789\n") == 0 );
        buffer[0] = 0;
    }

    FDO_CPPUNIT_ASSERT( ix == lineCount );

    // 2nd copy (first 150 char only)
    inFile = FdoIoFileStream::Create( L"nums.txt", L"rt" );
    outFile = FdoIoFileStream::Create( L"numsB.txt", L"wt" );
    outFile->Write( inFile, 150 );
    outFile = NULL;

    // Verify that only first 150 char were copied
    fp = fopen("numsB.txt", "rt" );
    FDO_CPPUNIT_ASSERT( fp != NULL );

    ix = 0;
    while ( fgets(buffer, 49, fp) ) {
        ix++;
        if ( ix < 14 ) {
            FDO_CPPUNIT_ASSERT( strcmp(buffer,"0123456789\n") == 0 );
        }
        else {
            FDO_CPPUNIT_ASSERT( strcmp(buffer,"0123456") == 0 );
            break;
        }
        buffer[0] = 0;
    }

    FDO_CPPUNIT_ASSERT( ix == 14 );
}

void FdoIoTest::testLargeFile()
{
    try {
        FdoInt64 i;
        char buffer[11];
        FdoIoFileStreamP fileStream;

#if 1
        // Create a file larger than 4GB. The file contains a sequence
        // of 10 digit numbers where the number is the offset plus
        // 1,000,000,000.

        fileStream = FdoIoFileStream::Create( L"testLargeFile", L"w+" );

        for ( i = 100000000LL; i < 800000000LL; i++ ) {
            sprintf( buffer, "%d0", i );
            fileStream->Write( (FdoByte*) buffer, 10 );
        }
#endif
        // Text randomly accessing the large file.
        fileStream = FdoIoFileStream::Create( L"testLargeFile", L"r+" );

        FDO_CPPUNIT_ASSERT( fileStream->GetLength() == 7000000000LL );

        // Absolute skip
        fileStream->Skip( 5456235670LL );
        FdoSize count = fileStream->Read( (FdoByte*) buffer, 10 );
        buffer[count] = '\0';
        FDO_CPPUNIT_ASSERT( strcmp(buffer,"6456235670") == 0 );

        // another absolute skip
        fileStream->Reset();
        fileStream->Skip( 5256265675LL );
        count = fileStream->Read( (FdoByte*) buffer, 10 );
        buffer[count] = '\0';
        FDO_CPPUNIT_ASSERT( strcmp(buffer,"6567062562") == 0 );
                
        // Relative skip
        fileStream->Skip( 40 );
        count = fileStream->Read( (FdoByte*) buffer, 10 );
        buffer[count] = '\0';
        FDO_CPPUNIT_ASSERT( strcmp(buffer,"6572062562") == 0 );
                
        FDO_CPPUNIT_ASSERT( fileStream->GetIndex() == 5256265735LL );

        // Skip past end of file. Should be add end of file.
        fileStream->Skip( 5256265675LL );
        count = fileStream->Read( (FdoByte*) buffer, 10 );
        FDO_CPPUNIT_ASSERT( count == 0 );

#ifndef _WIN32
        // Change the length and verify that new length can be retrieved.
        fileStream->SetLength( 6200020058LL );
        FDO_CPPUNIT_ASSERT( fileStream->GetLength() == 6200020058LL );

        // try another skip after the length change.
        fileStream->Reset();
        fileStream->Skip( 6200020050LL );
        count = fileStream->Read( (FdoByte*) buffer, 10 );
        buffer[count] = '\0';
        FDO_CPPUNIT_ASSERT( strcmp(buffer,"72000200") == 0 );
#endif                
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void FdoIoTest::populateStream( FdoIoStream* stream)
{
    FdoInt32 i;
    char buffer[49];

    stream->Reset();

    for ( i = 0; i < LINE_COUNT; i++ ) {
        sprintf( buffer, "%010d ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\n", i );
        stream->Write( (FdoByte*) buffer, 48 );
    }
}

void FdoIoTest::vldStream( FdoIoStream* stream)
{
    FdoInt32 i;
    char buffer[49];
    char expected[49];

    FdoIoByteStreamReaderP reader = 
            FdoIoByteStreamReader::Create(stream);

    reader->Reset();

    FdoInt64 len = reader->GetLength();
    FDO_CPPUNIT_ASSERT(len == (LINE_COUNT * 48) );

    FdoInt64 pos = reader->GetIndex();
    FDO_CPPUNIT_ASSERT(pos == 0 );

    for ( i = 0; ; i++ ) {
        sprintf( expected, "%010d ABCDEFGHIJKLMNOPQRSTUVWXYZ012345", i );
        FdoInt32 readCount = reader->ReadNext( (FdoByte*) buffer, 0, 43 );

        if ( readCount == 0 ) 
            break;
        FDO_CPPUNIT_ASSERT(readCount == 43);

        buffer[readCount] = 0;
        FDO_CPPUNIT_ASSERT( strcmp( buffer, expected ) == 0 );

        reader->Skip(5);

        pos = stream->GetIndex();
        FDO_CPPUNIT_ASSERT(pos == ((i + 1) * 48) );
    }

    FDO_CPPUNIT_ASSERT(i == LINE_COUNT);
}

void FdoIoTest::bufferedWrite( FdoIoStream* inStream, FdoIoStream* outStream, FdoInt32 bufSize )
{
    FdoByte* buffer = new FdoByte[bufSize];

    inStream->Reset();
    outStream->Reset();

    FdoSize readCount = inStream->Read( buffer, bufSize );

    while ( readCount > 0 ) {
        outStream->Write( buffer, bufSize );
        readCount = inStream->Read( buffer, bufSize );
    }

    delete[] buffer;
}

void FdoIoTest::streamedWrite( FdoIoStream* inStream, FdoIoStream* outStream )
{
    inStream->Reset();
    outStream->Reset();

    outStream->Write(inStream);
}

void FdoIoTest::fdoArrayRead( FdoIoStream* stream)
{
    FdoInt32 i;
	FdoPtr<FdoByteArray> byteArray = FdoByteArray::Create(43);
    char expected[49];

    FdoIoByteStreamReaderP reader = 
            FdoIoByteStreamReader::Create(stream);

    reader->Reset();

    for ( i = 0; ; i++ ) {
        sprintf( expected, "%010d ABCDEFGHIJKLMNOPQRSTUVWXYZ012345", i );
        FdoInt32 readCount = reader->ReadNext( byteArray.p, 0, 43 );

        if ( readCount == 0 ) 
            break;
        FDO_CPPUNIT_ASSERT(readCount == 43);

        FDO_CPPUNIT_ASSERT( strncmp( (const char*) byteArray->GetData(), expected, readCount ) == 0 );

        reader->Skip(5);

    }
}

void FdoIoTest::CheckContext( FdoIoStream* stream, FILE* fp  )
{
    bool hasContext = false;

    try {
        FdoInt64 index = ftell( fp );

        if ( index >= 0 ) {
            // ftell succeeds if the stream is on a pipe but pipes aren't really 
            // fully contextual. I tried some fseeks on the pipes; they succeed but
            // the file pointer doesn't actually move.
            //
            // Check further to see if the stream is on a pipe. HasContext should be
            // false in this case.
#ifdef _WIN32
            int fd = _fileno(fp);
#else
            int fd = fileno(fp);
#endif
            struct stat fileStat;

            if ( fstat(fd, &fileStat) == 0 ) { 
#ifdef _WIN32
                hasContext = ( (fileStat.st_mode & _S_IFIFO) == 0 ); 
#else
                hasContext = ( (fileStat.st_mode & S_IFIFO) == 0 ); 
#endif
            }
        }
    }
    catch ( ... ) {
        hasContext = false;
    }

    if ( hasContext ) {
        FDO_CPPUNIT_ASSERT( stream->HasContext() == true );
    }
    else {
        FDO_CPPUNIT_ASSERT( stream->HasContext() == false );
    }
}

void FdoIoTest::copyViaMemoryStream( FdoSize bufSize, int lineCount )
{
    int ix;
    char outFileName[50];
    wchar_t outFileNameW[50];

    sprintf( outFileName, "nums%d.txt", lineCount );
    swprintf( outFileNameW, 49, L"%hs", outFileName ); 

    // 1st test copies entire input file
    FdoIoFileStreamP inFile = FdoIoFileStream::Create( L"nums.txt", L"rt" );
    FdoIoFileStreamP outFile = FdoIoFileStream::Create( outFileNameW, L"wt" );
    FdoIoMemoryStreamP intStream = FdoIoMemoryStream::Create(bufSize);

    intStream->Write( inFile );
    intStream->Reset();
    outFile->Write( intStream );
    outFile = NULL;

    // Verify that entire file was copied.
    char buffer[50];
    FILE* fp = fopen((const char*) outFileName, "rt" );
    FDO_CPPUNIT_ASSERT( fp != NULL );

    ix = 0;
    while ( fgets(buffer, 49, fp) ) {
        ix++;
        FDO_CPPUNIT_ASSERT( strcmp(buffer,"0123456789\n") == 0 );
        buffer[0] = 0;
    }

    FDO_CPPUNIT_ASSERT( ix == lineCount );

    // 2nd test just copies the 1st 150 char.
    inFile = FdoIoFileStream::Create( L"nums.txt", L"rt" );
    outFile = FdoIoFileStream::Create( outFileNameW, L"wt" );
    intStream = FdoIoMemoryStream::Create(bufSize);

    intStream->Write( inFile, 150 );
    intStream->Reset();
    outFile->Write( intStream );

    // Verify that only 1st 150 char were copied.
    fp = fopen((const char*) outFileName, "rt" );
    FDO_CPPUNIT_ASSERT( fp != NULL );

    ix = 0;
    while ( fgets(buffer, 49, fp) ) {
        ix++;
        if ( ix < 14 ) {
            FDO_CPPUNIT_ASSERT( strcmp(buffer,"0123456789\n") == 0 );
        }
        else {
            FDO_CPPUNIT_ASSERT( strcmp(buffer,"0123456") == 0 );
            break;
        }
        buffer[0] = 0;
    }

    FDO_CPPUNIT_ASSERT( ix == 14 );
}

