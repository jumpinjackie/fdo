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
#include "XmlTest.h"
#include "UnitTestUtil.h"
#include "XmlFormatter.h"
#ifdef _DEBUG
//#define DEBUG_DETAIL  1
#endif

static char* pXmlIn = 
"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<DataSet>\
<Colours><Red>Rouge</Red><Black>Noir</Black><Yellow>Jaune</Yellow></Colours>\
<Sizes><Small>Petit</Small><Medium>Moyen</Medium><Large>Grand</Large></Sizes>\
</DataSet>";

static char* pStylesheet = 
"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<stylesheet version=\"1.0\" xmlns=\"http://www.w3.org/1999/XSL/Transform\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">\
<xsl:param name=\"animal\" />\
<xsl:param name=\"colour\" />\
<xsl:param name=\"size\" />\
<xsl:template match=\"DataSet\">\
<xsl:element name=\"{$animal}\">\
<xsl:for-each select=\"Colours/node()[local-name()=$colour]\">\
<xsl:attribute name=\"colour\"><xsl:value-of select=\".\"/></xsl:attribute>\
</xsl:for-each>\
<xsl:attribute name=\"size\"><xsl:value-of select=\"$size\"/></xsl:attribute>\
</xsl:element>\
</xsl:template>\
</stylesheet>";

static char* pBadStylesheet = 
"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<stylesheet version=\"1.0\" xmlns=\"http://www.w3.org/1999/XSL/Transform\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">\
<xsl:param name=\"animal\" />\
<xsl:param name=\"colour\" junk=\"bad\" />\
<xsl:param name=\"size\" />\
<xsl:template match=\"DataSet\">\
<xsl:element name=\"{$animal}\">\
<xsl:for-each select=\"Colours/node()[local-name()=$colour]\">\
<xsl:attribute name=\"colour\"><xsl:value-of select=\".\"/></xsl:attribute>\
</xsl:for-each>\
<xsl:attribute name=\"size\"><xsl:value-of select=\"$size\"/></xsl:attribute>\
</xsl:element>\
</xsl:template>\
</stylesheet>";

static char* pExpectedOut = 
"<?xml version=\"1.0\" encoding=\"UTF-8\" ?><Dog xmlns=\"http://www.w3.org/1999/XSL/Transform\" colour=\"Rouge\" size=\"Moyen\"/>";

CPPUNIT_TEST_SUITE_REGISTRATION (XmlTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (XmlTest, "XmlTest");

XmlTest::XmlTest(void)
{
}

XmlTest::~XmlTest(void)
{
}

void XmlTest::setUp()
{
    // nothing to do at setup time... yet
}

void XmlTest::testXsl()
{
    try {      
        FdoIoMemoryStreamP inStream = FdoIoMemoryStream::Create();
        FdoIoMemoryStreamP stylesheetStream = FdoIoMemoryStream::Create();
        FdoIoMemoryStreamP outStream = FdoIoMemoryStream::Create();

        // Load the in document and stylesheet from static strings.
        inStream->Write( (FdoByte*) pXmlIn, strlen(pXmlIn) );
        inStream->Reset();
        stylesheetStream->Write( (FdoByte*) pStylesheet, strlen(pStylesheet) );
        stylesheetStream->Reset();

        // create transformer
        FdoXslTransformerP tfmr = FdoXslTransformer::Create(
            FdoXmlReaderP( FdoXmlReader::Create(inStream) ),
            FdoXmlReaderP( FdoXmlReader::Create(stylesheetStream) ),
            FdoXmlWriterP( FdoXmlWriter::Create(outStream, false) )
        );

        // Set the stylesheet parameters for selecting Dog size and colour
        FdoDictionaryP params = tfmr->GetParameters();
        // Root element
        params->Add( FdoDictionaryElementP( FdoDictionaryElement::Create( L"animal", L"'Dog'" ) ) );
        // Literal colour to translate
        params->Add( FdoDictionaryElementP( FdoDictionaryElement::Create( L"colour", L"'Red'" ) ) );
        // Size to translate, as an xpath expression
        params->Add( FdoDictionaryElementP( FdoDictionaryElement::Create( L"size", L"/DataSet/Sizes/node()[local-name()='Medium']" ) ) );

        tfmr->Transform();

        outStream->Reset();

        // Read output into string
        char* pXmlOut = new char[(FdoSize)(outStream->GetLength() + 1)];
        outStream->Read( (FdoByte*) pXmlOut, (FdoSize)(outStream->GetLength()) );
        pXmlOut[(FdoSize)(outStream->GetLength())] = 0;

        // Make sure we got expected results.
        FDO_CPPUNIT_ASSERT(strcmp(pXmlOut, pExpectedOut) == 0);
        delete[] pXmlOut;

        // Test some error conditions
        
        // read from streams at EOF.

        FdoBoolean bFailed = false;
        try {
            tfmr->Transform();
        }
        catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Cannot XSL Transform XML document whose stream is at end-of-file. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Cannot XSL Transform XML document whose stream is at end-of-file. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Transforming from stream at EOF should have failed." );

        inStream->Reset();

        bFailed = false;
        try {
            tfmr->Transform();
        }
        catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Cannot read XSL stylesheet whose stream is at end-of-file. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Cannot read XSL stylesheet whose stream is at end-of-file. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Transforming with stylesheet at EOF should have failed." );

        // bad arguments

        bFailed = false;
        try {
           tfmr = FdoXslTransformer::Create(
               FdoXmlReaderP( FdoXmlReader::Create(inStream) ),
               FdoXmlReaderP( FdoXmlReader::Create(stylesheetStream) ),
               NULL
        );

        }
        catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad outDoc value '(NULL)' passed to FdoXslTransformer::SetOutDoc. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad outDoc value '(NULL)' passed to FdoXslTransformer::SetOutDoc. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing to null outDoc should have failed." );

        bFailed = false;
        try {
           tfmr = FdoXslTransformer::Create(
               FdoXmlReaderP( FdoXmlReader::Create(inStream) ),
               NULL,
               NULL
        );

        }
        catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad stylesheet value '(NULL)' passed to FdoXslTransformer::SetStylesheet. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad stylesheet value '(NULL)' passed to FdoXslTransformer::SetStylesheet. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Transforming from null stylesheet should have failed." );

        bFailed = false;
        try {
           tfmr = FdoXslTransformer::Create(
               NULL,
               NULL,
               NULL
        );

        }
        catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad inDoc value '(NULL)' passed to FdoXslTransformer::SetInDoc. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Bad inDoc value '(NULL)' passed to FdoXslTransformer::SetInDoc. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Transforming from null inDoc should have failed." );

        stylesheetStream->SetLength(0);
        stylesheetStream->Write( (FdoByte*) pBadStylesheet, strlen(pBadStylesheet) );
        stylesheetStream->Reset();
        inStream->Reset();
        outStream->SetLength(0);

        tfmr = FdoXslTransformer::Create(
            FdoXmlReaderP( FdoXmlReader::Create(inStream) ),
            FdoXmlReaderP( FdoXmlReader::Create(stylesheetStream) ),
            FdoXmlWriterP( FdoXmlWriter::Create(outStream, false) )
        );

// Temporary disable the next two tests on Windows
#ifndef _WIN32
        bFailed = false;
        try {
           tfmr->Transform();   
        }
        catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcsncmp(pMessage, L"XSLTProcessorException: 'xsl:variable' has an invalid 'junk' attribute (sheet, line 1, column 225) ",100) == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcsncmp(pMessage, L"XSLTProcessorException: 'xsl:variable' has an invalid 'junk' attribute (sheet, line 1, column 225) ",100) == 0 );
#endif
            
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Transforming from bad stylesheet should have failed." );

        // Test against input streams (StringStream) that have no set length. Verify that we don't get
        // end-of-file error messages in this case.
        tfmr = FdoXslTransformer::Create(
            FdoXmlReaderP( FdoXmlReader::Create(StringStreamP(new StringStream(pXmlIn))) ),
            FdoXmlReaderP( FdoXmlReader::Create(StringStreamP(new StringStream(pStylesheet))) ),
            FdoXmlWriterP( FdoXmlWriter::Create(outStream, false) )
        );
        tfmr->Transform();
#endif
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void XmlTest::testNameAdjust()
{
    try {
        FdoXmlReaderP reader = FdoXmlReader::Create( 
            FdoIoMemoryStreamP( FdoIoMemoryStream::Create() )
        );

        FdoXmlWriterP writer = FdoXmlWriter::Create( 
            FdoIoMemoryStreamP( FdoIoMemoryStream::Create() )
        );

        // Round-trip various invalid XML names.

        FdoStringP name1 = L"Abc def";
        FdoStringP name2 = writer->EncodeName(name1);
        FDO_CPPUNIT_ASSERT( name2 == L"Abc-x20-def" );
        FDO_CPPUNIT_ASSERT( name1 == reader->DecodeName(name2) );
 
        name1 = L" Abc#defg$$";
        name2 = writer->EncodeName(name1);
        FDO_CPPUNIT_ASSERT( name2 == L"-x20-Abc-x23-defg-x24--x24-" );
        FDO_CPPUNIT_ASSERT( name1 == reader->DecodeName(name2) );
        
        name1 = L" Abc#defg hij";
        name2 = writer->EncodeName(name1);
        FDO_CPPUNIT_ASSERT( name2 == L"-x20-Abc-x23-defg-x20-hij" );
        FDO_CPPUNIT_ASSERT( name1 == reader->DecodeName(name2) );
        
        name1 = L"--abc-def---ghi--";
        name2 = writer->EncodeName(name1);
        FDO_CPPUNIT_ASSERT( name2 == L"--abc-def---ghi--" );
        FDO_CPPUNIT_ASSERT( name1 == reader->DecodeName(name2) );
        
        name1 = L"--abc-x20-def-x23--x24-ghi--";
        name2 = writer->EncodeName(name1);
        FDO_CPPUNIT_ASSERT( name2 == L"--abc-x2d-x20-def-x2d-x23--x2d-x24-ghi--" );
        FDO_CPPUNIT_ASSERT( name1 == reader->DecodeName(name2) );

        name1 = L"-xab";
        name2 = writer->EncodeName(name1);
        FDO_CPPUNIT_ASSERT( name2 == L"-x2d-xab" );
        FDO_CPPUNIT_ASSERT( name1 == reader->DecodeName(name2) );
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void XmlTest::testRead()
{
    try {
        FdoXmlReaderP reader;
        FdoBoolean bFailed = false;

        // Test various XML reader errors.
        // SchemaTest covers most of the happy tests

        try {
            reader = FdoXmlReader::Create( L"nonexistent.xml" );
        }
        catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Failed to open file 'nonexistent.xml' with access modes: 'rt'. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Failed to open file 'nonexistent.xml' with access modes: 'rt'. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }

        if ( !bFailed ) 
            CPPUNIT_FAIL( "Reading from non-existent file should have failed." );

        bFailed = false;
        try {
            reader = FdoXmlReader::Create( (FdoIoStream*) NULL );
        }
        catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Bad stream value '(NULL)' passed to FdoIoTextReader::Create. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Bad stream value '(NULL)' passed to FdoIoTextReader::Create. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }

        if ( !bFailed ) 
            CPPUNIT_FAIL( "Reading from null stream should have failed." );

        bFailed = false;
        try {
            reader = FdoXmlReader::Create( (FdoIoTextReader*) NULL );
        }
        catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Bad reader value '(NULL)' passed to FdoXmlReader::Create. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Bad reader value '(NULL)' passed to FdoXmlReader::Create. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }

        if ( !bFailed ) 
            CPPUNIT_FAIL( "Reading from null text reader should have failed." );

        bFailed = false;
        reader = FdoXmlReader::Create( 
            FdoIoMemoryStreamP( FdoIoMemoryStream::Create() )
        );
/* For future - if GetSaxHandler is ever added to the FDO API:
        FdoXmlSaxHandler* handler = reader->GetSaxHandler();
        FDO_CPPUNIT_ASSERT( handler == NULL );
        try {
            handler = reader->PopSaxHandler();
        }
        catch ( FdoException* e ) {
            FDO_CPPUNIT_ASSERT( wcscmp(e->GetExceptionMessage(),L"Stack is empty, cannot pop.") == 0 );
            bFailed = true;
            e->Release();
        }

        if ( !bFailed ) 
            CPPUNIT_FAIL( "Reading from null text reader should have failed." );
*/

        // Test nested call to Parse, must fail.
        FdoIoMemoryStreamP inStream = FdoIoMemoryStream::Create();
        inStream->Write( (FdoByte*) pXmlIn, strlen(pXmlIn) );
        inStream->Reset();

        reader = FdoXmlReader::Create(inStream);
        NesterP nester = Nester::Create(reader);

        bFailed = false;
        try {
            reader->Parse( nester );
        }
        catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Nested call to FdoXmlReader::Parse() attempted. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Nested call to FdoXmlReader::Parse() attempted. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }

        if ( !bFailed ) 
            CPPUNIT_FAIL( "Re-entrant parse should have failed." );

        // Try Parse again, should succeed this time.
        CounterP counter = Counter::Create();
        inStream->Reset();
        reader->Parse(counter);

        CPPUNIT_ASSERT( counter->GetCount() == 9 );
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void XmlTest::testWrite()
{
    try {
        FdoXmlWriterP writer;
        FdoBoolean bFailed = false;

        // Test various XML writer errors.
        // SchemaTest covers most of the happy tests

        bFailed = false;
        try {
            writer = FdoXmlWriter::Create( (FdoIoStream*) NULL );
        }
        catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Bad stream value '(NULL)' passed to FdoIoTextWriter::Create. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Bad stream value '(NULL)' passed to FdoIoTextWriter::Create. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }

        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing to null stream should have failed." );

        bFailed = false;
        try {
            writer = FdoXmlWriter::Create( (FdoIoTextWriter*) NULL );
        }
        catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Bad writer value '(NULL)' passed to FdoXmlWriter::Create. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Bad writer value '(NULL)' passed to FdoXmlWriter::Create. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }

        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing to null text reader should have failed." );

        writer = FdoXmlWriter::Create( 
            FdoIoMemoryStreamP( FdoIoMemoryStream::Create() ), 
            false
        );

        bFailed = false;
        try {
            writer->WriteCharacters( L"abcd" );
        }
        catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"FdoXmlWriter::WriteCharacters called when there is no open element; content is 'abcd'. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"FdoXmlWriter::WriteCharacters called when there is no open element; content is 'abcd'. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing content before start element should have failed" );

        bFailed = false;
        try {
            writer->WriteStartElement(NULL);
        }
        catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Bad elementName value '(NULL)' passed to FdoXmlWriter::WriteStartElement. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Bad elementName value '(NULL)' passed to FdoXmlWriter::WriteStartElement. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing null start element should have failed" );

        bFailed = false;
        try {
            writer->WriteStartElement(L"bad element name");
        }
        catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Element 'bad element name' is not a valid XML 1.0 element name. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Element 'bad element name' is not a valid XML 1.0 element name. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing invalid start element should have failed" );

        writer->WriteStartElement(L"rootElement");

        bFailed = false;
        try {
            writer->WriteAttribute(NULL, L"test");
        }
        catch ( FdoException* e ) {

#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Bad attributeName value '(NULL)' passed to FdoXmlWriter::WriteAttribute. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Bad attributeName value '(NULL)' passed to FdoXmlWriter::WriteAttribute. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing null attribute should have failed" );

        bFailed = false;
        try {
            writer->WriteAttribute(L"bad attribute name", L"test");
        }
        catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Attribute 'bad attribute name' is not a valid XML 1.0 attribute name. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Attribute 'bad attribute name' is not a valid XML 1.0 attribute name. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing invalid attribute should have failed" );

        bFailed = false;
        writer->WriteStartElement(L"nextElement" );
        writer->WriteEndElement();
        try {
            writer->WriteAttribute(L"noElement", L"test");
        }
        catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Cannot add attribute noElement; tag for element rootElement is no longer open. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Cannot add attribute noElement; tag for element rootElement is no longer open. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing attribute when no open element should have failed" );

        bFailed = false;
        try {
            writer->SetDefaultRoot(true);
        }
        catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Cannot change default root setting for FdoXmlWriter; root element has already been written. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Cannot change default root setting for FdoXmlWriter; root element has already been written. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "SetDefaultRoot should have failed" );

        bFailed = false;
        writer->WriteEndElement();
        try {
            writer->WriteEndElement();
        }
        catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"FdoXmlWriter::EndElement: there is no current element to end. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"FdoXmlWriter::EndElement: there is no current element to end. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing end element when no open element should have failed" );

        bFailed = false;
        try {
            writer->WriteStartElement(L"root2");
        }
        catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"FdoXmlWriter::WriteStartElement: cannot write second root element 'root2' to XML document. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"FdoXmlWriter::WriteStartElement: cannot write second root element 'root2' to XML document. ") == 0 );
#endif 
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing 2nd root element should have failed" );
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void XmlTest::testWriteClose()
{
    // The following verifies that an XML document cannot be written to
    // after Close() is called on the writer.

    try {
        // Simple case: close empty document

        FdoIoMemoryStreamP stream = FdoIoMemoryStream::Create(); 
        FdoXmlWriterP writer = FdoXmlWriter::Create( stream, false );
        writer->Close();
        WriteAfterClose( writer );

        // Another simple case: close document with default root element

        stream = FdoIoMemoryStream::Create(); 
        writer = FdoXmlWriter::Create( stream );
        writer->Close();
        WriteAfterClose( writer );

        // More complicated case: close document that has some open elements.

        stream = FdoIoMemoryStream::Create(); 
        writer = FdoXmlWriter::Create( stream );
        writer->WriteStartElement( L"element1" );
        writer->WriteStartElement( L"element2" );
        writer->WriteStartElement( L"element3" );
        writer->WriteStartElement( L"element4" );
        writer->Close();
        WriteAfterClose( writer );
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void XmlTest::testReadWrite()
{
    try {
        FdoIoMemoryStreamP stream = FdoIoMemoryStream::Create();
        FdoXmlWriterP writer = FdoXmlWriter::Create( stream );

        writer->WriteStartElement( L"an_element" );
        writer = NULL;

        FdoXmlReaderP reader = FdoXmlReader::Create( stream );
        
        bool bFailed = false;
        try {
            reader->Parse();
        }
        catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Cannot parse XML document whose stream is at end-of-file. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"Cannot parse XML document whose stream is at end-of-file. ") == 0 );
#endif
#endif
            bFailed = true;
            e->Release();
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Reading from stream at EOF should have failed." );

        stream->Reset();
        reader = FdoXmlReader::Create( stream );
        CounterP counter = Counter::Create();
        reader->Parse( counter );
        FDO_CPPUNIT_ASSERT( counter->GetCount() == 2 );

        // Test against input streams (StringStream) that have no set length. Verify that we don't get
        // end-of-file error messages in this case.
        reader = FdoXmlReader::Create( StringStreamP(new StringStream(pXmlIn)) );
        reader->Parse();

    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void XmlTest::testCopy()
{
    try {
		// Stream 1 is the original XML document
		FdoIoMemoryStreamP stream1 = FdoIoMemoryStream::Create();
		FdoXmlWriterP writer1 = FdoXmlWriter::Create(stream1);

		// Stream 2 is the output XML document. It just contains
		// the CopyTag elements found in stream 1
		FdoIoMemoryStreamP stream2 = FdoIoMemoryStream::Create();
		FdoXmlWriterP writer2 = FdoXmlWriter::Create( stream2);

		// Stream E is the expected output
		FdoIoMemoryStreamP streamE = FdoIoMemoryStream::Create();
		FdoXmlWriterP writerE = FdoXmlWriter::Create( streamE);

		FdoInt32 i;

		// Write the stream1 document. It contains 5 CopyTag elements
		// nested at various levels.
		for ( i = 0; i < 5; i++ ) {
			AddCopyWrapper( writer1, i, i, L"src" );
		}

		writer1->Close();

		// Extract all CopyTag elements from stream 1 to stream 2
		// This exercises the FdoXmlCopyHandler class.
		ExtractorP extractor = Extractor::Create( writer2 );

		stream1->Reset();
		FdoXmlReaderP reader = FdoXmlReader::Create( stream1 );
		reader->Parse( extractor );

		extractor = NULL;
		writer2 = NULL;

		// Write the expected out document.
		for ( i = 0; i < 5; i++ ) {
			AddCopyWrapper( writerE, i, i, L"dest" );
		}

		writerE = NULL;

		// This test automatically validates itself by comparing stream 1
		// with the expected output. However, for debugging purposes all
		// these streams are written to files.
		stream1->Reset();
		stream2->Reset();
		streamE->Reset();

		FdoIoFileStreamP out1 = FdoIoFileStream::Create( L"XmlCopyTestOrig.xml", L"wt+" );
        XmlFormatter formatter1(stream1, out1);
        formatter1.Format();
		out1 = NULL;

		FdoIoFileStreamP out2 = FdoIoFileStream::Create( L"XmlCopyTest.xml", L"wt+" );
        XmlFormatter formatter2(stream2, out2);
        formatter2.Format();
		out2 = NULL;

		FdoIoFileStreamP outE = FdoIoFileStream::Create( L"XmlCopyExpected.xml", L"wt+" );
        XmlFormatter formatterE(streamE, outE);
        formatterE.Format();
		outE = NULL;

		stream2->Reset();
		streamE->Reset();

		// Extract the output and expected documents into strings.
		char* buf2 = new char[(size_t)(stream2->GetLength() + 1)];
		char* bufE = new char[(size_t)(streamE->GetLength() + 1)];

		stream2->Read( (FdoByte*) buf2, (FdoInt32) stream2->GetLength() );
		buf2[stream2->GetLength()] = 0;
		streamE->Read( (FdoByte*) bufE, (FdoInt32) streamE->GetLength() );
		bufE[streamE->GetLength()] = 0;

		// Check if the documents are identical. If they are not, you can
		// do a diff's (windiff on Win32, diff on Linux) on the above-mentioned
		// files to see where the differences are.
		FDO_CPPUNIT_ASSERT( strcmp(buf2, bufE) == 0 );

        delete[] buf2;
        delete[] bufE;
	}
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

// The following must be passed a closed XML writer. It tries various
// writing functions which must fail.
void XmlTest::WriteAfterClose( FdoXmlWriter* writer ) 
{
    // Try writing an element
    FdoBoolean bFailed = false;

    try {
        writer->WriteStartElement(L"element10");
    }
    catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
        FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
        FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"FdoXmlWriter::WriteStartElement: cannot write second root element 'element10' to XML document. ") == 0 );
#else
		FdoString* pMessage = e->GetExceptionMessage();
        FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"FdoXmlWriter::WriteStartElement: cannot write second root element 'element10' to XML document. ") == 0 );
#endif
#endif
        bFailed = true;
        e->Release();
    }

    if ( !bFailed ) 
        CPPUNIT_FAIL( "Writing to closed XML writer should have failed." );

    // Try ending the current element (actually no current element at this point
    // since writer is closed).
    bFailed = false;
    try {
        writer->WriteEndElement();
    }
    catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
        FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
        FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"FdoXmlWriter::EndElement: there is no current element to end. ") == 0 );
#else
		FdoString* pMessage = e->GetExceptionMessage();
        FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"FdoXmlWriter::EndElement: there is no current element to end. ") == 0 );
#endif
#endif
        bFailed = true;
        e->Release();
    }

    if ( !bFailed ) 
        CPPUNIT_FAIL( "Writing to closed XML writer should have failed." );

    // Try writing an attribute
    bFailed = false;
    try {
        writer->WriteAttribute(L"attr1", L"val1");
    }
    catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
        FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
        FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"FdoXmlWriter::WriteAttribute called when there is no open element; cannot write attribute 'attr1'. ") == 0 );
#else
		FdoString* pMessage = e->GetExceptionMessage();
        FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"FdoXmlWriter::WriteAttribute called when there is no open element; cannot write attribute 'attr1'. ") == 0 );
#endif
#endif
        bFailed = true;
        e->Release();
    }

    if ( !bFailed ) 
        CPPUNIT_FAIL( "Writing to closed XML writer should have failed." );

    // Try writing element content
    bFailed = false;
    try {
        writer->WriteCharacters(L"some characters");
    }
    catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
        FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
        FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"FdoXmlWriter::WriteCharacters called when there is no open element; content is 'some characters'. ") == 0 );
#else
		FdoString* pMessage = e->GetExceptionMessage();
        FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"FdoXmlWriter::WriteCharacters called when there is no open element; content is 'some characters'. ") == 0 );
#endif
#endif
        bFailed = true;
        e->Release();
    }

    if ( !bFailed ) 
        CPPUNIT_FAIL( "Writing to closed XML writer should have failed." );

    // Try writing some arbitrary data.
    bFailed = false;
    try {
        char* someBytes = "<element20/>";
        writer->WriteBytes((FdoByte*) someBytes, 13);
    }
    catch ( FdoException* e ) {
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
        FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
        FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"FdoXmlWriter::WriteBytes called after document closed; cannot write bytes. ") == 0 );
#else
		FdoString* pMessage = e->GetExceptionMessage();
        FDO_CPPUNIT_ASSERT( wcscmp(pMessage,L"FdoXmlWriter::WriteBytes called after document closed; cannot write bytes. ") == 0 );
#endif
#endif
        bFailed = true;
        e->Release();
    }

    if ( !bFailed ) 
        CPPUNIT_FAIL( "Writing to closed XML writer should have failed." );
}

void XmlTest::AddCopyWrapper( FdoXmlWriter* writer, FdoInt32 idx, FdoInt32 wrapperLevel, FdoString* changeNamespace )
{
	FdoInt32 i;

	// If writing the original document, nest the CopyTag elements at
	// various levels.
	if ( wcscmp(changeNamespace, L"src") == 0 ) {
		for ( i = 0; i < (wrapperLevel % 3); i++ ) {
			writer->WriteStartElement( 
				FdoStringP::Format( L"fds:wrapper%d", i )
			);
 
			// src namespace is referenced inside CopyTag
			if ( i == 0 ) 
				writer->WriteAttribute( L"xmlns:src", L"http://www.changenamespace.com" );
		}
	}

	// Write the CopyTag element
	AddCopyPart( writer, idx, changeNamespace ); 

	// Close the wrapper elements if any
	if ( wcscmp(changeNamespace, L"src") == 0 ) {
		for ( i = 0; i < (wrapperLevel % 3); i++ ) {
			writer->WriteEndElement();
		}
	}
}

void XmlTest::AddCopyPart( FdoXmlWriter* writer, FdoInt32 idx, FdoString* changeNamespace )
{
	// Write the element to copy.
	writer->WriteStartElement( L"fds:CopyTag" );

	// Test copying attributes
	writer->WriteAttribute( 
		L"index",
		FdoStringP::Format( L"%d", idx )
	);
	writer->WriteAttribute( 
		L"copyable",
		L"yes"
	);

	// Test a namespace declared on the element to copy
	writer->WriteAttribute( L"xmlns:ctn", L"http://www.ctn.com" );

	// Test namespace change. The original doc has a "src" namespace but
	// the equivalent namespace in the output doc is called "dest".
	// The src namespace is declared on a wrapper element, if any.

	// If writing the expected results, add the dest namespace declaration
	if ( (wcscmp(changeNamespace,L"dest") == 0) || (idx % 3) ) 
		writer->WriteAttribute( L"xmlns:dest", L"http://www.changenamespace.com" );
	
	// Test a namespace that is not present in the output document
	if ( wcscmp(changeNamespace,L"src") == 0 ) 
		writer->WriteAttribute( L"xmlns:srconly", L"http://www.sourceonly.com" );

	// Test sub-elements and the copying of element content 
	writer->WriteStartElement( L"ctn:testchar" );
	if ( wcscmp(changeNamespace,L"dest") == 0 ) 
		writer->WriteAttribute( L"xmlns:srconly", L"http://www.sourceonly.com" );
	writer->WriteCharacters( L"abcde" );
	writer->WriteEndElement();

	// Try a nested CopyTag, should be treated like any other element.
	writer->WriteStartElement( L"fds:CopyTag" );
	writer->WriteAttribute( L"attr1", L"val1" );
	writer->WriteAttribute( L"attr2", L"val2" );
	writer->WriteAttribute( L"attrEmpty", L"" );
	if ( wcscmp(changeNamespace,L"dest") == 0 ) 
		writer->WriteAttribute( L"xmlns:srconly", L"http://www.sourceonly.com" );

	writer->WriteStartElement( L"fds:testSubAttr" );
	writer->WriteAttribute( L"attr1", L"val1" );
	writer->WriteEndElement();
	writer->WriteEndElement();

	// Test a sub-element in namespace declared on CopyTag. Should get
	// copied as is.
	writer->WriteStartElement( L"ctn:testchar" );
	if ( wcscmp(changeNamespace,L"dest") == 0 ) 
		writer->WriteAttribute( L"xmlns:srconly", L"http://www.sourceonly.com" );
	writer->WriteCharacters( L"ghijkl" );
	writer->WriteEndElement();

	writer->WriteStartElement( L"fds:testAttr" );
	writer->WriteAttribute( L"attr1", L"val1b" );
    if ( idx % 3 ) 
        writer->WriteAttribute( FdoStringP(changeNamespace) + L":attr2", L"val2b" );
    else
        writer->WriteAttribute( L"attr2", L"val2b" );
	writer->WriteAttribute( L"attrEmpty", L"" );
	if ( wcscmp(changeNamespace,L"dest") == 0 ) 
		writer->WriteAttribute( L"xmlns:srconly", L"http://www.sourceonly.com" );

	writer->WriteStartElement( L"fds:testSubAttr" );
	writer->WriteEndElement();

	// Test copying an element whose namespace changes ( "src" in original 
	// doc, "dest" in output doc ).
	if ( idx % 3 ) {
		writer->WriteStartElement( FdoStringP(changeNamespace) + L":ChangeNS" );
		writer->WriteEndElement();
	}

	// Test copying an element whose namespace is not declared in the 
	// output document. 
	if ( wcscmp(changeNamespace,L"src") == 0 ) {
		// This is how it looks in the input document.
		writer->WriteStartElement( L"srconly:test" );
		writer->WriteStartElement( L"srconly:subelement" );
	}
	else {
		// In the output document, the element ends up unqualified ...
        writer->WriteStartElement( L"srconly:test" );
        writer->WriteStartElement( L"srconly:subelement" );
	}

	writer->WriteEndElement();
	writer->WriteEndElement();

	// Test an unqualified element ( no namespace and hence no URI ).
	// Should get copied as is.
	writer->WriteStartElement( L"unqualified" );
	writer->WriteEndElement();

	// Test an unqualified element with undeclared URI.
	// Should get written as is. 
	writer->WriteStartElement( L"dfltqualified" );
		writer->WriteAttribute( L"xmlns", L"http://www.defaultqualified.com" );
	writer->WriteStartElement( L"subelement" );
	writer->WriteEndElement();
	// Add sub-element with namespace declaration for default namespace ..
	writer->WriteStartElement( L"subelement2" );
    writer->WriteAttribute( L"xmlns:dfl", L"http://www.defaultqualified.com" );
	if ( wcscmp(changeNamespace,L"src") == 0 ) {
		// plus sub-sub-element in the namespace
		writer->WriteStartElement( L"dfl:subelement2a" );
        // Attribute in default namespace
        writer->WriteAttribute( L"dfl:attr1", L"value1" );
		writer->WriteEndElement();
	}
	else {
		// In output document, sub-sub-element ends up unqualified 
		// since its namespace is the default.
        writer->WriteStartElement( L"dfl:subelement2a" );
        // Attribute in default namespace still qualified since
        // default namespaces do not apply to attributes.
        writer->WriteAttribute( L"dfl:attr1", L"value1" );
		writer->WriteEndElement();
	}
	writer->WriteEndElement();
	writer->WriteEndElement();

	writer->WriteEndElement();

    writer->WriteStartElement( L"attsourceonly" );
    // attribute with namespace not in destination doc
	if ( wcscmp(changeNamespace,L"dest") == 0 )
		writer->WriteAttribute( L"xmlns:srconly", L"http://www.sourceonly.com" );
    writer->WriteAttribute( L"srconly:attr1", L"sourceonly" );
    // namespace is automatically added to current element in destination doc.
    writer->WriteEndElement();

    writer->WriteEndElement();
}

// Extracts CopTag elements, thus testing FdoXmlCopyHandler
XmlTest::Extractor::Extractor( FdoXmlWriter* writer )
{
	mWriter = FDO_SAFE_ADDREF( writer );
}

FdoXmlSaxHandler* XmlTest::Extractor::XmlStartElement( 
	FdoXmlSaxContext* saxContext,
	FdoString* uri, 
	FdoString* localName, 
	FdoString* qName, 
	FdoXmlAttributeCollection* atts 
)
{
   // return NULL ( keep this object as the SAX Handler ) by default.
   FdoXmlSaxHandler* ret = NULL;

	// If the current element is an fds:CopyTag
   if ( (FdoStringP(uri) == L"http://fdo.osgeo.org/schemas/fds") &&
         (FdoStringP(localName) == L"CopyTag")
   ) {

	    // Todo. Most explicitly free the old copy handler first to ensure
	    // that the previous CopyTag is ended before this one is written. 
	    // Changes to FdoXmlReader (to dereference popped SAX Handlers ) 
	    // are required before this workaround can be removed.
	    mCopyHandler = NULL;

		// Create a copy handler that writes the CopyTag and its sub-elements
        // to the destination doc.
        mCopyHandler = FdoXmlCopyHandler::Create( mWriter, uri, localName, qName, atts );
		
		// Declare any namespaces needed in the output doc. These are 
		// placed on the CopyTag.
		mWriter->WriteAttribute( L"xmlns:ctn", L"http://www.ctn.com" );
		mWriter->WriteAttribute( L"xmlns:dest", L"http://www.changenamespace.com" );

        // Activate it by making it the new SAX handler. When the reader is finished parsing
        // the spatial context, this SCExtractor will be automatically re-instated as the 
        // SAX Handler. 
        ret = (FdoXmlCopyHandler*) mCopyHandler;
	}

   return ret;
}

// Nester tries a re-entrant parse.
XmlTest::Nester::Nester( FdoXmlReader* reader )
{
	mReader = FDO_SAFE_ADDREF( reader );
}

FdoXmlSaxHandler* XmlTest::Nester::XmlStartElement( 
	FdoXmlSaxContext* saxContext,
	FdoString* uri, 
	FdoString* localName, 
	FdoString* qName, 
	FdoXmlAttributeCollection* atts 
)
{
    // Try re-entrant call to Parse.
    mReader->Parse();

   return NULL;
}

// Simple SAX Handler that just counts the elements.
XmlTest::Counter::Counter() :
    mCount(0)
{
}

FdoXmlSaxHandler* XmlTest::Counter::XmlStartElement( 
	FdoXmlSaxContext* saxContext,
	FdoString* uri, 
	FdoString* localName, 
	FdoString* qName, 
	FdoXmlAttributeCollection* atts 
)
{
    mCount++;

    return NULL;
}

