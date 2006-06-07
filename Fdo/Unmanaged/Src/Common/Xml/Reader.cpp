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
#include <stdio.h>
#include "ReaderXrcs.h"

FdoXmlReader::FdoXmlReader(FdoIoTextReader* reader)
{
    reader->AddRef();
    mTextReader = reader;
    mStopParse = false;
    mParsed = false;
    mEOD = false;

    mHandlerStack = HandlerStack::Create();
    mPrefixes = PrefixMappingCollection::Create();
}

FdoXmlReader::~FdoXmlReader(void)
{
}

FdoXmlReader* FdoXmlReader::Create( FdoString* fileName )
{
    FdoIoTextReaderP pReader = FdoIoTextReader::Create(fileName);

    return( FdoXmlReader::Create(pReader) );
}

FdoXmlReader* FdoXmlReader::Create( FdoIoStream* stream )
{
    FdoIoTextReaderP pReader = FdoIoTextReader::Create(stream);

    return( FdoXmlReader::Create(pReader) );
}

FdoXmlReader* FdoXmlReader::Create( FdoIoTextReader* reader )
{
    if ( !reader ) 
        throw FdoXmlException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"reader",
                L"(NULL)",
                L"FdoXmlReader::Create"
            )
        );

    // Using Xerces as the parser.
    return( FdoXmlReaderXrcs::Create(reader) );
}

FdoIoTextReader* FdoXmlReader::GetTextReader()
{
    FdoIoTextReader* pReader = mTextReader;
    FDO_SAFE_ADDREF(pReader);

    return pReader;
}

FdoIoStream* FdoXmlReader::GetStream()
{
    return mTextReader->GetStream();
}


void FdoXmlReader::PushSaxHandler( FdoXmlSaxHandler* saxHandler )
{
    mHandlerStack->Push( StackElementP(StackElement::Create(saxHandler)) );
}

FdoXmlSaxHandler* FdoXmlReader::PopSaxHandler()
{
    return( StackElementP(mHandlerStack->Pop())->GetHandler() );
}

FdoXmlSaxHandler* FdoXmlReader::GetSaxHandler()
{
    StackElementP elem = mHandlerStack->Peek();
    return( (elem != NULL) ? elem->GetHandler() : NULL );
}

FdoBoolean FdoXmlReader::GetEOD() 
{
    return mEOD;
}

FdoDictionary* FdoXmlReader::GetNamespaces()
{
    FdoDictionary* pDict = FdoDictionary::Create();
    FdoInt32 i;

    for (i = 0; i < mPrefixes->GetCount(); i++ ) {
        PrefixMappingP prefixMapping = mPrefixes->GetItem(i);
        FdoDictionaryElementP ns = FdoDictionaryElement::Create(
            prefixMapping->GetName(),
            prefixMapping->GetUri()
        );

        pDict->Add( ns );
    }

    return pDict;
}

FdoString* FdoXmlReader::PrefixToUri( FdoString* prefix )
{
    FdoString* pUri = NULL;

    PrefixMappingP prefixMapping = mPrefixes->FindItem(prefix);

    if ( prefixMapping )
        pUri = prefixMapping->GetUri();

    return pUri;
}


void FdoXmlReader::HandleStartDocument()
{
    // Get the current SAX handler and delegate to it.
    FdoXmlSaxHandler* pHandler = GetSaxHandler();
    FdoXmlSaxHandler* pNewHandler = NULL;
    
    if ( pHandler )
        pNewHandler = pHandler->XmlStartDocument(mSaxContext);

    if ( pNewHandler )
        // We got a new SAX handler so push it to handle the root element.
        PushSaxHandler(pNewHandler);
    else
        // Otherwise, the current SAX handler remains active.
        PushSaxHandler(pHandler);
}

void FdoXmlReader::HandleEndDocument()
{
    // Pop the root element SAX handler to get back to the document's handler.
    PopSaxHandler();

    // Get and delegate to the document's hander.
    FdoXmlSaxHandler* pHandler = GetSaxHandler();

    if ( pHandler )
        pHandler->XmlEndDocument(mSaxContext);

    mEOD = true;
    SetStopParse(true);
}

void FdoXmlReader::HandleStartElement(FdoString *pUri, FdoString *pElementName, FdoString *pElementQName, FdoXmlAttributeCollection* pAttrs )
{
    // Get the current SAX handler.
    FdoXmlSaxHandler* pHandler = GetSaxHandler();
    FdoXmlSaxHandler* pNewHandler = NULL;
    
    // Delegate to the current SAX handler.
    if ( pHandler )
        pNewHandler = pHandler->XmlStartElement( mSaxContext, pUri, pElementName, pElementQName, pAttrs );

    if ( pNewHandler )
        // SAX Handler returned another handler, push it to handle the element's sub-elements.
        PushSaxHandler(pNewHandler);
    else
        // Otherwise, the current handler remains active.
        PushSaxHandler(pHandler);
}

void FdoXmlReader::HandleEndElement(FdoString *pUri, FdoString *pElementName, FdoString *pElementQName)
{
    // Pop the handler for sub-elements, to get back to the handler for this element.
    PopSaxHandler();

    // Get and delegate to the current SAX Handler.
    FdoXmlSaxHandler* pHandler = GetSaxHandler();

    if ( pHandler )
        SetStopParse( pHandler->XmlEndElement( mSaxContext, pUri, pElementName, pElementQName ) );
}

void FdoXmlReader::HandleStartPrefixMapping(FdoString *pPrefix, FdoString *pUri)
{
    // check if prefix already declared
    PrefixMappingP prefix = mPrefixes->FindItem( pPrefix );

    if ( !prefix ) {
        // not declared so create an entry form it.
        prefix = PrefixMapping::Create( pPrefix );
        mPrefixes->Add( prefix );
    }

    //This declaration is now in scope and supercedes any declarations
    //for this prefix.
    prefix->PushUri( pUri );
}

void FdoXmlReader::HandleEndPrefixMapping( FdoString *pPrefix )
{
    PrefixMappingP prefix = mPrefixes->FindItem( pPrefix );

    // declaration now out of scope so pop it.
    if ( prefix ) 
        prefix->PopUri();
}

void FdoXmlReader::HandleCharacters(FdoString *chars)
{
    FdoXmlSaxHandler* pHandler = GetSaxHandler();

    if ( pHandler )
        pHandler->XmlCharacters(mSaxContext, chars);
}

void FdoXmlReader::SetSaxContext( FdoXmlSaxContext* saxContext )
{
    FDO_SAFE_ADDREF(saxContext);
    mSaxContext = saxContext;
}

FdoBoolean FdoXmlReader::GetStopParse()
{
    return mStopParse;
}

void FdoXmlReader::SetStopParse( FdoBoolean stopParse )
{
    mStopParse = stopParse;
}

FdoBoolean FdoXmlReader::GetParsed()
{
    return mParsed;
}

void FdoXmlReader::SetParsed()
{
    mParsed = true;
}

FdoXmlReader::StackElement::StackElement( FdoXmlSaxHandler* pHandler )
{
    mpHandler = pHandler;
}

FdoXmlReader::StackElement* FdoXmlReader::StackElement::Create( FdoXmlSaxHandler* pHandler )
{
    return new StackElement( pHandler );
}

FdoXmlReader::PrefixMapping::PrefixMapping( FdoString* pPrefix ) :
    mPrefix(pPrefix)
{
    mUriStack = FdoStringCollection::Create();
}

FdoXmlReader::PrefixMapping* FdoXmlReader::PrefixMapping::Create( FdoString* pPrefix )
{
    return new PrefixMapping( pPrefix );
}

FdoString* FdoXmlReader::PrefixMapping::GetName()
{
    return mPrefix;
}

FdoString* FdoXmlReader::PrefixMapping::GetUri()
{
    if ( mUriStack->GetCount() > 0 )
        return mUriStack->GetString( mUriStack->GetCount() - 1 );
    else
        return L"";
}

void FdoXmlReader::PrefixMapping::PushUri( FdoString* pUri )
{
    mUriStack->Add( pUri );
}

void FdoXmlReader::PrefixMapping::PopUri()
{
    if ( mUriStack->GetCount() > 0 )
        mUriStack->RemoveAt( mUriStack->GetCount() - 1 );
}


