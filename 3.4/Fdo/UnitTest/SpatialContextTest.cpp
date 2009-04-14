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
#include <Fdo/Xml/SpatialContextReader.h>
#include <Fdo/Xml/SpatialContextWriter.h>
#include "SpatialContextTest.h"
#include "UnitTestUtil.h"
#include "XmlFormatter.h"
#ifdef _DEBUG
//#define DEBUG_DETAIL  1
#endif

#ifndef FALSE
#define FALSE   1
#endif

CPPUNIT_TEST_SUITE_REGISTRATION(SpatialContextTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (SpatialContextTest, "SpatialContextTest");

SpatialContextTest::SpatialContextTest(void)
{
}

SpatialContextTest::~SpatialContextTest(void)
{
}

void SpatialContextTest::setUp()
{
    // nothing to do at setup time... yet
}

void SpatialContextTest::testXml()
{
    try {  
        FdoIoMemoryStreamP stream = FdoIoMemoryStream::Create();
        FdoIoMemoryStreamP stream2 = FdoIoMemoryStream::Create();
        FdoXmlWriterP xmlWriter = FdoXmlWriter::Create(stream);

        FdoXmlSpatialContextWriterP writer = FdoXmlSpatialContextWriter::Create( 
            xmlWriter
        );

        // Try to write nameless spatial context (should fail)
        FdoBoolean bFailed = false;
        try {
           writer->WriteSpatialContext();
        }
        catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Current spatial context has no name; cannot write to XML. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Current spatial context has no name; cannot write to XML. ") == 0 );
#endif
#endif
            e->Release ();
            bFailed = true;
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing nameless spatial context should have failed." );

        writer->SetName( L"SC_0" );


        writer->SetCoordinateSystem( L"Quebec MTM Zone 8 (NAD83)" );

        // Try to write extentless spatial context (should fail)
        bFailed = false;
        try {
           writer->WriteSpatialContext();
        }
        catch ( FdoException* e ) {
//Todo: get this working for Linux (likely need to set up message catalogue
#ifdef _WIN32
// The exception message has no file information in release mode.That means there is no ')' in release mode . 
#ifdef _DEBUG
            FdoString* pMessage = wcschr( e->GetExceptionMessage(), ')' ) + 2;
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Current spatial context 'SC_0' has no extents; cannot write to XML. ") == 0 );
#else
			FdoString* pMessage = e->GetExceptionMessage();
            FDO_CPPUNIT_ASSERT( wcscmp(pMessage, L"Current spatial context 'SC_0' has no extents; cannot write to XML. ") == 0 );
#endif
#endif
            e->Release ();
            bFailed = true;
        }
        if ( !bFailed ) 
            CPPUNIT_FAIL( "Writing extentless spatial context should have failed." );

        // now for the happy tests.

        FdoByteArray* extent = SerializeExtent( 100000, 1000000, 200000, 1100000 );
        writer->SetExtent( extent );
        extent->Release();

        // write spatial context with no optional attributes set.
        writer->WriteSpatialContext();

        // write spatial contexts with optional attributes set.
        writer->SetName( L"Subway Map" );
        writer->SetDescription( L"Diagram of subway stops" );
        writer->SetCoordinateSystem( L"Non-Earth (Meter)" );
        writer->SetCoordinateSystemWkt( L"LOCAL_CS [ \"Non-Earth (Meter)\", LOCAL_DATUM [\"Local Datum\", 0], UNIT [\"Meter\", 1.0], AXIS [\"X\", EAST], AXIS[\"Y\", NORTH]]" );
        extent = SerializeExtent( 0, 0, 15000, 10000 );
        writer->SetExtent( extent );
        extent->Release();
        writer->SetExtentType( FdoSpatialContextExtentType_Dynamic );
        writer->SetXYTolerance( 0.1 );
        writer->SetZTolerance( 0.01 );

        writer->WriteSpatialContext();
        
        writer = FdoXmlSpatialContextWriter::Create( 
            xmlWriter
        );
        // write spatial context with no coordinate system set.
        writer->SetName( L"Default Rectangular" );
        extent = SerializeExtent( 0, 0, 1000, 1000 );
        writer->SetExtent( extent );
        extent->Release();

        writer->WriteSpatialContext();
        
        writer = NULL;
        xmlWriter = NULL;

        stream->Reset();
        stream2->Reset();

        // Round trip the spatial contexts that were written to a 2nd XML document.
        // This tests whether we lose anything when deserializing and serializing.
        XmlCopy( stream, stream2 );

        // Format output files for comparison. Adds carriage returns.
        stream->Reset();
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create( L"sc1a.xml", L"wt+");
        XmlFormatter formatter1a(stream, fileStream);
        formatter1a.Format();

        stream2->Reset();
        fileStream = FdoIoFileStream::Create( L"sc2a.xml", L"wt+");
        XmlFormatter formatter2a(stream2, fileStream);
        formatter2a.Format();

        stream->SetLength(0);
        stream2->SetLength(0);

        // This next test writes an XML document with a single spatial context.

        writer = FdoXmlSpatialContextWriter::Create( 
            FdoXmlWriterP( FdoXmlWriter::Create(stream) ),
            FdoXmlSpatialContextFlagsP( FdoXmlSpatialContextFlags::Create( L"www.ttc.ca" ) )
        );

        writer->SetName( L"Subway Map" );
        writer->SetDescription( L"Diagram of subway stops" );
        writer->SetCoordinateSystem( L"Non-Earth (Meter)" );
        extent = SerializeExtent( 0, 0, 15000, 10000 );
        writer->SetExtent( extent );
        extent->Release();
        writer->SetExtentType( FdoSpatialContextExtentType_Dynamic );
        writer->SetXYTolerance( 0.1 );
        writer->SetZTolerance( 0.01 );

        writer->WriteSpatialContext();

        writer = NULL;

        stream->Reset();
        stream2->Reset();

        // Test round tripping for second document
        XmlCopy( stream, stream2 );

        // Format output files for comparison. Adds carriage returns.
        stream->Reset();
        fileStream = FdoIoFileStream::Create( L"sc1b.xml", L"wt+");
        XmlFormatter formatter1b(stream, fileStream);
        formatter1b.Format();

        stream2->Reset();
        fileStream = FdoIoFileStream::Create( L"sc2b.xml", L"wt+");
        XmlFormatter formatter2b(stream2, fileStream);
        formatter2b.Format();

//Todo: get this working on linux
#ifdef _WIN32
        // Compare output and expected results.
        UnitTestUtil::CheckOutput( "sc1a_master.txt", "sc1a.xml" );
        UnitTestUtil::CheckOutput( "sc1b_master.txt", "sc1b.xml" );
        UnitTestUtil::CheckOutput( "sc2a_master.txt", "sc2a.xml" );
        UnitTestUtil::CheckOutput( "sc2b_master.txt", "sc2b.xml" );
#endif

    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void SpatialContextTest::testXmlV2()
{
    try {  
        FdoIoFileStreamP stream = FdoIoFileStream::Create( L"sc_v2.xml", L"r");
        FdoIoMemoryStreamP stream2 = FdoIoMemoryStream::Create();

        // Round trip the spatial contexts that were written to a 2nd XML document.
        // This tests whether we lose anything when deserializing and serializing.
        XmlCopy( stream, stream2 );

        // Format output file for comparison. Adds carriage returns.
        stream2->Reset();
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create( L"sc_v2_out.xml", L"wt+");
        XmlFormatter formatter2a(stream2, fileStream);
        formatter2a.Format();

//Todo: get this working on linux
#ifdef _WIN32
        // Compare output and expected results.
        UnitTestUtil::CheckOutput( "sc2a_master.txt", "sc_v2_out.xml" );
#endif

    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void SpatialContextTest::testXmlError()
{
    try { 
        // Read bad XML document under strict error checking
        XmlErrorLevel( "sc_err1a_master.txt", "sc_err1a.txt", FdoXmlFlags::ErrorLevel_High );
        // Retry with lax error checking, should get fewer errors
        XmlErrorLevel( "sc_err1b_master.txt", "sc_err1b.txt", FdoXmlFlags::ErrorLevel_Normal );
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void SpatialContextTest::testXmlNesting()
{
    try {
        // Create an XML document with Spatial Contexts nested 2 levels down from
        // root element.

        FdoIoMemoryStreamP stream = FdoIoMemoryStream::Create();
        FdoXmlWriterP writer = FdoXmlWriter::Create( stream );

        // Create 2 Level1 elements under the root element.

        writer->WriteStartElement( L"Level1" );
        writer->WriteStartElement( L"Level2" );
        writer->WriteEndElement();
        writer->WriteStartElement( L"Level2" );
        writer->WriteEndElement();
        writer->WriteEndElement();

        // The Spatial Contexts are under the second Level1 element.

        writer->WriteStartElement( L"Level1" );

        // Mix them up with some other elements that should be skipped.
        writer->WriteStartElement( L"Level2" );
        writer->WriteEndElement();

        FdoXmlSpatialContextWriterP SCWriter =
            FdoXmlSpatialContextWriter::Create( writer );
        SCWriter->SetName( L"Embedded1" );
        FdoByteArray* extent = SerializeExtent( 0, 0, 15000, 10000 );
        SCWriter->SetExtent( extent );
        extent->Release();
        SCWriter->WriteSpatialContext();
        SCWriter = NULL;

        writer->WriteStartElement( L"Level2" );
        writer->WriteEndElement();

        SCWriter =
            FdoXmlSpatialContextWriter::Create( writer );
        SCWriter->SetName( L"Embedded2" );
        extent = SerializeExtent( 0, 0, 15000, 10000 );
        SCWriter->SetExtent( extent );
        extent->Release();
        SCWriter->WriteSpatialContext();
        SCWriter = NULL;

        writer->WriteStartElement( L"Level2" );
        writer->WriteEndElement();
        writer = NULL;

        stream->Reset();

        FdoIoFileStreamP fileStream = FdoIoFileStream::Create( L"sc_nested.xml", L"w+");
        XmlFormatter formatter1(stream, fileStream);
        formatter1.Format();
        stream->Reset();

        FdoPtr<NestedReader> reader = NestedReader::Create(stream);

        // Read the spatial contexts. Should get both of them
        reader->ReadNext();
        FDO_CPPUNIT_ASSERT( wcscmp(reader->GetName(), L"Embedded1") == 0 );

        reader->ReadNext();
        FDO_CPPUNIT_ASSERT( wcscmp(reader->GetName(), L"Embedded2") == 0 );

        // Shouldn't be any more spatial contexts at this point.
        FDO_CPPUNIT_ASSERT( reader->ReadNext() == false );
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void SpatialContextTest::testXmlDotColon()
{
    try {
        FdoInt32 pass;

        // pass 0 writes SC without WKT, pass1 writes it with WKT.
        for ( pass = 0; pass < 2; pass++ ) {
            FdoIoMemoryStreamP stream = FdoIoMemoryStream::Create();
            FdoIoMemoryStreamP stream2 = FdoIoMemoryStream::Create();
            FdoXmlWriterP xmlWriter = FdoXmlWriter::Create(stream);

            FdoXmlSpatialContextWriterP writer = FdoXmlSpatialContextWriter::Create( 
                xmlWriter
            );

            writer->SetName( L"EPSG:123.4" );
            writer->SetDescription( L". and : test" );
            writer->SetCoordinateSystem( L"EPSG:1234" );
            if ( pass == 1 ) 
                writer->SetCoordinateSystemWkt( L"LOCAL_CS [ \"Non-Earth (Meter)\", LOCAL_DATUM [\"Local Datum\", 0], UNIT [\"Meter\", 1.0], AXIS [\"X\", EAST], AXIS[\"Y\", NORTH]]" );
            FdoPtr<FdoByteArray> extent = SerializeExtent( 0, 0, 15000, 10000 );
            writer->SetExtent( extent );
            writer->SetExtentType( FdoSpatialContextExtentType_Dynamic );
            writer->SetXYTolerance( 0.1 );
            writer->SetZTolerance( 0.01 );

            writer->WriteSpatialContext();

            writer = NULL;
            xmlWriter = NULL;
            stream->Reset();

            FdoXmlSpatialContextReaderP reader =
                FdoXmlSpatialContextReader::Create(
                    FdoXmlReaderP( FdoXmlReader::Create(stream) ),
                    FdoXmlSpatialContextFlagsP(
                        FdoXmlSpatialContextFlags::Create( NULL, FdoXmlFlags::ErrorLevel_High ) 
                    )
                );

            FDO_CPPUNIT_ASSERT( reader->ReadNext() );
            FDO_CPPUNIT_ASSERT( wcscmp(reader->GetName(), L"EPSG:123.4") == 0 );
            FDO_CPPUNIT_ASSERT( wcscmp(reader->GetCoordinateSystem(), L"EPSG:1234") == 0 );
            FDO_CPPUNIT_ASSERT( wcscmp(reader->GetDescription(), L". and : test") == 0 );
         
            FDO_CPPUNIT_ASSERT( !reader->ReadNext() );
        }
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

FdoByteArray* SpatialContextTest::SerializeExtent( double minX, double minY, double maxX, double maxY )
{
    // Create a byte array 
  	FdoPtr<FdoFgfGeometryFactory>  gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIEnvelope>            env = gf->CreateEnvelopeXY(minX, minY, maxX, maxY );  
    FdoPtr<FdoIGeometry>            geom = gf->CreateGeometry(env); 

    return( gf->GetFgf(geom) );
}

void SpatialContextTest::XmlCopy( FdoIoStream* inStream, FdoIoStream* outStream )
{
    FdoXmlSpatialContextReaderP reader =
        FdoXmlSpatialContextReader::Create(
            FdoXmlReaderP( FdoXmlReader::Create(inStream) ),
            FdoXmlSpatialContextFlagsP(
                FdoXmlSpatialContextFlags::Create( NULL, FdoXmlFlags::ErrorLevel_High ) 
            )
        );

    FdoXmlSpatialContextWriterP writer =
        FdoXmlSpatialContextWriter::Create(
            FdoXmlWriterP( FdoXmlWriter::Create(outStream) ),
            FdoXmlSpatialContextFlagsP(
                FdoXmlSpatialContextFlags::Create( L"www.nexturl.com", FdoXmlFlags::ErrorLevel_High ) 
            )
        );

    while ( reader->ReadNext() ) {
        writer->SetName( reader->GetName() );
        writer->SetDescription( reader->GetDescription() );
        writer->SetCoordinateSystem( reader->GetCoordinateSystem() );
        writer->SetCoordinateSystemWkt( reader->GetCoordinateSystemWkt() );
        writer->SetExtentType( reader->GetExtentType() );
        FdoByteArray* extent = reader->GetExtent();
        writer->SetExtent( extent );
        extent->Release();
        writer->SetXYTolerance( reader->GetXYTolerance() );
        writer->SetZTolerance( reader->GetZTolerance() );

        writer->WriteSpatialContext();
    }
}

void SpatialContextTest::XmlErrorLevel( char* masterFile, char* errFile, FdoXmlFlags::ErrorLevel errorLevel )
{
    FdoStringP wErrFile(errFile);

    FdoIoFileStreamP errStream = FdoIoFileStream::Create( wErrFile, L"w+t" );

    FdoXmlSpatialContextReaderP reader =
        FdoXmlSpatialContextReader::Create(
            FdoXmlReaderP( FdoXmlReader::Create(L"sc_error.xml") ),
            FdoXmlSpatialContextFlagsP(
                FdoXmlSpatialContextFlags::Create( NULL, errorLevel ) 
            )
        );

    int i;
    FdoBoolean more;

    for ( more = true, i = 0; more && (i <100); i++) {
        try {
            more = reader->ReadNext();
        }
        catch ( FdoException* e ) {
            UnitTestUtil::PrintException( e, errStream, true );
            e->Release ();
        }
    }

//Todo: get this working on linux
#ifdef _WIN32
    // Compare output and expected results.
    UnitTestUtil::CheckOutput( masterFile, errFile );
#endif

}

// not used:
SpatialContextTest::NestedReader::NestedReader() :
    FdoXmlSpatialContextReader( NULL, NULL),
    mState(0)
{
}

SpatialContextTest::NestedReader::NestedReader( FdoIoStream *stream ) :
    FdoXmlSpatialContextReader( FdoPtr<FdoXmlReader>(FdoXmlReader::Create(stream)), NULL),
    mState(0)
{
}

FdoXmlSaxHandler* SpatialContextTest::NestedReader::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts
)
{
    FdoXmlSaxHandler* ret = NULL;

    switch ( mState ) {
    case 0:
        // In state 0 we haven't yet read the 1st Level1 element.
        if ( wcscmp( name, L"Level1" ) == 0 ) 
            // Now we have so move on to state 1
            mState = 1;
        break;

    case 1:
        // In state 0 we haven't yet read the 2nd Level1 element.
        if ( wcscmp( name, L"Level1" ) == 0 )
            // Now we have so move on to state 2
            mState = 2;
        break;

    case 2:
        // The reading of the spatial contexts starts in state 2.
        ret = FdoXmlSpatialContextReader::XmlStartElement( context, uri, name, qname, atts );
    }

    return(ret);
}

FdoBoolean SpatialContextTest::NestedReader::XmlEndElement( 
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname
)
{
    if ( mState == 2 ) 
        // In state 2 so pass event on to Spatial Context Reader
        return( FdoXmlSpatialContextReader::XmlEndElement( context, uri, name, qname ) );
    else
        // Otherwise, nothing to do.
        return( false );
}





