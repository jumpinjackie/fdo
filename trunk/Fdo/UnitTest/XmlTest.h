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


#ifndef CPP_UNIT_XMLTEST_H
#define CPP_UNIT_XMLTEST_H


// Test cases for the Schema classes.
class XmlTest : public CppUnit::TestCase
{
    FDO_CPPUNIT_DEFINE(testXsl);
 
    FDO_CPPUNIT_DEFINE(testNameAdjust);
    FDO_CPPUNIT_DEFINE(testRead);
    FDO_CPPUNIT_DEFINE(testWrite);
    FDO_CPPUNIT_DEFINE(testWriteClose);
    FDO_CPPUNIT_DEFINE(testReadWrite);
    FDO_CPPUNIT_DEFINE(testCopy);

    CPPUNIT_TEST_SUITE(XmlTest);

    CPPUNIT_TEST(testXsl);
    CPPUNIT_TEST(testNameAdjust);
    CPPUNIT_TEST(testRead);
    CPPUNIT_TEST(testReadWrite);
    CPPUNIT_TEST(testWrite);
    CPPUNIT_TEST(testWriteClose);
    CPPUNIT_TEST(testCopy);

    CPPUNIT_TEST_SUITE_END();

public:
    XmlTest(void);
    virtual ~XmlTest(void);
    void setUp ();

protected:
    void testXsl();
    void testNameAdjust();
    void testRead();
    void testWrite();
    void testWriteClose();
    void testReadWrite();
    void testCopy();

    void WriteAfterClose( FdoXmlWriter* writer );
	void AddCopyWrapper( FdoXmlWriter* writer, FdoInt32 idx, FdoInt32 wrapperLevel, FdoString* changeNamespace = L"" );
	void AddCopyPart( FdoXmlWriter* writer, FdoInt32 idx, FdoString* changeNamespace );

private:

	class Extractor : 
		public FdoDisposable,
		public FdoXmlSaxHandler
	{
	public:
		static Extractor* Create( FdoXmlWriter* writer )
		{
			return new Extractor( writer );
		}

	protected:
		Extractor() {}

		Extractor( FdoXmlWriter* writer );

		~Extractor() {}

 		virtual FdoXmlSaxHandler* XmlStartElement( 
			FdoXmlSaxContext* saxContext,
			FdoString* uri, 
			FdoString* localName, 
			FdoString* qName, 
			FdoXmlAttributeCollection* atts 
		);

		FdoXmlWriterP mWriter;
		FdoXmlCopyHandlerP mCopyHandler;
	};

    typedef FdoPtr<Extractor>ExtractorP;

	class Nester : 
		public FdoDisposable,
		public FdoXmlSaxHandler
	{
	public:
		static Nester* Create( FdoXmlReader* reader )
		{
			return new Nester( reader );
		}

	protected:
		Nester() {}

		Nester( FdoXmlReader* writer );

		~Nester() {}

 		virtual FdoXmlSaxHandler* XmlStartElement( 
			FdoXmlSaxContext* saxContext,
			FdoString* uri, 
			FdoString* localName, 
			FdoString* qName, 
			FdoXmlAttributeCollection* atts 
		);

		FdoXmlReaderP mReader;
	};

    typedef FdoPtr<Nester> NesterP;

	class Counter : 
		public FdoDisposable,
		public FdoXmlSaxHandler
	{
	public:
		static Counter* Create()
		{
			return new Counter();
		}

        long GetCount()
        {
            return mCount;
        }

	protected:
		Counter();
		~Counter() {}

 		virtual FdoXmlSaxHandler* XmlStartElement( 
			FdoXmlSaxContext* saxContext,
			FdoString* uri, 
			FdoString* localName, 
			FdoString* qName, 
			FdoXmlAttributeCollection* atts 
		);

		long mCount;
	};

    typedef FdoPtr<Counter> CounterP;

    // A stream that does not keep track of its length.
    // Use to test that XML parsing from such streams does not
    // generate false end-of-file exceptions.
    class StringStream : public FdoIoStream
    {
    public:
        StringStream( char* contents )
        {
            mContents = contents;
            mIndex = 0;
        }

        virtual FdoSize Read( FdoByte* buffer, FdoSize count )
        {
            if ( mContents[mIndex] == 0 )
                return 0;

            buffer[0] = mContents[mIndex++];
            return 1;
        }

	    virtual void Write( FdoByte* buffer, FdoSize count )
        {
        }

	    virtual void Write( FdoIoStream* stream, FdoSize count = 0 )
        {
        }

	    virtual void SetLength( FdoInt64 )
        {
        }

        virtual FdoInt64 GetLength()
        {
            return -1;
        }

        virtual FdoInt64 GetIndex()
        {
            return 0;
        }

	    virtual void Skip( FdoInt64 offset )
        {
        }

	    virtual void Reset()
        {
        }

	    virtual FdoBoolean CanWrite()
        {
            return false;
        }
	    virtual FdoBoolean HasContext()
        {
            return false;
        }

    private:
        char* mContents;
        int mIndex;
    };

    typedef FdoPtr<StringStream> StringStreamP;
};

#endif

