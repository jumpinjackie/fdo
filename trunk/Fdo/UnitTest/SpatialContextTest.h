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

#ifdef _WIN32
#pragma once
#endif

#ifndef CPP_UNIT_SPATIALCONTEXTTEST_H
#define CPP_UNIT_SPATIALCONTEXTTEST_H

// Test cases for the Schema classes.
class SpatialContextTest : public CppUnit::TestCase
{
    FDO_CPPUNIT_DEFINE(testXml);
    FDO_CPPUNIT_DEFINE(testXmlV2);
    FDO_CPPUNIT_DEFINE(testXmlError);
    FDO_CPPUNIT_DEFINE(testXmlNesting);

    CPPUNIT_TEST_SUITE(SpatialContextTest);

    // concrete classes
    CPPUNIT_TEST(testXml);
    CPPUNIT_TEST(testXmlV2);
    CPPUNIT_TEST(testXmlError);
    CPPUNIT_TEST(testXmlNesting);

    CPPUNIT_TEST_SUITE_END();

public:
    SpatialContextTest(void);
    virtual ~SpatialContextTest(void);
    void setUp ();

protected:
    // Test serializing spatial contexts to and from XML
    void testXml();

    // Test deserializing FDO Version 2 spatial context
    void testXmlV2();

    // Test the handling of XML documents with errors.
    void testXmlError();

    // Test the reading of Spatial Contexts that are 2 levels down from the 
    // root element.
    void testXmlNesting();

    // Convert extents from doubles to byte array
    FdoByteArray* SerializeExtent( double minX, double minY, double maxX, double maxY );
    // Deserialize spatial contexts from inStream and serialize them to outStream
    void XmlCopy( FdoIoStream* inStream, FdoIoStream* outStream );
    // Run a deserialization test at the given error level.
    void XmlErrorLevel( char* masterFile, char* errFile, FdoXmlFlags::ErrorLevel errorLevel );

private:

    // The NestReader tests the reading of Spatial Contexts that are in an arbitrary
    // location in an XML document. By default, the Spatial Contexts must be direct
    // sub-elements of the root element. This class finds them under the 2nd Level1
    // sub-element of the root.
    //
    // This class overrides the Spatial Context Reader's SAX event handlers to skip
    // elements until the 2nd Level1 element is found. After that, the events are 
    // passed through to the Spatial Context Reader to read all spatial contexts
    // that are children of the 2nd Level1 element.

    class NestedReader : 
        public FdoXmlSpatialContextReader
    {
    public:
        static NestedReader* Create( FdoIoStream* stream )
        {
            return new NestedReader( stream );
        }
    
        // Turn off this event to prevent jump-down into the first sub-element
        // of the 2nd level1 element.
        virtual FdoXmlSaxHandler* XmlStartDocument(FdoXmlSaxContext*)
        {
            return( NULL );
        }

        virtual FdoXmlSaxHandler* XmlStartElement( 
            FdoXmlSaxContext* context, 
            FdoString* uri, 
            FdoString* name, 
            FdoString* qname, 
            FdoXmlAttributeCollection* atts
        );

        virtual FdoBoolean XmlEndElement( 
            FdoXmlSaxContext* context, 
            FdoString* uri, 
            FdoString* name, 
            FdoString* qname
        );


    protected:
        NestedReader();
        NestedReader( FdoIoStream* stream );
        ~NestedReader() {}

    private:
        FdoInt32 mState;
    };
};

#endif

