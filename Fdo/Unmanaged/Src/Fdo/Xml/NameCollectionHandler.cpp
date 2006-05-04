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
#include <Common.h>
#include <Common/Xml/CharDataHandler.h>
#include <Fdo/Xml/Context.h>
#include <Fdo/Xml/NameCollectionHandler.h>


FdoXmlNameCollectionHandler::FdoXmlNameCollectionHandler()
{
}

FdoXmlNameCollectionHandler::FdoXmlNameCollectionHandler(FdoStringCollection* pNames)
{
    mNames = FDO_SAFE_ADDREF(pNames);

    // Create an empty name collection if none provided
    if ( !mNames ) 
        mNames = FdoStringCollection::Create();
}

FdoXmlNameCollectionHandler::~FdoXmlNameCollectionHandler(void)
{
}

FdoXmlNameCollectionHandler* FdoXmlNameCollectionHandler::Create(FdoStringCollection* pNames)
{
    return new FdoXmlNameCollectionHandler(pNames);
}

FdoStringCollection* FdoXmlNameCollectionHandler::GetNames()
{
    return FDO_SAFE_ADDREF( (FdoStringCollection*) mNames );
}

FdoXmlSaxHandler* FdoXmlNameCollectionHandler::XmlStartElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname, 
    FdoXmlAttributeCollection* atts
)
{
    FdoXmlSaxHandler*   pRet = NULL;

    // Name is the only expected element.
    if ( wcscmp(name, L"Name") == 0 ) {
        // Set up SAX handler to read the name from the element contents
        mNameHandler = FdoXmlCharDataHandler::Create();
        pRet = mNameHandler;
    }
    else {
        FdoXmlContext*      fdoContext = dynamic_cast<FdoXmlContext*>(context);
        
        // For unsupported elements, log error when error level is high.
        if ( fdoContext && (FdoXmlFlagsP(fdoContext->GetFlags())->GetErrorLevel() == FdoXmlFlags::ErrorLevel_High) ) { 
            fdoContext->AddError(
                    FdoXmlExceptionP(
                        FdoXmlException::Create(
                            FdoXmlException::NLSGetMessage(
                                FDO_NLSID(FDO_52_BADSUBELEMENT),
                                name,
                                L"fdo:NameCollectionType"
                            )
                        )
                    )
                );
        }

        // Skip the contents of unsupported elements.
        mXmlSkipper = FdoXmlSkipElementHandler::Create();
        pRet = mXmlSkipper;
    }

    return(pRet);
}

FdoBoolean FdoXmlNameCollectionHandler::XmlEndElement(
    FdoXmlSaxContext* context, 
    FdoString* uri, 
    FdoString* name, 
    FdoString* qname
)
{
    FdoXmlContext* fdoContext = dynamic_cast<FdoXmlContext*>(context);

    if ( wcscmp(name, L"Name") == 0 ) {
        // Get the name from the element contents
        FdoStringP nameData = mNameHandler->GetString();

        // Decode the name
        if ( fdoContext )
            // When the context is an FDO context, decoding might be currently
            // turned off. fdoContext->DecodeName() handles this optional name
            // decoding
            nameData = fdoContext->DecodeName( nameData );
        else
            // When context not an FDO context, do the default action, which is 
            // to always decode the name.
            nameData = FdoXmlReaderP(context->GetReader())->DecodeName( nameData );

        mNames->Add( nameData );
    }

    mNameHandler = NULL;

    return false;
}

void FdoXmlNameCollectionHandler::_writeXml(FdoXmlWriter* xmlWriter, const FdoXmlFlags* flags)
{
    FdoInt32 idx;

    for ( idx = 0; idx < mNames->GetCount(); idx++ ) {
        xmlWriter->WriteStartElement( L"Name" );
        FdoStringP name = mNames->GetString(idx);
        // Encode the name in case it contains invalid characters.
        xmlWriter->WriteCharacters( flags->GetNameAdjust() ? xmlWriter->EncodeName(name) : name );
        xmlWriter->WriteEndElement();
    }
}


