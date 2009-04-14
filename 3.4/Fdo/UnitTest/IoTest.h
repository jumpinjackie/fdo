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


#ifndef CPP_UNIT_FDOIOTEST_H
#define CPP_UNIT_FDOIOTEST_H


// Test cases for the Schema classes.
class FdoIoTest : public CppUnit::TestCase
{
    FDO_CPPUNIT_DEFINE(testStreams);
    FDO_CPPUNIT_DEFINE(testFileOpen);
    FDO_CPPUNIT_DEFINE(testFileRead);
    FDO_CPPUNIT_DEFINE(testFileWrite);
    FDO_CPPUNIT_DEFINE(testFileContext);
    FDO_CPPUNIT_DEFINE(testFileCapabilities);
    FDO_CPPUNIT_DEFINE(testMemoryStream);
    FDO_CPPUNIT_DEFINE(testTextFile);
    FDO_CPPUNIT_DEFINE(testLargeFile);

    CPPUNIT_TEST_SUITE(FdoIoTest);

    CPPUNIT_TEST(testStreams);
    CPPUNIT_TEST(testFileOpen);
    CPPUNIT_TEST(testFileRead);
    CPPUNIT_TEST(testFileWrite);
    CPPUNIT_TEST(testFileContext);
    CPPUNIT_TEST(testFileCapabilities);
    CPPUNIT_TEST(testMemoryStream);
    CPPUNIT_TEST(testTextFile);
// This test is very time-consuming so not part of the regular test suite.
//    CPPUNIT_TEST(testLargeFile);
    CPPUNIT_TEST_SUITE_END();

public:
    FdoIoTest(void);
    virtual ~FdoIoTest(void);
    void setUp ();

protected:
    void testStreams();

    // Most of the following are error handling tests.
    // Happy tests are performed by higher level object tests that use streams.
    void testFileOpen();
    void testFileRead();
    void testFileWrite();
    void testFileContext();
    void testFileCapabilities();
    void testMemoryStream();
    void testTextFile();
    void testLargeFile();

    void populateStream( FdoIoStream* stream );
    void vldStream( FdoIoStream* stream );
    void bufferedWrite( FdoIoStream* inStream, FdoIoStream* outStream, FdoInt32 bufSize = 100 );
    void streamedWrite( FdoIoStream* inStream, FdoIoStream* outStream );
    void fdoArrayRead( FdoIoStream* stream );
    void CheckContext( FdoIoStream* stream, FILE* fp );
    void copyViaMemoryStream( FdoSize bufSize, int lineCount );
};

#endif

