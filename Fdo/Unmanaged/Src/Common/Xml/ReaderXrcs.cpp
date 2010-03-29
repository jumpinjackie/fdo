// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include <FdoCommon.h>
#include "ReaderXrcs.h"
#include <Common/Xml/UtilXrcs.h>
#include <Common/Semaphore.h>

#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax/SAXParseException.hpp>

#if _XERCES_VERSION >= 20800
#include <xercesc/sax2/Attributes.hpp>
#endif

#include <stdio.h>
#include "../CommonInternal.h"

FdoXmlReaderXrcs::FdoXmlReaderXrcs(FdoIoTextReader* reader) : 
    FdoXmlReader(reader),
    mParsing(false)
{
    // Initialize the Xerces parser.
    mpParser = XERCES_CPP_NAMESPACE::XMLReaderFactory::createXMLReader();
    mpParser->setContentHandler(this);
    mpParser->setErrorHandler( this );

    // Turn off schema handling, which is not used by FDO. 
    // When schema handling is on, Xerces tries look up schemas referenced
    // by xsi:schemaLocation attributes. In some cases this can cause an 
    // exception to be thrown after a long pause.
    mpParser->setFeature( XERCES_CPP_NAMESPACE::XMLUni::fgXercesSchema, false );
    mpParser->setFeature( XERCES_CPP_NAMESPACE::XMLUni::fgXercesLoadExternalDTD, false );
}

FdoXmlReaderXrcs::~FdoXmlReaderXrcs(void)
{
    delete mpParser;
}

FdoXmlReaderXrcs* FdoXmlReaderXrcs::Create(FdoIoTextReader* reader)
{
    FdoXmlUtilXrcs::Initialize();

    return( new FdoXmlReaderXrcs(reader) );
}

FdoBoolean FdoXmlReaderXrcs::Parse( 
    FdoXmlSaxHandler* saxHandler, 
    FdoXmlSaxContext* saxContext,
    FdoBoolean incremental
)
{
    // Parse is not re-entrant
    // The following semaphore will reset mParsing when it 
    // goes out of scope, including cases where an exception is thrown.
    FdoSemaphoreP semaphore = FdoSemaphore::Create( 
        mParsing,
        FdoXmlExceptionP(
            FdoXmlException::Create(
                FdoException::NLSGetMessage(
                    FDO_NLSID(FDO_53_NESTEDXMLPARSEERROR)
                )
            )
        )
    );

    // Nothing to do if at end of document.

    if ( !GetEOD() ) {

        // Reading from a stream at EOF is a common client error.
        // Therefore trap this particular problem when this is
        // the first parse.
        FdoIoStreamP stream = GetStream();
        FdoInt64 streamLen = stream->GetLength();

        if ( (!GetParsed()) && (streamLen >= 0) && (stream->GetIndex() >= streamLen) ) 
            throw FdoXmlException::Create(
                FdoException::NLSGetMessage(
                    FDO_NLSID(FDO_54_XMLINPUTEOF)
                )
            );

        if ( saxHandler ) 
            PushSaxHandler(saxHandler);

        if ( saxContext )
            SetSaxContext(saxContext);
        else
            SetSaxContext(FdoXmlSaxContextP(FdoXmlSaxContext::Create(this)));

        try {
            if ( incremental ) {
                SetStopParse(false);

                // For incremental parse, keep going until told to stop.
                while ( !GetStopParse() ) 
                {
                    if ( GetParsed() ) {
                        // This is not the first parse
                        mpParser->parseNext( mParseToken );
                    }
                    else {
                        // This is the first parse
                        mpParser->parseFirst( *((const XERCES_CPP_NAMESPACE::InputSource*) this), mParseToken );
                        SetParsed();
                    }
                }
            }
            else {
                // Not incremental, parse the whole document.
                mpParser->parse(*((const XERCES_CPP_NAMESPACE::InputSource*) this));
            }
        }
        catch ( const XERCES_CPP_NAMESPACE::SAXParseException& ex ) {
            if ( saxHandler ) 
                PopSaxHandler();
            SetSaxContext(NULL);

            throw FdoXmlException::Create(
                FdoException::NLSGetMessage(
                    FDO_NLSID(FDO_33_XMLPARSEERROR),
                    ex.getLineNumber(), 
                    ex.getColumnNumber(), 
                    (FdoString*) FdoXmlUtilXrcs::Xrcs2Unicode(ex.getMessage())
                )
            );
        }
        catch ( ... ) {
            if ( saxHandler ) 
                PopSaxHandler();
            SetSaxContext(NULL);

            throw;
        }

        if ( saxHandler ) 
            PopSaxHandler();

        SetSaxContext(NULL);
    }

    // return whether there is still more document to parse.
    return( !GetEOD() );
}

FdoStringP FdoXmlReaderXrcs::DecodeName ( FdoStringP name )
{
    // The encoding pattern is delimited by '-' so break string up by '-'.

    FdoStringsP tokens = FdoStringCollection::Create( name, L"-", true );
    FdoInt32 i;
    FdoStringP outName;
    FdoCharacter xChar = 0;
    FdoBoolean prevDecode = true;

    // Check each token for the encoding pattern.
    for ( i = 0; i < tokens->GetCount(); i++ ) {
        FdoStringP token = FdoStringP(tokens->GetString(i));

        // For the pattern, tack on a character to match the whole token to the pattern.
        FdoStringP pattern = token + L"x";

        xChar = 0;

        // The following does 2 things:
        //    checks if the token matches the encoding pattern (a hex number).
        //    if it does, convert the hex pattern to a wide character.
        if ( (!prevDecode) && (swscanf( (const FdoString*) pattern, L"x%xx", &xChar ) > 0) && (xChar > 0) ) {
            // it matches, append the converted character to the output string.
            outName += FdoStringP::Format( L"%c", xChar );
            prevDecode = true;
        }
        else {
            // It doesn't so ...

            // Make sure we don't lose '-' that aren't pattern delimiters.
            if ( i > 0 && !prevDecode ) 
                outName += L"-";

            // append the whole token to the output string.
            outName += token;
            prevDecode = false;
        }
    }

    outName = outName.Replace( L".", L"-dot-" );
    outName = outName.Replace( L":", L"-colon-" );

    return(outName);
}

void FdoXmlReaderXrcs::startDocument ()
{
    HandleStartDocument();
}

void  FdoXmlReaderXrcs::endDocument ()
{
    HandleEndDocument();
}

void  FdoXmlReaderXrcs::startElement (
    const XMLCh *const uri, 
    const XMLCh *const name, 
    const XMLCh *const qname, 
    const XERCES_CPP_NAMESPACE::Attributes &attrs
)
{
    // Reuse current attribute collection if exists and no one else references it.
    // Saves some memory allocations.
    if ( mFdoAttrs && (mFdoAttrs->GetRefCount() == 1) ) {
        mFdoAttrs->Clear();
    }
    else {
        mFdoAttrs = FdoXmlAttributeCollection::Create();
    }

    XMLSize_t i;
    XMLSize_t attrCount = attrs.getLength();

    // Convert each Xerces XML attribute to a FDO XML attribute.
    for ( i = 0; i < attrCount; i++ ) {
#ifdef _WIN32
        // on Windows, XMLCh same as FdoString so assign directly to save memory allocations
        FdoStringP uri((FdoString*) attrs.getURI(i));
        FdoStringP localName((FdoString*) attrs.getLocalName(i));
        FdoStringP qName((FdoString*)attrs.getQName(i));
#else
        FdoStringP uri = FdoXmlUtilXrcs::Xrcs2Unicode(attrs.getURI(i));
        FdoStringP localName = FdoXmlUtilXrcs::Xrcs2Unicode(attrs.getLocalName(i));
        FdoStringP qName = FdoXmlUtilXrcs::Xrcs2Unicode(attrs.getQName(i));
#endif
        FdoStringP prefix;

        if ( qName.Contains(L":") )
            prefix = qName.Left(L":");

        // Generate unique name for attribute. For qualified attribute name, concatenate
        // the namespace uri and local name. If name unqualified, use local name as is. 
        FdoStringP uniqueName = prefix.GetLength() > 0 ?
                                    uri + L":" + localName :
                                    localName;

#ifdef _WIN32
        // on Windows, XMLCh same as FdoString so assign directly to save memory allocations
        FdoStringP value( (FdoString*)(attrs.getValue(i)) );
#else
        FdoStringP value = FdoXmlUtilXrcs::Xrcs2Unicode(attrs.getValue(i));
#endif

        FdoStringP valueUri;
        FdoStringP valuePrefix;
        FdoStringP localValue = value;
        FdoStringP leftValue;
        FdoStringP rightValue;

        if ( value.Contains(L":") ) {
            leftValue = value.Left(L":");
            rightValue = value.Right(L":");

            if ( !rightValue.Contains(L":") ) {
                // Attribute value qualified by namespace prefix.

                // Set the prefix and unqualified value.
                if ( leftValue.GetLength() > 0 ) {
                    valueUri = PrefixToUri(leftValue);
                    valuePrefix = leftValue;
                    localValue = rightValue;
                }
            }
        }

        if ( valuePrefix == L"" ) {
            // Attribute not qualified, set the uri for the 
            // current default namespace.
            valueUri = PrefixToUri( L"" );
        }

        mFdoAttrs->Add( FdoXmlAttributeP( FdoCommonInternal::CreateXmlAttribute( 
            uniqueName, 
            value,
            localName,
            uri,
            prefix,
            valueUri,
            localValue,
            valuePrefix
        )));

    }
 
    // Call our start element handler. Convert text from Xrcs to FDO Unicode format.
    // Xrcs wide characters are always 2 bytes. FDO wide characters vary according
    // to operating system ( 2 bytes on Windows, 4 on Linux ).
    HandleStartElement( 
#ifdef _WIN32
        (FdoString*) uri, 
        (FdoString*) name, 
        (FdoString*) qname, 
#else
        FdoXmlUtilXrcs::Xrcs2Unicode(uri), 
        FdoXmlUtilXrcs::Xrcs2Unicode(name),
        FdoXmlUtilXrcs::Xrcs2Unicode(qname),
#endif
        mFdoAttrs 
    );
}

void  FdoXmlReaderXrcs::endElement (
    const XMLCh *const uri, 
    const XMLCh *const name, 
    const XMLCh *const qname
)
{
    // Call our end element handler.
    HandleEndElement( 
#ifdef _WIN32
        // on Windows, XMLCh same as FdoString so assign directly to save memory allocations
        (FdoString*) uri, 
        (FdoString*) name, 
        (FdoString*) qname 
#else
        FdoXmlUtilXrcs::Xrcs2Unicode(uri), 
        FdoXmlUtilXrcs::Xrcs2Unicode(name), 
        FdoXmlUtilXrcs::Xrcs2Unicode(qname) 
#endif
    );

}

void  FdoXmlReaderXrcs::startPrefixMapping (
    const XMLCh *const prefix, 
    const XMLCh *const uri
)
{

    HandleStartPrefixMapping( 
#ifdef _WIN32
        // on Windows, XMLCh same as FdoString so assign directly to save memory allocations
        (FdoString*) prefix, 
        (FdoString*) uri
#else
        FdoXmlUtilXrcs::Xrcs2Unicode(prefix), 
        FdoXmlUtilXrcs::Xrcs2Unicode(uri)
#endif
    );
}

void  FdoXmlReaderXrcs::endPrefixMapping (
    const XMLCh *const prefix 
)
{

    HandleEndPrefixMapping( 
#ifdef _WIN32
        // on Windows, XMLCh same as FdoString so assign directly to save memory allocations
        (FdoString*) prefix
#else
        FdoXmlUtilXrcs::Xrcs2Unicode(prefix)
#endif
    );
}

void  FdoXmlReaderXrcs::characters (const XMLCh *const chars, const XMLSize_t length)
{

    // Call our element content handler.
    HandleCharacters( FdoXmlUtilXrcs::Xrcs2Unicode(chars,length) );
}

XERCES_CPP_NAMESPACE::BinInputStream* FdoXmlReaderXrcs::makeStream ()  const
{
    return new InputStream( FdoIoStreamP(((FdoXmlReaderXrcs*)this)->GetStream()) );
}

FdoXmlReaderXrcs::InputStream::InputStream( FdoIoStream* stream ) 
{
    stream->AddRef();
    mStream = stream;
}

XMLFilePos FdoXmlReaderXrcs::InputStream::curPos()  const
{
    return( (XMLFilePos) mStream->GetIndex() );
}

XMLSize_t FdoXmlReaderXrcs::InputStream::readBytes( 
    XMLByte *const  toFill,  
    const XMLSize_t  maxToRead 
)
{
    return (XMLSize_t)(( mStream->Read( toFill, maxToRead ) ));
}



