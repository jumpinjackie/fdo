/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "stdafx.h"
#include "FdoWfsSchemaRefHandler.h"
#include "FdoWfsGlobals.h"

FdoWfsSchemaRefHandler::FdoWfsSchemaRefHandler()
{
    m_refs = FdoDictionary::Create();
}

FdoXmlSaxHandler* FdoWfsSchemaRefHandler::XmlStartElement( 
	FdoXmlSaxContext* saxContext,
	FdoString* uri, 
	FdoString* localName, 
	FdoString* qName, 
	FdoXmlAttributeCollection* atts 
)
{
    if ( FdoXml::mXsUri == uri ) {
        FdoStringP pLocalName( localName );
        FdoXmlAttributeP nsAtt;
        FdoXmlAttributeP locationAtt;

        if ( pLocalName == FdoWfsGlobals::import ) {
            nsAtt = atts->FindItem( FdoWfsGlobals::NameSpace );
            locationAtt = atts->FindItem( FdoWfsGlobals::schemaLocation );
        }

        if ( pLocalName == FdoWfsGlobals::include ) {
            locationAtt = atts->FindItem( FdoWfsGlobals::schemaLocation );
        }

        if ( locationAtt ) {
            FdoStringP location = locationAtt->GetValue();

            // Avoid adding same location twice.
            if ( !m_refs->FindItem(location) ) {
                FdoStringP uri;
                if ( nsAtt ) 
                    uri = nsAtt->GetValue();

                // The WFS server may return error if there are too many type names in a request.
                // To simplify the check, we split the location string to several short strings 
                // when its length is greater than 2048.
                if (location.GetLength() > 2048) {
                    // Find "TYPENAME=" in the location string.
                    FdoStringP delimiterTypeName = FdoWfsGlobals::TYPENAME;
                    delimiterTypeName += FdoWfsGlobals::Equal;
                    std::wstring tempLocation = (FdoString*)location.Upper();
                    std::wstring::size_type pos = tempLocation.find(delimiterTypeName);

                    // Get the base Url and type names.
                    FdoStringP baseUrl = location.Mid(0, pos+9);
                    FdoStringP typeNames = location.Mid(pos+9, location.GetLength()-pos-9);

                    // Replace encoded "%2C" with ",", then create a Fdo string collection that is tokenized from ",".
                    typeNames = typeNames.Replace(FdoWfsGlobals::CommaEncoding, FdoWfsGlobals::Comma);
                    FdoStringsP subTypeNames = FdoStringCollection::Create(typeNames, FdoWfsGlobals::Comma);
                    FdoStringP subLocation = L"";

                    for (FdoInt32 index = 0; index < subTypeNames->GetCount(); index++) {
                        FdoStringP subType = subTypeNames->GetString(index);
                        subLocation += subType;
                        // Create ref item for each 50 type names. 
                        // NOTE: We didn't find the exact count of type names that the WFS server supports.
                        // 50 is an empirical value which ensures the request can be handled by server.
                        if (0 == (index+1) % 50) {
                            FdoDictionaryElementP item = FdoDictionaryElement::Create( baseUrl + subLocation, uri );
                            m_refs->Add( item );
                            subLocation = L"";
                        } else {
                            subLocation += FdoWfsGlobals::CommaEncoding;
                        }
                    }
                    if (subLocation != L"") {
                        //remove last '%2C'
                        subLocation = subLocation.Mid(0, subLocation.GetLength() - 3);
                        FdoDictionaryElementP item = FdoDictionaryElement::Create( baseUrl + subLocation, uri );
                        m_refs->Add( item );
                    }
                }
                else {
                    FdoDictionaryElementP item = FdoDictionaryElement::Create( location, uri );
                    m_refs->Add( item );
                }
            }
        }
    }
    return NULL;
}



