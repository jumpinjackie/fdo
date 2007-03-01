#ifndef FDO_XML_READERXRCS_H
#define FDO_XML_READERXRCS_H
// 

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

#include <FdoCommon.h>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax/InputSource.hpp>
#include <xercesc/util/BinInputStream.hpp>

/// \cond DOXYGEN-IGNORE

// Implementation of FdoXmlReader that uses the Xerces XML parser. 
// This class is not directly exposed to the FDO API, only through the FdoXmlReader
// interface.
class FdoXmlReaderXrcs :
    public FdoXmlReader, 
    public XERCES_CPP_NAMESPACE::DefaultHandler,
    public XERCES_CPP_NAMESPACE::InputSource
{
public:
    // Creates an XML Reader on a text reader.
    static FdoXmlReaderXrcs* Create(FdoIoTextReader* reader);

    // Parses the XML document from the text reader.
    virtual FdoBoolean Parse( 
        FdoXmlSaxHandler* saxHandler = NULL, 
        FdoXmlSaxContext* saxContext = NULL,
        FdoBoolean incremental = false
    );

    // Decodes an element name (See FdoXmlReader for details).
    FDO_API_COMMON virtual FdoStringP DecodeName ( FdoStringP name );

protected:

    // Stream adapter. Adapts a FdoIoStream so that the Xerces parser can read 
    // from it.
    class InputStream : public XERCES_CPP_NAMESPACE::BinInputStream
    {
    public:
        // Create a BinInputStream that wraps around a FDO stream.
        InputStream( FdoIoStream* stream ); 
        virtual ~InputStream(void) {}

        // from BinInputStream interface

        virtual unsigned int curPos()  const;

        virtual unsigned int readBytes( 
            XMLByte *const  toFill,  
            const unsigned int  maxToRead 
        );

    private:
        FdoIoStreamP mStream;
    };

    FdoXmlReaderXrcs(FdoIoTextReader* reader);
    virtual ~FdoXmlReaderXrcs();

    // Since this class inherites from Xerces DefaultHandler and InputSource,
    // these operators had to be defined to resolve ambiguity.
    void * operator new (size_t size)
    {
        return malloc(size);
    }

    void * operator new (size_t size, const char*, int)
    {
        return malloc(size);
    }

    void   operator delete (void* ptr)
    {
        free(ptr);
    }

    // SAX2 Parse events from DefaultHandler Interface

    virtual void  startDocument (); 
    virtual void  endDocument ();
    virtual void  startElement (
        const XMLCh *const uri, 
        const XMLCh *const name, 
        const XMLCh *const qname, 
        const XERCES_CPP_NAMESPACE::Attributes &attrs
    );
    virtual void  endElement (
        const XMLCh *const uri, 
        const XMLCh *const name, 
        const XMLCh *const qname 
    );
    virtual void startPrefixMapping (
        const XMLCh *const prefix, 
        const XMLCh *const uri
    );
    virtual void endPrefixMapping (
        const XMLCh *const prefix
    );
    virtual void  characters (const XMLCh *const chars, const unsigned int length);

// The following are part of the SAX2 callback interface but are currently not used
// by FDO.
/*
    virtual void  ignorableWhitespace (const XMLCh *const, const unsigned int) {}
    virtual void  processingInstruction (const XMLCh *const, const XMLCh *const) {}
    virtual void  resetDocument () {}
    virtual void  setDocumentLocator (const XERCES_CPP_NAMESPACE::Locator *const) {}
*/
    // from InputSource interface

    // Callback for getting the stream to read the XML document from.
    // The implementation returns our InputStream adaptor.
    virtual XERCES_CPP_NAMESPACE::BinInputStream* makeStream()  const;

    // Xerces parser.
    XERCES_CPP_NAMESPACE::SAX2XMLReader* mpParser;

    // Token for incremental parsing
    XERCES_CPP_NAMESPACE::XMLPScanToken mParseToken;

    // Semaphore for prevent re-entrant parsing.
    FdoBoolean mParsing;

private:
    FdoXmlAttributesP mFdoAttrs;
};
/// \endcond

#endif

