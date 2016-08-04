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

FdoXmlCopyHandler::FdoXmlCopyHandler( FdoXmlWriter* writer )
{
    SetWriter( writer );
    mWroteCurrElement = false;
}

FdoXmlCopyHandler::FdoXmlCopyHandler( 
    FdoXmlWriter* writer,
    FdoString* uri,
    FdoString* name, 
    FdoString* qName, 
    FdoXmlAttributeCollection* atts,
    FdoDictionary* namespaces
)
{
    SetWriter( writer );

    WriteStartElement( uri, name, qName, atts );

    if ( namespaces ) 
        WriteNamespaces( namespaces );

    mWroteCurrElement = true;
}

FdoXmlCopyHandler::~FdoXmlCopyHandler()
{
    // Close the element written by the constructor, if any.
    // Sub-elements are closed in XmlEndElement().
    if ( mWriter && mWroteCurrElement ) 
        mWriter->WriteEndElement();
}

FdoXmlSaxHandler* FdoXmlCopyHandler::XmlStartElement(
    FdoXmlSaxContext* pContext, 
    FdoString* uri , 
    FdoString* name, 
    FdoString* qName, 
    FdoXmlAttributeCollection* atts
)
{
    // Skip the fdo:DataStore element if it was automatically written.
    if ( !(mWriter && mWriter->GetDefaultRoot() && FdoXml::mDefaultRoot == qName) ) {
        WriteStartElement( uri, name, qName, atts );

        FdoXmlReaderP reader = pContext->GetReader();

        // Make sure all of the element's namespace declarations get into
        // the destination document.
        WriteNamespaces( FdoDictionaryP(reader->GetNamespaces()) );
    }

    // Keep this copy handler as the current SAX event handler.
    return NULL;
}

FdoBoolean FdoXmlCopyHandler::XmlEndElement(FdoXmlSaxContext*, FdoString*, FdoString*, FdoString* qName)
{
    // Skip the end of any fdo:DataStore element if start was skipped.
    if ( mWriter && !(mWriter->GetDefaultRoot() && (FdoXml::mDefaultRoot == qName)) ) 
        mWriter->WriteEndElement();

    return(false);
}

void FdoXmlCopyHandler::XmlCharacters(FdoXmlSaxContext* pContext, FdoString* chars)
{
    if ( mWriter ) 
        mWriter->WriteCharacters( chars );
}

FdoBoolean FdoXmlCopyHandler::AttHasNs(
    FdoString* uri,
    FdoString* name, 
    FdoString* qName, 
    FdoXmlAttribute* att
)
{
    // Attribute value has namespace if it references elements in an XML Schema.
    if ( FdoStringP(uri) == FdoXml::mXsUri ) {
        FdoStringP attName = att->GetLocalName();
        if ( attName == L"base" || attName == L"type" || attName == L"ref" )
            return true;
        else
            return false;
    }
    else {
        return false;
    }
}

void FdoXmlCopyHandler::SetWriter( FdoXmlWriter* writer )
{
    mWriter = FDO_SAFE_ADDREF(writer);
}

void FdoXmlCopyHandler::WriteStartElement(
    FdoString* uri, 
    FdoString* name, 
    FdoString* qName, 
    FdoXmlAttributeCollection* atts
)
{
    FdoInt32 idx;

    if ( mWriter ) {
        // An element's uri doesn't change but the namespace abbreviation
        // can change between documents.
        // The following generates the proper qualified name for the element,
        // based on uri and name, and the namespaces declared in the destination
        // document.
        FdoStringP outQName = mWriter->UriToQName(uri, name);

        if ( outQName.GetLength() > 0 ) {
            mWriter->WriteStartElement( outQName );
        }
        else {
            // The uri is not currently declared in the destination document.
            // Just write the unqualified name,
            mWriter->WriteStartElement( name );

            // If the element has no uri, it's likely because of an
            // error in the source document. Copy the element anyway,
            // just skip the uri handling.
            if ( wcslen(uri) > 0 ) {
                // If the qualified name has a namespace, add declaration
                // mapping the namespace to the uri. This saves having to 
                // re-declare the uri in any sub-elements of this element.
                FdoStringsP tokens = FdoStringCollection::Create( qName, L":" );
                if ( tokens->GetCount() == 2 ) 
                    mWriter->WriteAttribute( FdoXml::mXmlnsPref + L":" + tokens->GetString(0), uri );
                
                // Unqualified element name was written, so must declare the 
                // URI as the default namespace.
                mWriter->WriteAttribute( FdoXml::mXmlnsPref, uri );
            }
        }

        // Write out the attributes.

        for ( idx = 0; idx < atts->GetCount(); idx++ ) {
            FdoXmlAttributeP att = atts->GetItem(idx);
            FdoStringP attUri = att->GetUri();
            FdoStringP localName = att->GetLocalName();
            FdoStringP prefix = att->GetPrefix();

            // The following converts any ns prefix on the attribute
            // name to the appropriate prefix in the output document.
            FdoStringP attQName = HandleQAtt(
                att->GetUri(),
                att->GetLocalName(),
                att->GetPrefix(),
                att->GetQName()
            );
            
            FdoStringP attQValue = att->GetValue();
            FdoStringP valueUri = att->GetValueUri();

            // Check if attribute value may be namespace qualified (references
            // an XML element).
            if ( AttHasNs(uri, name, qName, att) && (valueUri.GetLength() > 0) ) {
                // It is qualified so convert prefix to appropriate one in 
                // output document.
                attQValue = HandleQAtt(
                    att->GetValueUri(),
                    att->GetLocalValue(),
                    att->GetValuePrefix(),
                    attQValue
                );
            }

            mWriter->WriteAttribute( attQName, attQValue );
        }
    }
}

void FdoXmlCopyHandler::WriteNamespaces( FdoDictionary* namespaces )
{
    int i;

    for ( i = 0; i < namespaces->GetCount(); i++ ) {
        FdoDictionaryElementP ns = namespaces->GetItem(i);
        FdoStringP nsPrefix = ns->GetName();
        FdoStringP nsUri = ns->GetValue();

        // Skip default namespace declarations
        if ( (nsPrefix.GetLength() > 0) && (nsUri.GetLength() > 0) ) {
            // Check if this namespace is already declared in the output
            // document. If a qname can 
            // be generated from it then it is already declared
            FdoStringP attQName = mWriter->UriToQName(ns->GetValue(), L"", true);

            if ( attQName.GetLength() == 0 )
                // namespace not declared so declare it.
                mWriter->WriteAttribute( 
                    FdoStringP(FdoXml::mXmlnsPref) + L":" + ns->GetName(), 
                    ns->GetValue()
                );
        }
    }

}

FdoStringP FdoXmlCopyHandler::HandleQAtt(FdoStringP uri, FdoStringP localName, FdoStringP prefix, FdoStringP QName  )
{
    // Generate qname from uri and local name
    FdoStringP outQName = mWriter->UriToQName(uri, localName, false);

    if ( outQName.GetLength() == 0 ) {
        // qname could not be generated so need to declare prefix and uri.
        if ( uri.GetLength() > 0 ) {
            if ( prefix.GetLength() > 0 )
                mWriter->WriteAttribute( FdoXml::mXmlnsPref + L":" + prefix, uri ); 
            else
                mWriter->WriteAttribute( FdoXml::mXmlnsPref, uri ); 
        }

        outQName = QName;
    }

    return outQName;
}

